#pragma once

typedef BOOL (*DLAUNCHGETOPTVALBYNAME)(const char *optionName, DWORD *pOptionValue);

class AutoUpdater
{
public:
    static HRESULT Init();

    static HRESULT NewVersionAvailable(bool &newVersionAvailable);

private:
    static DLAUNCHGETOPTVALBYNAME s_DlaunchGetOptValByName;
    static Socket s_Socket;
    static std::string s_PluginPath;

    // Get the full path of the Hayzen plugin from the DashLaunch config.
    static std::string GetPluginPath();
};
