#pragma once

#include "Core/Title.h"

class SpecOpsMW2Title : public Title
{
public:
    SpecOpsMW2Title();

    virtual void InitMenu() override;

private:
    static void ClientCommandHook(int clientNum, const char *s);

    virtual void InitRenderer() override;
};
