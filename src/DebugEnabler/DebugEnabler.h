#pragma once

class DebugEnabler
{
public:
    static HRESULT Enable();

    static void Disable();

private:
    static Detour *s_pXexpResolveImageImportsDetour;

    static int XexpResolveImageImportsHook(void *pExportBaseAddress, XEX_IMPORT_DESCRIPTOR *pImportDesc, uint32_t flags);
};
