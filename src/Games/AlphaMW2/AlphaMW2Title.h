#pragma once

#include "Core/Title.h"
#include "Elements/HudElem.h"

#include "Games/AlphaMW2/Structs.h"

class AlphaMW2Title : public Title
{
public:
    ~AlphaMW2Title();

    virtual void Init();

private:
    virtual void CreateStructure();

    static Detour *s_pScr_NotifyDetour;

    static void Scr_NotifyHook(AlphaMW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    static Detour *s_pSV_ExecuteClientCommandDetour;

    static void SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer);
};
