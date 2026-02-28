#pragma once

#include "Core/Title.h"
#include "Games/SpecOps/AlphaMW2/Structs.h"
#include "Modules/Console.h"

class SpecOpsAlphaMW2Title : public Title
{
public:
    SpecOpsAlphaMW2Title();

    virtual void InitMenu() override;

private:
    Console<SpecOpsAlphaMW2::Game::dvar_t> m_Console;

    static void ClientCommandHook(int clientNum, const char *s);

    virtual void InstallHooks() override;

    virtual void Update() override;

    virtual void Render() override;

    virtual void InitRenderer() override;
};
