#pragma once

// Automatic updating system.
class AutoUpdater
{
public:
    // Get the full path to the Hayzen plugin and create the socket.
    static HRESULT Init();

    // Send a request to the server to check if a new version is available.
    static HRESULT CheckForNewVersion(bool &newVersionAvailable);

private:
    static Socket s_Socket;
    static std::string s_PluginPath;

    // Get the full path of the Hayzen plugin from the DashLaunch config.
    static std::string GetPluginPath();
};
