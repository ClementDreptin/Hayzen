#pragma once

#include "Core/Title.h"
#include "Games/AlphaMW2/Structs.h"

// Disable the C4481 warning for the override keyword
#pragma warning(push)
#pragma warning(disable : 4481)

class AlphaMW2Title : public Title
{
public:
    AlphaMW2Title();

    ~AlphaMW2Title();

private:
    static Detour *s_pSV_ExecuteClientCommandDetour;

    static Detour *s_pScr_NotifyDetour;

    static void Scr_NotifyHook(AlphaMW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    static void SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer);

    virtual void InitMenu() override;

    virtual void InitRenderer() override;
};

#pragma warning(pop)
