#pragma once

// Class to run things when getting on the Xbox 360 dashboard
class Dashboard
{
public:
    // Print a notification and look for updates.
    static void Init();

private:
    // Check if a new version is available, if so, download it if the user wants to.
    static HRESULT LookForUpdates();
};
