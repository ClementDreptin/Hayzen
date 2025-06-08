#pragma once

#include "Core/Title.h"
#include "Games/AlphaGhosts/Structs.h"

class AlphaGhostsTitle : public Title
{
public:
    AlphaGhostsTitle();

    virtual void InitMenu() override;

private:
    static void Scr_NotifyHook(AlphaGhosts::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    static void SV_ExecuteClientCommandHook(AlphaGhosts::Game::client_t *client, const char *s, int fromOldServer);

    virtual void InitRenderer() override;

    void WaitUntilReady();
};
