#include "pch.h"
#include "Games\Dashboard\Dashboard.h"

#include "Games\Dashboard\AutoUpdater.h"

void Dashboard::Init()
{
    Xam::XNotify("Hayzen - Dashboard Detected");

    // Just for now for debug
    Sleep(5000);

    LookForUpdates();
}

HRESULT Dashboard::LookForUpdates()
{
    HRESULT hr = S_OK;

    // Initiliaze the auto updater
    hr = AutoUpdater::Init();
    if (FAILED(hr))
        return E_FAIL;

    // Check if there is a new update available
    bool newVersionAvailable = false;
    hr = AutoUpdater::CheckForNewVersion(newVersionAvailable);
    if (FAILED(hr))
        return E_FAIL;

    // If we already have the latest version, just stop here
    if (!newVersionAvailable)
        return S_OK;

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
        return S_OK;

    // If the user pressed anything else than "Yes", return early
    if (buttonPressedIndex != 0)
        return S_OK;

    // Download the latest version and write it to disk
    hr = AutoUpdater::DownloadLatestVersion();
    if (FAILED(hr))
    {
        const wchar_t *okButton[] = { L"OK" };

        Xam::ShowMessageBox(
            L"Download error",
            L"An error occured while downloading the latest version of Hayzen.",
            okButton,
            ARRAYSIZE(okButton),
            nullptr,
            XMB_ERRORICON
        );

        return E_FAIL;
    }

    // Display a success message box to the user and ask them if they want to reboot to load the latest version
    result = Xam::ShowMessageBox(
        L"Download complete",
        L"The latest version of Hayzen was successfully downloaded.\nThe changes won't be applied until the next reboot, would you like to reboot now?",
        buttonLabels,
        ARRAYSIZE(buttonLabels),
        &buttonPressedIndex,
        XMB_ALERTICON,
        1
    );

    // If the user cancelled the message box, return early
    if (result != ERROR_SUCCESS)
        return S_OK;

    // If the user pressed anything else than "Yes", return early
    if (buttonPressedIndex != 0)
        return S_OK;

    // Reboot
    // TODO: Call HalReturnToFirmware with either HalRebootRoutine or HalFatalErrorRebootRoutine (cf. _kernelext.h from xkelib)

    return hr;
}
