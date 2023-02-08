#pragma once

#include "Core/Title.h"
#include "Elements/HudElem.h"

class SpecOpsMW2Title : public Title
{
public:
    ~SpecOpsMW2Title();

    virtual void Init();

private:
    static bool s_HasJumped;

    virtual void CreateStructure();

    static Detour *s_pClientCommandDetour;

    static void ClientCommandHook(int clientNum, const char *s);
};
