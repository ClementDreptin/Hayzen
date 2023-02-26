#pragma once

#include "Core/Title.h"

class SpecOpsMW3Title : public Title
{
public:
    SpecOpsMW3Title();

    ~SpecOpsMW3Title();

    virtual void InitMenu() override;

private:
    static Detour *s_pClientCommandDetour;

    static Detour *s_pPlayerCmd_AllowJumpDetour;

    static void ClientCommandHook(int clientNum, const char *s);

    static void PlayerCmd_AllowJumpHook();

    virtual void InitRenderer() override;
};
