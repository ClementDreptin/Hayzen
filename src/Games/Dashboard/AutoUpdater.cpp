#include "pch.h"
#include "Games\Dashboard\AutoUpdater.h"

#define HAYZEN_VERSION "6.0.3"

Socket AutoUpdater::s_Socket;
std::string AutoUpdater::s_PluginPath;

HRESULT AutoUpdater::Init()
{
    HRESULT hr = S_OK;

    // Initialize the DashLaunch functions
    hr = DashLaunch::Init();
    if (FAILED(hr))
        return E_FAIL;

    // Get the full path to the Hayzen plugin
    s_PluginPath = GetPluginPath();
    if (s_PluginPath == "")
        return E_FAIL;

    // Create the socket that will be used to communicate with the server
    s_Socket = Socket("3.219.96.23", 80);

    return hr;
}

HRESULT AutoUpdater::CheckForNewVersion(bool &newVersionAvailable)
{
    // Connect to the server
    HRESULT hr = s_Socket.Connect();
    if (FAILED(hr))
        return E_FAIL;

    std::string request = "HEAD /" HAYZEN_VERSION " HTTP/1.1\r\nHost: hayzen-updater.herokuapp.com\r\n\r\n";

    // Send the request to the server
    int bytesSent = s_Socket.Send(request.c_str(), request.size());
    if (bytesSent != static_cast<int>(request.size()))
    {
        s_Socket.Disconnect();
        return E_FAIL;
    }

    // Wait a little bit for the server to process the request and send the response
    Sleep(700);

    // Get the response from the server
    char buffer[4096] = { 0 };
    std::string response = "";
    int bytesReceived = 0;
    while ((bytesReceived = s_Socket.Receive(buffer, sizeof(buffer))) > 0)
    {
        response.append(buffer, bytesReceived);

        Sleep(100);
    }

    // Disconnect from the server
    s_Socket.Disconnect();

    // Get the status code
    size_t pos = response.find("\r\n");
    std::string firstResponseLine = response.substr(0, pos);

    int statusCode = 0;
    sscanf_s(firstResponseLine.c_str(), "HTTP/1.1 %d", &statusCode);

    // 304 Not Modified means the version in the HAYZEN_VERSION macro is the latest one
    if (statusCode == 304)
    {
        newVersionAvailable = false;
        return S_OK;
    }

    // 200 OK means the version in the HAYZEN_VERSION macro is not the latest one
    if (statusCode == 200)
    {
        newVersionAvailable = true;
        return S_OK;
    }

    // Something went wrong if the status code is not 200 nor 304
    return E_FAIL;
}

HRESULT AutoUpdater::DownloadLatestVersion()
{
    // Connect to the server
    HRESULT hr = s_Socket.Connect();
    if (FAILED(hr))
        return E_FAIL;

    std::string request = "GET /latest HTTP/1.1\r\nHost: hayzen-updater.herokuapp.com\r\n\r\n";

    // Send the request to the server
    int bytesSent = s_Socket.Send(request.c_str(), request.size());
    if (bytesSent != static_cast<int>(request.size()))
    {
        s_Socket.Disconnect();
        return E_FAIL;
    }

    // Wait a little bit for the server to process the request and send the response
    Sleep(700);

    // Get the response from the server
    char buffer[4096] = { 0 };
    std::string response = "";
    int bytesReceived = 0;
    while ((bytesReceived = s_Socket.Receive(buffer, sizeof(buffer))) > 0)
    {
        response.append(buffer, bytesReceived);

        Sleep(100);
    }

    // Disconnect from the server
    s_Socket.Disconnect();

    // Make sure we received something
    if (response.size() == 0)
        return E_FAIL;

    // Isolate the Content-Length header
    size_t startOfContentLengthLine = response.find("Content-Length");
    if (startOfContentLengthLine == std::string::npos)
        return E_FAIL;

    size_t endOfContentLengthLine = response.find("\r\n", startOfContentLengthLine);
    std::string contentLengthLine = response.substr(startOfContentLengthLine, endOfContentLengthLine - startOfContentLengthLine);

    // Get the content length
    int contentLength = 0;
    sscanf_s(contentLengthLine.c_str(), "Content-Length: %d", &contentLength);

    // Get the content
    std::string delimiter = "\r\n\r\n";
    size_t startOfContent = response.find(delimiter) + delimiter.size();
    std::string content = response.substr(startOfContent, contentLength);

    // Open the file
    std::ofstream file;
    file.open(s_PluginPath, std::ios::out | std::ios::binary);
    if (!file.is_open())
        return E_FAIL;

    // Write the content to the file
    file << content;
    file.close();

    return hr;
}

std::string AutoUpdater::GetPluginPath()
{
    // Dashlaunch allows a maximum of 5 plugins
    const size_t maxNumberOfPlugins = 5;

    for (size_t i = 0; i <= maxNumberOfPlugins; i++)
    {
        char *pluginFullPath = nullptr;
        char pluginKey[8] = { 0 };

        // Create the plugin key, plugin1, plugin2 and so on
        _snprintf_s(pluginKey, _TRUNCATE, "plugin%d", i);

        // Get the path set for the current plugin key
        BOOL res = DashLaunch::GetOptionValueByName(pluginKey, reinterpret_cast<uint32_t *>(&pluginFullPath));

        // Some plugin keys might be missing in launch.ini so just continue to the next plugin if the key is not found
        if (res == FALSE)
            continue;

        // Extract the file name from the full plugin path
        char pluginFileName[MAX_PATH] = { 0 };
        _splitpath_s(
            pluginFullPath,
            nullptr, 0,
            nullptr, 0,
            pluginFileName, sizeof(pluginFileName),
            nullptr, 0
        );

        // Check if the current plugin file name is Hayzen, return the current plugin full path
        if (!strcmp(pluginFileName, "Hayzen"))
        {
            // The full path returned from DashLaunch does not contain the drive name so we need to add it
            std::string result = "hdd:";
            result += pluginFullPath;

            return result;
        }
    }

    // If nothing is found, just return an empty string
    return std::string("");
}
