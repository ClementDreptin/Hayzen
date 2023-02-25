#pragma once

#include "Core/Title.h"
#include "Games/MW3/Structs.h"

// Disable the C4481 warning for the override keyword
#pragma warning(push)
#pragma warning(disable : 4481)

class MW3Title : public Title
{
public:
    MW3Title();

    ~MW3Title();

    virtual void InitMenu() override;

private:
    static Detour *s_pScr_NotifyDetour;

    static Detour *s_pSV_ExecuteClientCommandDetour;

    static void Scr_NotifyHook(MW3::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    static void SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer);

    virtual void InitRenderer() override;
};

#pragma warning(pop)
