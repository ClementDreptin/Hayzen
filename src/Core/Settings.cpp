#include "pch.h"
#include "Core/Settings.h"

#include "UI/Renderer.h"

namespace Settings
{

bool AllowDebugBuilds = false;

bool DisplayControlsTexts = true;

float Y = 150.0f;

D3DCOLOR Color = D3DCOLOR_XRGB(210, 20, 20);

const D3DCOLOR BackgroundColor = D3DCOLOR_XRGB(25, 25, 25);

const D3DCOLOR TextColor = D3DCOLOR_XRGB(255, 255, 255);

const float Padding = 5.0f;

const float Gap = 2.0f;

const float BorderWidth = 2.5f;

// The following values are calculated dynamically when initializing the menu

float X = 0.0f;

float Width = 0.0f;

float Height = 0.0f;

void Reset()
{
    // Double cast to round to the closest integer value
    X = static_cast<float>(static_cast<uint32_t>(Renderer::DisplayWidth - Settings::Width - 10.0f));
    Y = 150.0f;
    Color = D3DCOLOR_XRGB(210, 20, 20);
    DisplayControlsTexts = true;
}

}
