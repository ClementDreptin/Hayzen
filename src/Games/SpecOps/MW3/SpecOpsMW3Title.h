#pragma once

#include "Core/Title.h"
#include "Elements/HudElem.h"

class SpecOpsMW3Title : public Title
{
public:
    ~SpecOpsMW3Title();

    virtual void Init();

private:
    static bool s_HasJumped;

    virtual void CreateStructure();

    static Detour *s_pClientCommandDetour;

    static void ClientCommandHook(int clientNum, const char *s);

    static Detour *s_pPlayerCmd_AllowJumpDetour;

    static void PlayerCmd_AllowJumpHook();
};
