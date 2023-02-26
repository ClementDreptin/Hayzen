#pragma once

#include "Core/Title.h"
#include "Games/MW2/Structs.h"

class MW2Title : public Title
{
public:
    MW2Title();

    ~MW2Title();

    virtual void InitMenu() override;

private:
    static Detour *s_pScr_NotifyDetour;

    static Detour *s_pSV_ExecuteClientCommandDetour;

    static void Scr_NotifyHook(MW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    static void SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer);

    virtual void InitRenderer() override;
};
