#include "pch.h"
#include "Games\Dashboard\Dashboard.h"

#include "Games\Dashboard\AutoUpdater.h"

void Dashboard::Init()
{
    Xam::XNotify("Hayzen - Dashboard Detected");

    // Just for now for debug
    Sleep(5000);

    HRESULT hr = S_OK;

    // Initiliaze the auto updater
    hr = AutoUpdater::Init();
    if (FAILED(hr))
        return;

    // Check if there is a new update available
    bool newVersionAvailable = false;
    hr = AutoUpdater::NewVersionAvailable(newVersionAvailable);
    if (FAILED(hr))
        return;

    // If we already have the latest version, just stop here
    if (!newVersionAvailable)
        return;

    // Display a message box asking the user if they want to download the new version
    const wchar_t *buttonLabels[] = { L"Yes", L"No" };
    uint32_t buttonPressedIndex = 0;

    uint32_t result = Xam::ShowMessageBox(
        L"New version available",
        L"A new version of Hayzen is available, would you like to download it?",
        buttonLabels,
        ARRAYSIZE(buttonLabels),
        &buttonPressedIndex,
        XMB_ALERTICON
    );

    // If the user cancelled the message box, return early
    if (result != ERROR_SUCCESS)
        return;

    // If the user pressed anything else than "Yes", exit early
    if (buttonPressedIndex != 0)
        return;

    // TODO:
    // - Send the request to download the latest version of the plugin
    // - Write the file at the right place
    // - Ask the user to reboot to load the new version of the plugin
}
