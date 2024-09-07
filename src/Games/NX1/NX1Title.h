#pragma once

#include "Core/Title.h"
#include "Games/NX1/Structs.h"

class NX1Title : public Title
{
public:
    NX1Title();

    virtual void InitMenu() override;

private:
    static void Scr_NotifyHook(NX1::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount);

    static void SV_ExecuteClientCommandHook(NX1::Game::client_t *client, const char *s, int clientOK, int fromOldServer);

    virtual void InitRenderer() override;

    void WaitUntilReady();
};
