#include "pch.h"
#include "Modules/AutoUpdater.h"

#include "Core/Plugin.h"
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

static const uint8_t s_R12_RSA_DN[] = {
    0x30, 0x33, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
    0x02, 0x55, 0x53, 0x31, 0x16, 0x30, 0x14, 0x06, 0x03, 0x55, 0x04, 0x0A,
    0x13, 0x0D, 0x4C, 0x65, 0x74, 0x27, 0x73, 0x20, 0x45, 0x6E, 0x63, 0x72,
    0x79, 0x70, 0x74, 0x31, 0x0C, 0x30, 0x0A, 0x06, 0x03, 0x55, 0x04, 0x03,
    0x13, 0x03, 0x52, 0x31, 0x32
};

static const uint8_t s_R12_RSA_N[] = {
    0xDA, 0x98, 0x28, 0x74, 0xAD, 0xBE, 0x94, 0xFE, 0x3B, 0xE0, 0x1E, 0xE2,
    0xE5, 0x4B, 0x75, 0xAB, 0x2C, 0x12, 0x7F, 0xED, 0xA7, 0x03, 0x32, 0x7E,
    0x36, 0x97, 0xEC, 0xE8, 0x31, 0x8F, 0xA5, 0x13, 0x8D, 0x0B, 0x99, 0x2E,
    0x1E, 0xCD, 0x01, 0x51, 0x3D, 0x4C, 0xE5, 0x28, 0x6E, 0x09, 0x55, 0x31,
    0xAA, 0xA5, 0x22, 0x5D, 0x72, 0xF4, 0x2D, 0x07, 0xC2, 0x4D, 0x40, 0x3C,
    0xDF, 0x01, 0x23, 0xB9, 0x78, 0x37, 0xF5, 0x1A, 0x65, 0x32, 0x34, 0xE6,
    0x86, 0x71, 0x9D, 0x04, 0xEF, 0x84, 0x08, 0x5B, 0xBD, 0x02, 0x1A, 0x99,
    0xEB, 0xA6, 0x01, 0x00, 0x9A, 0x73, 0x90, 0x6D, 0x8F, 0xA2, 0x07, 0xA0,
    0xD0, 0x97, 0xD3, 0xDA, 0x45, 0x61, 0x81, 0x35, 0x3D, 0x14, 0xF9, 0xC4,
    0xC0, 0x5F, 0x6A, 0xDC, 0x0B, 0x96, 0x1A, 0xB0, 0x9F, 0xE3, 0x2A, 0xEA,
    0xBD, 0x2A, 0xD6, 0x98, 0xC7, 0x9B, 0x71, 0xAB, 0x3B, 0x74, 0x0F, 0x3C,
    0xDB, 0xB2, 0x60, 0xBE, 0x5A, 0x4B, 0x4E, 0x18, 0xE9, 0xDB, 0x2A, 0x73,
    0x5C, 0x89, 0x61, 0x65, 0x9E, 0xFE, 0xED, 0x3C, 0xA6, 0xCB, 0x4E, 0x6F,
    0xE4, 0x9E, 0xF9, 0x00, 0x46, 0xB3, 0xFF, 0x19, 0x4D, 0x2A, 0x63, 0xB3,
    0x8E, 0x66, 0xC6, 0x18, 0x85, 0x70, 0xC7, 0x50, 0x65, 0x6F, 0x3B, 0x74,
    0xE5, 0x48, 0x83, 0x0F, 0x08, 0x58, 0x5D, 0x2D, 0x23, 0x9D, 0x5E, 0xA3,
    0xFE, 0xE8, 0xDB, 0x00, 0xA1, 0xD2, 0xF4, 0xE3, 0x19, 0x4D, 0xF2, 0xEE,
    0x7A, 0xF6, 0x27, 0x9E, 0xE5, 0xCD, 0x9C, 0x2D, 0xA2, 0xF2, 0x7F, 0x9C,
    0x17, 0xAD, 0xEF, 0x13, 0x37, 0x39, 0xD1, 0xB4, 0xC8, 0x2C, 0x41, 0xD6,
    0x86, 0xC0, 0xE9, 0xEC, 0x21, 0xF8, 0x59, 0x1B, 0x7F, 0xB9, 0x3A, 0x7C,
    0x9F, 0x5C, 0x01, 0x9D, 0x62, 0x04, 0xC2, 0x28, 0xBD, 0x0A, 0xAD, 0x3C,
    0xCA, 0x10, 0xEC, 0x1B
};

static const uint8_t s_R12_RSA_E[] = {
    0x01, 0x00, 0x01
};

static Optional<std::string> GetVersionFromBody(const std::string &body)
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
        return NullOpt();
    }

    // Read up until the version key is found
    hr = Json::ReadUpToKey(reader, versionKey);
    if (FAILED(hr))
        return NullOpt();

    // Open the value at the version key
    hr = Json::ReadTokenType(reader, Json_String);
    if (FAILED(hr))
        return NullOpt();

    // Read the value at the version key
    hr = XJSONGetTokenValue(reader, value, sizeof(value));
    if (FAILED(hr))
    {
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Couldn't read \"%s\" value from JSON: %X.",
            versionKey.c_str(),
            hr
        );
        return NullOpt();
    }

    XJSONCloseReader(reader);

    return value;
}

