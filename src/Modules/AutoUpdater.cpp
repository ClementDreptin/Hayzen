#include "pch.h"
#include "Modules/AutoUpdater.h"

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

static std::string GetVersionStringFromBody(const std::string &body)
{
    HRESULT hr = S_OK;

    const std::string versionKey = "tag_name";

    char key[128] = {};
    char value[128] = {};
    JSONTOKENTYPE tokenType = Json_NotStarted;
    DWORD unused = 0;

    // Setup XJSON
    HJSONREADER reader = XJSONCreateReader();
    hr = XJSONSetBuffer(reader, body.c_str(), body.size(), TRUE);
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't set the JSON buffers: %X.", hr);
        return value;
    }

    for (;;)
    {
        // Parse the body
        hr = XJSONReadToken(reader, &tokenType, &unused, &unused);
        if (FAILED(hr))
        {
            DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't parse JSON body: %X.", hr);
            break;
        }

        // End of body reached
        if (hr == S_FALSE)
        {
            DebugPrint("[Hayzen][AutoUpdater]: Error: Reached end of JSON body without finding the version.");
            break;
        }

        if (tokenType != Json_FieldName)
            continue;

        // Get the key name
        hr = XJSONGetTokenValue(reader, key, sizeof(key));
        if (FAILED(hr))
        {
            DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't read key from JSON: %X.", hr);
            break;
        }

        // If the current key is not the version key, skip
        if (strncmp(key, versionKey.c_str(), sizeof(key)))
            continue;

        // Parse the value at the version key
        hr = XJSONReadToken(reader, &tokenType, &unused, &unused);
        if (FAILED(hr))
        {
            DebugPrint(
                L"[Hayzen][AutoUpdater]: Error: Couldn't parse \"%s\" value from JSON: %X.",
                key,
                hr
            );
            break;
        }

        // If the value of the version key is not a string, something is wrong in the response
        if (tokenType != Json_String)
        {
            DebugPrint(
                L"[Hayzen][AutoUpdater]: Error: Incorrect type found for \"%s\", expected %d and got %d.",
                key,
                Json_String,
                tokenType
            );
            break;
        }

        // Read the value at the version key
        hr = XJSONGetTokenValue(reader, value, sizeof(value));
        if (FAILED(hr))
        {
            DebugPrint(
                L"[Hayzen][AutoUpdater]: Error: Couldn't read \"%s\" value from JSON: %X.",
                key,
                hr
            );
            break;
        }

        break;
    }

    XJSONCloseReader(reader);

    return value;
}

static std::string GetAssetUrlFromBody(const std::string &body)
{
    HRESULT hr = S_OK;

    const std::string assetsArrayKey = "assets";
    const std::string assetUrlKey = "url";

    char key[128] = {};
    char value[2048] = {};
    JSONTOKENTYPE tokenType = Json_NotStarted;
    DWORD unused = 0;

    // Setup XJSON
    HJSONREADER reader = XJSONCreateReader();
    hr = XJSONSetBuffer(reader, body.c_str(), body.size(), TRUE);
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't set the JSON buffers: %X.", hr);
        return value;
    }

    for (;;)
    {
        // Parse the body
        hr = XJSONReadToken(reader, &tokenType, &unused, &unused);
        if (FAILED(hr))
        {
            DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't parse JSON body: %X.", hr);
            break;
        }

        // End of body reached
        if (hr == S_FALSE)
        {
            DebugPrint("[Hayzen][AutoUpdater]: Error: Reached end of JSON body without finding the asset URL.");
            break;
        }

        if (tokenType != Json_FieldName)
            continue;

        // Get the key name
        hr = XJSONGetTokenValue(reader, key, sizeof(key));
        if (FAILED(hr))
        {
            DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't read key from JSON: %X.", hr);
            break;
        }

        // If the current key is assets array key, skip
        if (strncmp(key, assetsArrayKey.c_str(), sizeof(key)))
            continue;

        // Parse the value at the assets array key
        hr = XJSONReadToken(reader, &tokenType, &unused, &unused);
        if (FAILED(hr))
        {
            DebugPrint(
                L"[Hayzen][AutoUpdater]: Error: Couldn't parse \"%s\" value from JSON: %X.",
                key,
                hr
            );
            break;
        }

        // If the value of the assets array key is not an array, something is wrong in the response
        if (tokenType != Json_BeginArray)
        {
            DebugPrint(
                L"[Hayzen][AutoUpdater]: Error: Incorrect type found for \"%s\", expected %d and got %d.",
                key,
                Json_BeginArray,
                tokenType
            );
            break;
        }

        // Parse the content of the array
        hr = XJSONReadToken(reader, &tokenType, &unused, &unused);
        if (FAILED(hr))
        {
            DebugPrint(
                L"[Hayzen][AutoUpdater]: Error: Couldn't parse \"%s\" value from JSON: %X.",
                key,
                hr
            );
            break;
        }

        // If the first value of the assets array is not an object, something is wrong in the response
        if (tokenType != Json_BeginObject)
        {
            DebugPrint(
                L"[Hayzen][AutoUpdater]: Error: Incorrect type found for \"%s\", expected %d and got %d.",
                key,
                Json_BeginArray,
                tokenType
            );
            break;
        }
    }

    XJSONCloseReader(reader);

    return value;
}

struct LatestVersionMetadata
{
    std::string Version;
    std::string AssetUrl;
};

static HRESULT GetLatestVersion(LatestVersionMetadata &metadata)
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
    std::string versionString = GetVersionStringFromBody(body);
    if (versionString.empty())
        return E_FAIL;

    metadata.Version = versionString;
    DebugPrint("version: %s", versionString.c_str());

    // Get the URL of the latest binary
    std::string assetUrl = GetAssetUrlFromBody(body);
    if (assetUrl.empty())
        return E_FAIL;

    metadata.AssetUrl = assetUrl;
    DebugPrint("asset URL: %s", assetUrl.c_str());

    return hr;
}

HRESULT Run()
{
    HRESULT hr = S_OK;

    LatestVersionMetadata metadata = {};
    hr = GetLatestVersion(metadata);
    if (FAILED(hr))
        return hr;

    return hr;
}

}
