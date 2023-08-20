#pragma once

#include "Core/Title.h"

class SpecOpsAlphaMW2Title : public Title
{
public:
    SpecOpsAlphaMW2Title();

    virtual void InitMenu() override;

private:
    static void ClientCommandHook(int clientNum, const char *s);

    virtual void InitRenderer() override;
};
