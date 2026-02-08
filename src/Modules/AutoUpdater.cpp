#include "pch.h"
#include "Modules/AutoUpdater.h"

#include "Core/Plugin.h"
#include "Modules/Http.h"
#include "Modules/Json.h"

namespace AutoUpdater
{

static const uint8_t s_SectigoECC_DN[] = {
    0x30, 0x60, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
    0x02, 0x47, 0x42, 0x31, 0x18, 0x30, 0x16, 0x06, 0x03, 0x55, 0x04, 0x0A,
    0x13, 0x0F, 0x53, 0x65, 0x63, 0x74, 0x69, 0x67, 0x6F, 0x20, 0x4C, 0x69,
    0x6D, 0x69, 0x74, 0x65, 0x64, 0x31, 0x37, 0x30, 0x35, 0x06, 0x03, 0x55,
    0x04, 0x03, 0x13, 0x2E, 0x53, 0x65, 0x63, 0x74, 0x69, 0x67, 0x6F, 0x20,
    0x50, 0x75, 0x62, 0x6C, 0x69, 0x63, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65,
    0x72, 0x20, 0x41, 0x75, 0x74, 0x68, 0x65, 0x6E, 0x74, 0x69, 0x63, 0x61,
    0x74, 0x69, 0x6F, 0x6E, 0x20, 0x43, 0x41, 0x20, 0x44, 0x56, 0x20, 0x45,
    0x33, 0x36
};

static const uint8_t s_SectigoECC_Q[] = {
    0x04, 0x68, 0xA1, 0xA7, 0x6C, 0x05, 0x27, 0x05, 0x89, 0x63, 0x1C, 0x39,
    0xA7, 0xFF, 0x25, 0x21, 0xC5, 0xED, 0xD3, 0x2F, 0x12, 0x98, 0xBB, 0x2C,
    0xDC, 0xF5, 0x55, 0xE8, 0x49, 0xA0, 0x84, 0x57, 0x91, 0x7B, 0xDC, 0x58,
    0x5F, 0x6B, 0xF4, 0xA2, 0xFD, 0x13, 0x2C, 0x9B, 0x04, 0xC5, 0x5B, 0x74,
    0x7D, 0xB3, 0xB2, 0x7A, 0x26, 0x96, 0x19, 0x16, 0x6B, 0xB8, 0x76, 0x8D,
    0xEF, 0x93, 0xF5, 0x60, 0x97
};

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
    hr = Json::ReadUpToKey(reader, versionKey);
    if (FAILED(hr))
        return value;

    // Open the value at the version key
    hr = Json::ReadTokenType(reader, Json_String);
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
    hr = Json::ReadUpToKey(reader, assetsArrayKey);
    if (FAILED(hr))
        return value;

    // Open the array
    hr = Json::ReadTokenType(reader, Json_BeginArray);
    if (FAILED(hr))
        return value;

    // Open the first object
    hr = Json::ReadTokenType(reader, Json_BeginObject);
    if (FAILED(hr))
        return value;

    // Read up until the download URL key is found
    hr = Json::ReadUpToKey(reader, downloadUrlKey);
    if (FAILED(hr))
        return value;

    // Open the value at the download URL key
    hr = Json::ReadTokenType(reader, Json_String);
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

static HRESULT ConnectToGitHub(Socket &socket)
{
    HRESULT hr = S_OK;

    // Register GitHub's EC trust anchor
    hr = socket.AddECTrustAnchor(s_SectigoECC_DN, sizeof(s_SectigoECC_DN), s_SectigoECC_Q, sizeof(s_SectigoECC_Q), Socket::Curve_secp256r1);
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't add EC trust anchor.");
        return hr;
    }

    // Connect to GitHub
    hr = socket.Connect();
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't connect to GitHub.");
        return hr;
    }

    return hr;
}

struct LatestVersion
{
    std::string Name;
    std::string DownloadUrl;
};

