#pragma once

#include "Core/Title.h"
#include "Elements/HudElem.h"

class SpecOpsAlphaMW2Title : public Title
{
public:
    ~SpecOpsAlphaMW2Title();

    virtual void Init();

private:
    static bool s_HasJumped;

    virtual void CreateStructure();

    static Detour *s_pClientCommandDetour;

    static void ClientCommandHook(int clientNum, const char *s);
};
