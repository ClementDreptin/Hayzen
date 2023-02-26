#pragma once

#include "Core/Title.h"

class SpecOpsAlphaMW2Title : public Title
{
public:
    SpecOpsAlphaMW2Title();

    ~SpecOpsAlphaMW2Title();

    virtual void InitMenu() override;

private:
    static Detour *s_pClientCommandDetour;

    static void ClientCommandHook(int clientNum, const char *s);

    virtual void InitRenderer() override;
};