static HRESULT GetLatestVersion(LatestVersion &latestVersion)
{
    HRESULT hr = S_OK;

    // Connect to GitHub
    std::string domain = "api.github.com";
    Socket socket(domain, 443);
    hr = ConnectToGitHub(socket);
    if (FAILED(hr))
        return hr;

    // Send the request
    hr = Http::SendRequest(socket, domain, "/repos/ClementDreptin/Hayzen/releases/latest");
    if (FAILED(hr))
        return hr;

    // Read HTTP status
    uint32_t status = Http::ReadStatus(socket);
    if (status != 200)
    {
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Invalid status code when trying to get the "
            "latest version number. Expected 200 but got %d.",
            status
        );
        return E_FAIL;
    }

    // Flush the headers
    auto headers = Http::ReadHeaders(socket);

    // Read the body
    std::string body = Http::ReadBody(socket);
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

static bool AskToDownload(const std::string &currentVersion, const std::string &latestVersion)
{
    std::vector<std::wstring> buttonLabels(2);
    buttonLabels[0] = L"Yes";
    buttonLabels[1] = L"No";
    uint32_t buttonPressedIndex = 0;

    std::string text = Formatter::Format(
        "Your current version is %s and %s is available, would you like to download it?",
        currentVersion.c_str(),
        latestVersion.c_str()
    );

    uint32_t result = Xam::ShowMessageBox(
        L"Hayzen update",
        Formatter::ToWide(text),
        buttonLabels,
        XMB_ALERTICON,
        &buttonPressedIndex,
        1
    );

    // Return true if the user closed the message box by clicking on "Yes"
    return result == ERROR_SUCCESS && buttonPressedIndex == 0;
}

static std::string GetFinalDownloadUrl(const std::string &url)
{
    HRESULT hr = S_OK;

    Http::Url parsedUrl = {};
    hr = Http::ParseUrl(url, parsedUrl);
    if (FAILED(hr))
        return "";

    // Connect to GitHub
    Socket socket(parsedUrl.Hostname, 443);
    hr = ConnectToGitHub(socket);
    if (FAILED(hr))
        return "";

    // Send the request
    hr = Http::SendRequest(socket, parsedUrl.Hostname, parsedUrl.Path);
    if (FAILED(hr))
        return "";

    // Read HTTP status
    uint32_t status = Http::ReadStatus(socket);
    if (status != 302)
    {
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Invalid status code when trying to follow "
            "redirect. Expected 302 but got %d.",
            status
        );
        return "";
    }

    // Read the headers
    auto headers = Http::ReadHeaders(socket);

    // Flush the body
    Http::ReadBody(socket);

    // The final download URL is in the "Location" header
    if (headers.find("Location") == headers.end())
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't find \"Location\" header in HTTP response.");
        return "";
    }

    return headers.at("Location");
}

static HRESULT ReadBodyToFile(Socket &socket)
{
    NTSTATUS status = STATUS_SUCESS;

    // This is an NT device path, which means we need to use the kernel functions to
    // open and write to the file
    Fs::Path pluginPath = g_pPlugin->GetFullPath();

    // Create the attributes from the path
    OBJECT_ATTRIBUTES attributes = {};
    STRING pluginPathString = {};
    RtlInitAnsiString(&pluginPathString, pluginPath.c_str());
    InitializeObjectAttributes(&attributes, &pluginPathString, 0, nullptr);

    // Open the file
    HANDLE handle = INVALID_HANDLE_VALUE;
    IO_STATUS_BLOCK block = {};
    status = NtCreateFile(
        &handle,
        GENERIC_WRITE,
        &attributes,
        &block,
        nullptr,
        0,
        FILE_SHARE_READ,
        FILE_OVERWRITE_IF,
        FILE_SYNCHRONOUS_IO_NONALERT
    );
    if (status != 0 || handle == INVALID_HANDLE_VALUE)
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't open \"%s\".", pluginPath.c_str());
        return status;
    }

    // Flush any remaining data in the internal HTTP buffer
    std::vector<char> httpInternalBuffer = Http::FlushInternalBuffer();
    status = NtWriteFile(
        handle,
        nullptr,
        nullptr,
        nullptr,
        &block,
        httpInternalBuffer.data(),
        httpInternalBuffer.size(),
        nullptr
    );
    if (status != 0)
    {
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Couldn't write internal buffer to \"%s\" (%X).",
            pluginPath.c_str(),
            status
        );
        NtClose(handle);
        return status;
    }

    // Get the response
    char buffer[2048] = {};
    for (;;)
    {
        int bytesRead = socket.Receive(buffer, sizeof(buffer));
        if (bytesRead <= 0)
            break;

        status = NtWriteFile(
            handle,
            nullptr,
            nullptr,
            nullptr,
            &block,
            buffer,
            bytesRead,
            nullptr
        );
        if (status != 0)
        {
            DebugPrint(
                "[Hayzen][AutoUpdater]: Error: Couldn't write to \"%s\" (%X)",
                pluginPath.c_str(),
                status
            );
            NtClose(handle);
            return status;
        }
    }

    NtClose(handle);

    return status;
}

