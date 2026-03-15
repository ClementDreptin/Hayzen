#pragma once

#include "Core/Title.h"
#include "Games/CoD4/Structs.h"
#include "Modules/Console.h"

class CoD4Title : public Title
{
public:
    CoD4Title();

    virtual void InitMenu() override;

private:
    Console<CoD4::Game::dvar_t> m_Console;

    static std::string s_PatchedGameObjectsGscMainFunction;

    static void Scr_NotifyNumHook(int entNum, uint32_t classNum, uint32_t stringValue, uint32_t paramCount);

    static void G_ShutdownGameHook(bool freeScripts);

    static char *Scr_AddSourceBufferHook(const char *filename, const char *extFilename, const char *codePos, bool archive);

    void ApplyPatches();

    virtual void InstallHooks() override;

    virtual void Update() override;

    virtual void Render() override;

    virtual void InitRenderer() override;
};
