#include "pch.h"
#include "Modules/AutoUpdater.h"

#include "Core/Plugin.h"

namespace AutoUpdater
{

static const unsigned char s_GitHubDN[] = {
    0x30, 0x81, 0x8F, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06,
    0x13, 0x02, 0x47, 0x42, 0x31, 0x1B, 0x30, 0x19, 0x06, 0x03, 0x55, 0x04,
    0x08, 0x13, 0x12, 0x47, 0x72, 0x65, 0x61, 0x74, 0x65, 0x72, 0x20, 0x4D,
    0x61, 0x6E, 0x63, 0x68, 0x65, 0x73, 0x74, 0x65, 0x72, 0x31, 0x10, 0x30,
    0x0E, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x07, 0x53, 0x61, 0x6C, 0x66,
    0x6F, 0x72, 0x64, 0x31, 0x18, 0x30, 0x16, 0x06, 0x03, 0x55, 0x04, 0x0A,
    0x13, 0x0F, 0x53, 0x65, 0x63, 0x74, 0x69, 0x67, 0x6F, 0x20, 0x4C, 0x69,
    0x6D, 0x69, 0x74, 0x65, 0x64, 0x31, 0x37, 0x30, 0x35, 0x06, 0x03, 0x55,
    0x04, 0x03, 0x13, 0x2E, 0x53, 0x65, 0x63, 0x74, 0x69, 0x67, 0x6F, 0x20,
    0x45, 0x43, 0x43, 0x20, 0x44, 0x6F, 0x6D, 0x61, 0x69, 0x6E, 0x20, 0x56,
    0x61, 0x6C, 0x69, 0x64, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x53, 0x65,
    0x63, 0x75, 0x72, 0x65, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x20,
    0x43, 0x41
};

static const unsigned char s_GitHubECQ[] = {
    0x04, 0x79, 0x18, 0x93, 0xCA, 0x9F, 0x6D, 0x9E, 0x6C, 0x57, 0x00, 0x23,
    0x05, 0x37, 0x0B, 0x5F, 0x0F, 0x58, 0x5A, 0xC4, 0xDE, 0x7F, 0x55, 0xA3,
    0xE9, 0x1E, 0xD6, 0xD9, 0x25, 0x0A, 0x88, 0xA0, 0x20, 0x4A, 0x1D, 0x7A,
    0x4F, 0x05, 0x30, 0x8A, 0x63, 0x49, 0x13, 0x8C, 0x64, 0x21, 0x07, 0x95,
    0xFD, 0x3A, 0x35, 0xE1, 0x4A, 0xCE, 0x90, 0xF0, 0x18, 0xF7, 0x3D, 0xAF,
    0x68, 0xA6, 0xFB, 0xD4, 0x48
};

static std::string GetHttpResponseBody(const std::string &response)
{
    const std::string headersEnd = "\r\n\r\n";

    size_t headersEndPos = response.find(headersEnd);
    if (headersEndPos == std::string::npos)
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't separate headers from body of HTTP response.");
        return "";
    }

    return response.substr(headersEndPos + headersEnd.size());
}

static HRESULT ReadUpToKey(HJSONREADER reader, const std::string &key)
{
    HRESULT hr = S_OK;

    char buffer[128] = {};
    JSONTOKENTYPE tokenType = Json_NotStarted;
    DWORD unused = 0;

    for (;;)
    {
        // Parse the body
        hr = XJSONReadToken(reader, &tokenType, &unused, &unused);
        if (FAILED(hr))
        {
            DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't parse JSON body: %X.", hr);
            return hr;
        }

        // End of body reached
        if (hr == S_FALSE)
        {
            DebugPrint("[Hayzen][AutoUpdater]: Error: Reached end of JSON without finding \"%s\".", key.c_str());
            return E_FAIL;
        }

        if (tokenType != Json_FieldName)
            continue;

        // Get the key name
        hr = XJSONGetTokenValue(reader, buffer, sizeof(buffer));
        if (FAILED(hr))
        {
            DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't read key from JSON: %X.", hr);
            return hr;
        }

        // If the current key is the key we're looking for, stop
        if (strncmp(buffer, key.c_str(), sizeof(buffer)) == 0)
            break;
    }

    return hr;
}

static HRESULT ReadTokenType(HJSONREADER reader, JSONTOKENTYPE expectedType)
{
    HRESULT hr = S_OK;

    JSONTOKENTYPE actualType = Json_NotStarted;
    DWORD unused = 0;

    // Parse the value at the version key
    hr = XJSONReadToken(reader, &actualType, &unused, &unused);
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't read token from JSON: %X.", hr);
        return hr;
    }

    // If the value of the version key is not a string, something is wrong in the response
    if (actualType != expectedType)
    {
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Unexpected token type found, expected %d and got %d.",
            expectedType,
            actualType
        );
        return E_FAIL;
    }

    return hr;
}

