#pragma once

class AutoUpdater
{
public:
    static HRESULT Init();

    static HRESULT CheckForNewVersion(bool &newVersionAvailable);

    static HRESULT DownloadLatestVersion();

private:
    static Socket s_Socket;
    static std::string s_PluginPath;

    static std::string GetPluginPath();
};
