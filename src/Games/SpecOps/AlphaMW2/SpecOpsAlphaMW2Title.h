#pragma once

#include "Core/Title.h"
#include "Modules/Console.h"

class SpecOpsAlphaMW2Title : public Title
{
public:
    SpecOpsAlphaMW2Title();

    virtual void InitMenu() override;

private:
    Console m_Console;

    static void ClientCommandHook(int clientNum, const char *s);

    virtual void Update() override;

    virtual void Render() override;

    virtual void InitRenderer() override;
};