static std::string GetVersionNameFromBody(const std::string &body)
{
    HRESULT hr = S_OK;

    const std::string versionKey = "tag_name";
    char value[128] = {};

    // Setup XJSON
    HJSONREADER reader = XJSONCreateReader();
    hr = XJSONSetBuffer(reader, body.c_str(), body.size(), TRUE);
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't set the JSON buffers: %X.", hr);
        return value;
    }

    // Read up until the version key is found
    hr = ReadUpToKey(reader, versionKey);
    if (FAILED(hr))
        return value;

    // Open the value at the version key
    hr = ReadTokenType(reader, Json_String);
    if (FAILED(hr))
        return value;

    // Read the value at the version key
    hr = XJSONGetTokenValue(reader, value, sizeof(value));
    if (FAILED(hr))
    {
        DebugPrint(
            L"[Hayzen][AutoUpdater]: Error: Couldn't read \"%s\" value from JSON: %X.",
            versionKey.c_str(),
            hr
        );
        return value;
    }

    XJSONCloseReader(reader);

    return value;
}

static std::string GetDownloadUrlFromBody(const std::string &body)
{
    HRESULT hr = S_OK;

    const std::string assetsArrayKey = "assets";
    const std::string downloadUrlKey = "browser_download_url";
    char value[128] = {};

    // Setup XJSON
    HJSONREADER reader = XJSONCreateReader();
    hr = XJSONSetBuffer(reader, body.c_str(), body.size(), TRUE);
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't set the JSON buffers: %X.", hr);
        return value;
    }

    // Read up until the assets array key is found
    hr = ReadUpToKey(reader, assetsArrayKey);
    if (FAILED(hr))
        return value;

    // Open the array
    hr = ReadTokenType(reader, Json_BeginArray);
    if (FAILED(hr))
        return value;

    // Open the first object
    hr = ReadTokenType(reader, Json_BeginObject);
    if (FAILED(hr))
        return value;

    // Read up until the download URL key is found
    hr = ReadUpToKey(reader, downloadUrlKey);
    if (FAILED(hr))
        return value;

    // Open the value at the download URL key
    hr = ReadTokenType(reader, Json_String);
    if (FAILED(hr))
        return value;

    // Read the value at the download URL key
    hr = XJSONGetTokenValue(reader, value, sizeof(value));
    if (FAILED(hr))
    {
        DebugPrint(
            L"[Hayzen][AutoUpdater]: Error: Couldn't read \"%s\" value from JSON: %X.",
            downloadUrlKey.c_str(),
            hr
        );
        return value;
    }

    XJSONCloseReader(reader);

    return value;
}

struct LatestVersion
{
    std::string Name;
    std::string DownloadUrl;
};

static HRESULT GetLatestVersion(LatestVersion &latestVersion)
{
    HRESULT hr = S_OK;

    std::string domain = "api.github.com";
    Socket socket(domain, 443);

    // Register the GitHub certificate
    hr = socket.AddECTrustAnchor(s_GitHubDN, sizeof(s_GitHubDN), s_GitHubECQ, sizeof(s_GitHubECQ), TlsSession::Curve_secp256r1);
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't add GitHub trust anchor.");
        return hr;
    }

    // Connect to GitHub
    hr = socket.Connect();
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't connect to GitHub.");
        return hr;
    }

    // Send the request
    std::string request = XexUtils::Formatter::Format(
        "GET /repos/ClementDreptin/Hayzen/releases/latest HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: Hayzen AutoUpdater\r\n"
        "Accept: application/json\r\n"
        "Connection: close\r\n\r\n",
        domain.c_str()
    );
    int bytesSent = socket.Send(request.c_str(), request.size());
    if (bytesSent < static_cast<int>(request.size()))
    {
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Not all bytes could be sent, "
            "expected to send %d but only sent %d.",
            request.size(),
            bytesSent
        );
        return E_FAIL;
    }

    // Get the response
    std::stringstream responseStream;
    char buffer[2048] = {};
    for (;;)
    {
        int bytesRead = socket.Receive(buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0)
            break;

        buffer[bytesRead] = '\0';
        responseStream << buffer;
    }

    // Extract the body from the response
    std::string response = responseStream.str();
    std::string body = GetHttpResponseBody(response);
    if (body.empty())
        return E_FAIL;

    // Get the version from the body
    std::string versionName = GetVersionNameFromBody(body);
    if (versionName.empty())
        return E_FAIL;

    // Get the URL of the latest binary
    std::string downloadUrl = GetDownloadUrlFromBody(body);
    if (downloadUrl.empty())
        return E_FAIL;

    latestVersion.Name = versionName;
    latestVersion.DownloadUrl = downloadUrl;

    return hr;
}

static bool AskToDownload()
{
    std::vector<std::wstring> buttonLabels(2);
    buttonLabels[0] = L"Yes";
    buttonLabels[1] = L"No";
    uint32_t buttonPressedIndex = 0;

    uint32_t result = Xam::ShowMessageBox(
        L"Update",
        L"A new version is available, would you like to download it?",
        buttonLabels,
        XMB_ALERTICON,
        &buttonPressedIndex,
        1
    );

    // Return true if the user closed the message box by clicking on "Yes"
    return result == ERROR_SUCCESS && buttonPressedIndex == 0;
}

HRESULT Run()
{
    HRESULT hr = S_OK;

    // Get information about the latest version available on GitHub
    LatestVersion latestVersion = {};
    hr = GetLatestVersion(latestVersion);
    if (FAILED(hr))
        return hr;

    // We're up to date, stop here
    std::string currentVersionName = g_pPlugin->GetVersion();
    if (currentVersionName == latestVersion.Name)
        return hr;

    // Ask the user if they want to download the latest version
    bool wantsToDownload = AskToDownload();
    if (!wantsToDownload)
        return hr;

    Log::Print("download latest version");

    return hr;
}

}
