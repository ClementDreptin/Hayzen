#pragma once

#include "Core/Title.h"
#include "Games/WaW/Structs.h"

class WaWTitle : public Title
{
public:
    WaWTitle();

    virtual void InitMenu() override;

private:
    static void Scr_Notify(WaW::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    static void G_ShutdownGameHook(bool freeScripts);

    virtual void InitRenderer() override;

    void ApplyPatches();

    static char *Scr_AddSourceBufferHook(uint32_t scriptInstance, const char *filename, const char *extFilename, const char *codePos, bool archive);

    static std::string s_PatchedGameObjectsGscMainFunction;
};
