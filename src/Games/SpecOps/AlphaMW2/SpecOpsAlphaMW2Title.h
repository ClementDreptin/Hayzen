#pragma once

#include "Core/Title.h"

// Disable the C4481 warning for the override keyword
#pragma warning(push)
#pragma warning(disable : 4481)

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

#pragma warning(pop)
