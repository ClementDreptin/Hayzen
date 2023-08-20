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

    static void SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer);

    virtual void InitRenderer() override;
};
