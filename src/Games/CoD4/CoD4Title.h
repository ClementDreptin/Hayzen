#pragma once

#include "Core/Title.h"
#include "Games/CoD4/Structs.h"

class CoD4Title : public Title
{
public:
    CoD4Title();

    virtual void InitMenu() override;

private:
    static void Scr_NotifyNumHook(int entNum, uint32_t classNum, uint32_t stringValue, uint32_t paramCount);

    static void G_ShutdownGameHook(bool freeScripts);

    virtual void InitRenderer() override;

    void ApplyPatches();

    static char *Scr_AddSourceBufferHook(const char *filename, const char *extFilename, const char *codePos, bool archive);

    static std::string s_PatchedGameObjectsGscMainFunction;
};
