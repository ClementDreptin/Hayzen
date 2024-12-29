#pragma once

#include <mINI.h>

class Config
{
public:
    typedef mINI::INIStructure IniConfig;
    typedef mINI::INIFile IniConfigFile;

    Config(const std::string &filePath);

    HRESULT Save();

    HRESULT Load();

private:
    IniConfigFile m_ConfigFile;
    IniConfig m_Config;
};
