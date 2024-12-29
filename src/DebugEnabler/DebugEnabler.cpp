// Original version written by Xx jAmes t xX and released here
// https://www.obscuregamers.com/threads/release-17559-hvp-xex-allows-debug-builds-to-run-on-jtag-rgh-without-rgl-or-xdk.1799/post-28644.html
//
// This version is very simplified and might not work for all games but it works for
// the games supported by this plugin

#include "pch.h"
#include "DebugEnabler/DebugEnabler.h"

#include "Core/Plugin.h"

namespace DebugEnabler
{

static Detour *s_pXexpResolveImageImportsDetour = nullptr;

static int XexpResolveImageImportsHook(void *pExportBaseAddress, XEX_IMPORT_DESCRIPTOR *pImportDesc, uint32_t flags)
{
    XASSERT(s_pXexpResolveImageImportsDetour != nullptr);

    // Get a pointer to the first string in the name table. The name table is right after
    // the import descriptor and contains moduleCount strings
    char *currentModuleName = reinterpret_cast<char *>(reinterpret_cast<uintptr_t>(pImportDesc) + sizeof(XEX_IMPORT_DESCRIPTOR));

    for (size_t i = 0; i < pImportDesc->ModuleCount; i++)
    {
        size_t currentModuleNameSize = strnlen_s(currentModuleName, MAX_PATH);
        size_t spaceTakenByCurrentModuleName = Math::RoundToNextMultipleOf<4>(currentModuleNameSize);

        // Look for xbdm.xex
        if (!strnicmp(currentModuleName, "xbdm.xex", currentModuleNameSize))
        {
            // Make sure we have enough space
            size_t requiredSpace = g_pPlugin->GetName().size() + 1; // + 1 for the null character
            if (requiredSpace > spaceTakenByCurrentModuleName)
            {
                DebugPrint(
                    "[Hayzen][DebugEnabler]: Error: Not enough space to store the plugin name (%s),"
                    "required: %d, available: %d.",
                    g_pPlugin->GetName().c_str(),
                    requiredSpace,
                    spaceTakenByCurrentModuleName
                );
                break;
            }

            // Replace xbdm.xex with the plugin name
            ZeroMemory(currentModuleName, currentModuleNameSize);
            strncpy_s(
                currentModuleName,
                requiredSpace,
                g_pPlugin->GetName().c_str(),
                _TRUNCATE
            );

            break;
        }

        // Make the pointer point to the next string in the table
        currentModuleName += spaceTakenByCurrentModuleName;
    }

    return s_pXexpResolveImageImportsDetour->GetOriginal<decltype(&XexpResolveImageImportsHook)>()(pExportBaseAddress, pImportDesc, flags);
}

HRESULT Enable()
{
    XASSERT(s_pXexpResolveImageImportsDetour == nullptr);

    Hypervisor::Poke<uint32_t>(0x800001040002AA58, 0x60000000);

    s_pXexpResolveImageImportsDetour = new Detour(0x80079D48, XexpResolveImageImportsHook);

    return s_pXexpResolveImageImportsDetour->Install();
}

void Disable()
{
    XASSERT(s_pXexpResolveImageImportsDetour != nullptr);

    Hypervisor::Poke<uint32_t>(0x800001040002AA58, 0x48000274);

    delete s_pXexpResolveImageImportsDetour;
    s_pXexpResolveImageImportsDetour = nullptr;
}

}
