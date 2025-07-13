#include "pch.h"
#include "Modules/AutoUpdater.h"

#include "Core/Plugin.h"
#include "Modules/Http.h"
#include "Modules/Json.h"

namespace AutoUpdater
{

static const uint8_t s_SectigoECC_DN[] = {
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

static const uint8_t s_SectigoECC_Q[] = {
    0x04, 0x79, 0x18, 0x93, 0xCA, 0x9F, 0x6D, 0x9E, 0x6C, 0x57, 0x00, 0x23,
    0x05, 0x37, 0x0B, 0x5F, 0x0F, 0x58, 0x5A, 0xC4, 0xDE, 0x7F, 0x55, 0xA3,
    0xE9, 0x1E, 0xD6, 0xD9, 0x25, 0x0A, 0x88, 0xA0, 0x20, 0x4A, 0x1D, 0x7A,
    0x4F, 0x05, 0x30, 0x8A, 0x63, 0x49, 0x13, 0x8C, 0x64, 0x21, 0x07, 0x95,
    0xFD, 0x3A, 0x35, 0xE1, 0x4A, 0xCE, 0x90, 0xF0, 0x18, 0xF7, 0x3D, 0xAF,
    0x68, 0xA6, 0xFB, 0xD4, 0x48
};

static const uint8_t s_SectigoRSA_DN[] = {
    0x30, 0x81, 0x8F, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06,
    0x13, 0x02, 0x47, 0x42, 0x31, 0x1B, 0x30, 0x19, 0x06, 0x03, 0x55, 0x04,
    0x08, 0x13, 0x12, 0x47, 0x72, 0x65, 0x61, 0x74, 0x65, 0x72, 0x20, 0x4D,
    0x61, 0x6E, 0x63, 0x68, 0x65, 0x73, 0x74, 0x65, 0x72, 0x31, 0x10, 0x30,
    0x0E, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x07, 0x53, 0x61, 0x6C, 0x66,
    0x6F, 0x72, 0x64, 0x31, 0x18, 0x30, 0x16, 0x06, 0x03, 0x55, 0x04, 0x0A,
    0x13, 0x0F, 0x53, 0x65, 0x63, 0x74, 0x69, 0x67, 0x6F, 0x20, 0x4C, 0x69,
    0x6D, 0x69, 0x74, 0x65, 0x64, 0x31, 0x37, 0x30, 0x35, 0x06, 0x03, 0x55,
    0x04, 0x03, 0x13, 0x2E, 0x53, 0x65, 0x63, 0x74, 0x69, 0x67, 0x6F, 0x20,
    0x52, 0x53, 0x41, 0x20, 0x44, 0x6F, 0x6D, 0x61, 0x69, 0x6E, 0x20, 0x56,
    0x61, 0x6C, 0x69, 0x64, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x53, 0x65,
    0x63, 0x75, 0x72, 0x65, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x20,
    0x43, 0x41
};

static const uint8_t s_SectigoRSA_N[] = {
    0xD6, 0x73, 0x33, 0xD6, 0xD7, 0x3C, 0x20, 0xD0, 0x00, 0xD2, 0x17, 0x45,
    0xB8, 0xD6, 0x3E, 0x07, 0xA2, 0x3F, 0xC7, 0x41, 0xEE, 0x32, 0x30, 0xC9,
    0xB0, 0x6C, 0xFD, 0xF4, 0x9F, 0xCB, 0x12, 0x98, 0x0F, 0x2D, 0x3F, 0x8D,
    0x4D, 0x01, 0x0C, 0x82, 0x0F, 0x17, 0x7F, 0x62, 0x2E, 0xE9, 0xB8, 0x48,
    0x79, 0xFB, 0x16, 0x83, 0x4E, 0xAD, 0xD7, 0x32, 0x25, 0x93, 0xB7, 0x07,
    0xBF, 0xB9, 0x50, 0x3F, 0xA9, 0x4C, 0xC3, 0x40, 0x2A, 0xE9, 0x39, 0xFF,
    0xD9, 0x81, 0xCA, 0x1F, 0x16, 0x32, 0x41, 0xDA, 0x80, 0x26, 0xB9, 0x23,
    0x7A, 0x87, 0x20, 0x1E, 0xE3, 0xFF, 0x20, 0x9A, 0x3C, 0x95, 0x44, 0x6F,
    0x87, 0x75, 0x06, 0x90, 0x40, 0xB4, 0x32, 0x93, 0x16, 0x09, 0x10, 0x08,
    0x23, 0x3E, 0xD2, 0xDD, 0x87, 0x0F, 0x6F, 0x5D, 0x51, 0x14, 0x6A, 0x0A,
    0x69, 0xC5, 0x4F, 0x01, 0x72, 0x69, 0xCF, 0xD3, 0x93, 0x4C, 0x6D, 0x04,
    0xA0, 0xA3, 0x1B, 0x82, 0x7E, 0xB1, 0x9A, 0xB9, 0xED, 0xC5, 0x9E, 0xC5,
    0x37, 0x78, 0x9F, 0x9A, 0x08, 0x34, 0xFB, 0x56, 0x2E, 0x58, 0xC4, 0x09,
    0x0E, 0x06, 0x64, 0x5B, 0xBC, 0x37, 0xDC, 0xF1, 0x9F, 0x28, 0x68, 0xA8,
    0x56, 0xB0, 0x92, 0xA3, 0x5C, 0x9F, 0xBB, 0x88, 0x98, 0x08, 0x1B, 0x24,
    0x1D, 0xAB, 0x30, 0x85, 0xAE, 0xAF, 0xB0, 0x2E, 0x9E, 0x7A, 0x9D, 0xC1,
    0xC0, 0x42, 0x1C, 0xE2, 0x02, 0xF0, 0xEA, 0xE0, 0x4A, 0xD2, 0xEF, 0x90,
    0x0E, 0xB4, 0xC1, 0x40, 0x16, 0xF0, 0x6F, 0x85, 0x42, 0x4A, 0x64, 0xF7,
    0xA4, 0x30, 0xA0, 0xFE, 0xBF, 0x2E, 0xA3, 0x27, 0x5A, 0x8E, 0x8B, 0x58,
    0xB8, 0xAD, 0xC3, 0x19, 0x17, 0x84, 0x63, 0xED, 0x6F, 0x56, 0xFD, 0x83,
    0xCB, 0x60, 0x34, 0xC4, 0x74, 0xBE, 0xE6, 0x9D, 0xDB, 0xE1, 0xE4, 0xE5,
    0xCA, 0x0C, 0x5F, 0x15
};

static const uint8_t s_SectigoRSA_E[] = {
    0x01, 0x00, 0x01
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
    hr = socket.AddECTrustAnchor(s_SectigoECC_DN, sizeof(s_SectigoECC_DN), s_SectigoECC_Q, sizeof(s_SectigoECC_Q), TlsSession::Curve_secp256r1);
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't add EC trust anchor.");
        return hr;
    }

    // Register GitHub's RSA trust anchor
    hr = socket.AddRsaTrustAnchor(s_SectigoRSA_DN, sizeof(s_SectigoRSA_DN), s_SectigoRSA_N, sizeof(s_SectigoRSA_N), s_SectigoRSA_E, sizeof(s_SectigoRSA_E));
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't add RSA trust anchor.");
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
    NTSTATUS status = 0;

    // This is an NT device path, which means we need to use the kernel functions to
    // open and write to the file
    std::string pluginPath = g_pPlugin->GetFullPath();

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
        TRUNCATE_EXISTING,
        0x20 // FILE_SYNCHRONOUS_IO_NONALERT
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
