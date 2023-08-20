#pragma once

#include "Core/Title.h"
#include "Games/AlphaMW2/Structs.h"

class AlphaMW2Title : public Title
{
public:
    AlphaMW2Title();

    ~AlphaMW2Title();

    virtual void InitMenu() override;

private:
    static Detour *s_pScr_NotifyDetour;

    static Detour *s_pSV_ExecuteClientCommandDetour;

    static void Scr_NotifyHook(AlphaMW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    static void SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer);

    virtual void InitRenderer() override;
};
