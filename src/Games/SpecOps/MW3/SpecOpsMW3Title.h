#pragma once

#include "Core/Title.h"
#include "Games/SpecOps/MW3/Structs.h"
#include "Modules/Console.h"

class SpecOpsMW3Title : public Title
{
public:
    SpecOpsMW3Title();

    virtual void InitMenu() override;

private:
    Console<SpecOpsMW3::Game::dvar_t> m_Console;

    static void ClientCommandHook(int clientNum, const char *s);

    static void ForceJumpEnabled();

    virtual void InstallHooks() override;

    virtual void Update() override;

    virtual void Render() override;

    virtual void InitRenderer() override;

    virtual void WaitUntilReady() override;
};
