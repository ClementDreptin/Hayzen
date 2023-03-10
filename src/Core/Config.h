#pragma once

#include <mINI.h>

class Config
{
public:
    typedef mINI::INIStructure IniConfig;
    typedef mINI::INIFile IniConfigFile;

    Config(const std::string &filePath);

    bool Save();

    bool Load();

private:
    std::string m_FilePath;
    IniConfigFile m_ConfigFile;
    IniConfig m_Config;

    static bool s_HddMounted;

    static void MountHddIfNeeded();
};
