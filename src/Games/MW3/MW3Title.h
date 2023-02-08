#pragma once

#include "Core/Title.h"
#include "Elements/HudElem.h"

#include "Games/MW3/Structs.h"

class MW3Title : public Title
{
public:
    ~MW3Title();

    virtual void Init();

private:
    virtual void CreateStructure();

    static Detour *s_pScr_NotifyDetour;

    static void Scr_NotifyHook(MW3::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    static Detour *s_pSV_ExecuteClientCommandDetour;

    static void SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer);
};
