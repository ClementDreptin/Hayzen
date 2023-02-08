#pragma once

class Dashboard
{
public:
    static void Init();

private:
    static HRESULT LookForUpdates();
};
