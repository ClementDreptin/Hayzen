#pragma once

#include "Core/Title.h"
#include "Elements/HudElem.h"

#include "Games/MW2/Structs.h"

class MW2Title : public Title
{
public:
    ~MW2Title();

    virtual void Init();

private:
    virtual void CreateStructure();

    static Detour *s_pScr_NotifyDetour;

    static void Scr_NotifyHook(MW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    static Detour *s_pSV_ExecuteClientCommandDetour;

    static void SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer);
};
