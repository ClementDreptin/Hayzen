#pragma once

#include "Core/Title.h"
#include "Games/MW2/Structs.h"

class MW2Title : public Title
{
public:
    MW2Title();

    virtual void InitMenu() override;

private:
    static void Scr_NotifyHook(MW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    static void SV_ExecuteClientCommandHook(MW2::Game::client_t *client, const char *s, int clientOK, int fromOldServer);

    virtual void InitRenderer() override;
};
