#pragma once

#include "Core/Title.h"

class SpecOpsMW3Title : public Title
{
public:
    SpecOpsMW3Title();

    virtual void InitMenu() override;

private:
    static void ClientCommandHook(int clientNum, const char *s);

    static void ForceJumpEnabled();

    virtual void InitRenderer() override;
};
