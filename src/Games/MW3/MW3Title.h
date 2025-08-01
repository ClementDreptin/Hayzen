#pragma once

#include "Core/Title.h"
#include "Games/MW3/Structs.h"

class MW3Title : public Title
{
public:
    MW3Title();

    virtual void InitMenu() override;

private:
    static void Scr_NotifyHook(MW3::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    static void SV_ExecuteClientCommandHook(MW3::Game::client_t *client, const char *s, int clientOK, int fromOldServer);

    static void SV_DropClientHook(MW3::Game::client_t *client, const char *reason, bool tellThem);

    virtual void InitRenderer() override;
};
