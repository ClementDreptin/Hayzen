#include "pch.h"
#include "Games\Dashboard\AutoUpdater.h"

// Version set to an incorrect value for now just for testing
#define HAYZEN_VERSION "6.0.1"

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

    // Connect to the server
    hr = s_Socket.Connect();
    if (FAILED(hr))
        return E_FAIL;

    return hr;
}

HRESULT AutoUpdater::NewVersionAvailable(bool &newVersionAvailable)
{
    std::string request = "HEAD /" HAYZEN_VERSION " HTTP/1.1\r\nHost: hayzen-updater.herokuapp.com\r\nConnection: keep-alive\r\n\r\n";

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
            return std::string(pluginFullPath);
    }

    // If nothing is found, just return an empty string
    return std::string("");
}