static HRESULT Download(const std::string &url)
{
    HRESULT hr = S_OK;

    Http::Url parsedUrl = {};
    hr = Http::ParseUrl(url, parsedUrl);
    if (FAILED(hr))
        return hr;

    // Connect to GitHub
    Socket socket(parsedUrl.Hostname, 443);
    hr = ConnectToGitHub(socket);
    if (FAILED(hr))
        return hr;

    // Send the request
    hr = Http::SendRequest(socket, parsedUrl.Hostname, parsedUrl.Path);
    if (FAILED(hr))
        return hr;

    // Read HTTP status
    uint32_t status = Http::ReadStatus(socket);
    if (status != 200)
    {
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Invalid status code when trying to download. "
            "Expected 200 but got %d.",
            status
        );
        return E_FAIL;
    }

    // Flush the headers
    Http::ReadHeaders(socket);

    // Overwrite the old file with the new one
    hr = ReadBodyToFile(socket);
    if (FAILED(hr))
        return hr;

    return hr;
}

static void DisplaySuccessMessageBox(const std::string &latestVersion)
{
    std::vector<std::wstring> buttonLabels(1);
    buttonLabels[0] = L"OK";

    std::string text = Formatter::Format(
        "%s was successfully downloaded.\n\nChanges will apply the next time you load the plugin.",
        latestVersion.c_str()
    );

    Xam::ShowMessageBox(
        L"Hayzen updated!",
        Formatter::ToWide(text),
        buttonLabels,
        XMB_ALERTICON
    );
}

HRESULT Run()
{
    HRESULT hr = S_OK;

    // Get information about the latest version available on GitHub
    LatestVersion latestVersion = {};
    hr = GetLatestVersion(latestVersion);
    if (FAILED(hr))
        return hr;

    // If we're up to date, stop here
    std::string currentVersionName = g_pPlugin->GetVersion();
    if (currentVersionName == latestVersion.Name)
        return hr;

    // Ask the user if they want to download the latest version
    bool wantsToDownload = AskToDownload(currentVersionName, latestVersion.Name);
    if (!wantsToDownload)
        return hr;

    // The download URL returned by the API will return a redirect, so we need to follow it
    std::string finalDownloadUrl = GetFinalDownloadUrl(latestVersion.DownloadUrl);
    if (finalDownloadUrl.empty())
    {
        Xam::XNotify("Couldn't download the latest version.", Xam::XNOTIFYUI_TYPE_AVOID_REVIEW);
        return hr;
    }

    // Download the file and replace the existing one
    hr = Download(finalDownloadUrl);
    if (FAILED(hr))
    {
        Xam::XNotify("Couldn't download the latest version.", Xam::XNOTIFYUI_TYPE_AVOID_REVIEW);
        return hr;
    }

    // Let the user know everything succeeded
    DisplaySuccessMessageBox(latestVersion.Name);

    return hr;
}

}