static Optional<std::string> GetDownloadUrlFromBody(const std::string &body)
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
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't set the JSON buffers (%X).", hr);
        return NullOpt();
    }

    // Read up until the assets array key is found
    hr = Json::ReadUpToKey(reader, assetsArrayKey);
    if (FAILED(hr))
        return NullOpt();

    // Open the array
    hr = Json::ReadTokenType(reader, Json_BeginArray);
    if (FAILED(hr))
        return NullOpt();

    // Open the first object
    hr = Json::ReadTokenType(reader, Json_BeginObject);
    if (FAILED(hr))
        return NullOpt();

    // Read up until the download URL key is found
    hr = Json::ReadUpToKey(reader, downloadUrlKey);
    if (FAILED(hr))
        return NullOpt();

    // Open the value at the download URL key
    hr = Json::ReadTokenType(reader, Json_String);
    if (FAILED(hr))
        return NullOpt();

    // Read the value at the download URL key
    hr = XJSONGetTokenValue(reader, value, sizeof(value));
    if (FAILED(hr))
    {
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Couldn't read \"%s\" value from JSON (%X).",
            downloadUrlKey.c_str(),
            hr
        );
        return NullOpt();
    }

    XJSONCloseReader(reader);

    return value;
}

struct LatestVersion
{
    std::string Number;
    std::string DownloadUrl;
};

static Optional<LatestVersion> GetLatestVersion(const Http::Client &httpClient)
{
    // Send request to GitHub API
    Optional<Http::Response> response = httpClient.Get("https://api.github.com/repos/ClementDreptin/Hayzen/releases/latest");
    if (!response)
    {
        DebugPrint("Couldn't get the latest version from the GitHub API.");
        return NullOpt();
    }

    // Check HTTP status
    if (response->Status != 200)
    {
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Invalid status code when trying to get the "
            "latest version number. Expected 200 but got %d.",
            response->Status
        );
        return NullOpt();
    }

    // Get the version from the body
    std::string body = response->BodyAsString();
    Optional<std::string> version = GetVersionFromBody(body);
    if (!version)
        return NullOpt();

    // Get the URL of the latest binary
    Optional<std::string> downloadUrl = GetDownloadUrlFromBody(body);
    if (!downloadUrl)
        return NullOpt();

    LatestVersion result;
    result.Number = *version;
    result.DownloadUrl = *downloadUrl;

    return result;
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

static HRESULT WriteBodyToFile(const std::vector<uint8_t> &body)
{
    XASSERT(g_pPlugin != nullptr);

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
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Couldn't open \"%s\" (%X).",
            pluginPath.c_str(),
            status
        );
        return status;
    }

    // Replace the contents of the existing file with the response body
    status = NtWriteFile(
        handle,
        nullptr,
        nullptr,
        nullptr,
        &block,
        const_cast<uint8_t *>(body.data()),
        body.size(),
        nullptr
    );
    if (status != 0)
    {
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Couldn't write to \"%s\" (%X).",
            pluginPath.c_str(),
            status
        );
        NtClose(handle);
        return status;
    }

    NtClose(handle);

    return status;
}

static HRESULT Download(const Http::Client &httpClient, const std::string &url)
{
    // Send request to GitHub API
    Optional<Http::Response> response = httpClient.Get(url);
    if (!response)
    {
        DebugPrint("[Hayzen][AutoUpdater]: Error: Couldn't download the latest binary from GitHub.");
        return E_FAIL;
    }

    // Check HTTP status
    if (response->Status != 200)
    {
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Invalid status code when trying to download. "
            "Expected 200 but got %d.",
            response->Status
        );
        return E_FAIL;
    }

    // Overwrite the old file with the new one
    HRESULT hr = WriteBodyToFile(response->Body);
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
    XASSERT(g_pPlugin != nullptr);

    // Setup the HTTP client to send requests to GitHub
    Http::Client httpClient;
    httpClient.AddECTrustAnchor(s_SectigoECC_DN, sizeof(s_SectigoECC_DN), s_SectigoECC_Q, sizeof(s_SectigoECC_Q), Socket::Curve_secp256r1);
    httpClient.AddRsaTrustAnchor(s_R12_RSA_DN, sizeof(s_R12_RSA_DN), s_R12_RSA_N, sizeof(s_R12_RSA_N), s_R12_RSA_E, sizeof(s_R12_RSA_E));

    // Get information about the latest version available on GitHub
    Optional<LatestVersion> latestVersion = GetLatestVersion(httpClient);
    if (!latestVersion)
        return E_FAIL;

    // If we're up to date, stop here
    std::string currentVersion = g_pPlugin->GetVersion();
    if (currentVersion == latestVersion->Number)
        return S_OK;

    // Ask the user if they want to download the latest version
    bool wantsToDownload = AskToDownload(currentVersion, latestVersion->Number);
    if (!wantsToDownload)
        return S_OK;

    // Download the file and replace the existing one
    HRESULT hr = Download(httpClient, latestVersion->DownloadUrl);
    if (FAILED(hr))
    {
        Xam::XNotify("Couldn't download the latest version.", Xam::XNOTIFYUI_TYPE_AVOID_REVIEW);
        return hr;
    }

    // Let the user know everything succeeded
    DisplaySuccessMessageBox(latestVersion->Number);

    return hr;
}

}
