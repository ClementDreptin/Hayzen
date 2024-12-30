#pragma once

#include <mINI.h>

class Config
{
public:
    Config(const std::string &filePath);

    HRESULT SaveToDisk();

    HRESULT LoadFromDisk();

    void Reset();

    bool AllowDebugBuilds;
    bool DisplayControlsTexts;
    float CarePackageDistance;
    float CarePackageHeight;
    float CarePackageAngle;
    float Y;
    D3DCOLOR Color;
    D3DCOLOR BackgroundColor;
    D3DCOLOR TextColor;
    float Padding;
    float Gap;
    float BorderWidth;
    float X;
    float Width;
    float Height;

private:
    mINI::INIFile m_File;
    mINI::INIStructure m_Structure;
};

extern Config g_Config;
