#include "pch.h"
#include "Games\SpecOps\MW2\MW2.h"

#include "Games\SpecOps\MW2\Functions.h"
#include "Games\SpecOps\MW2\Events.h"

namespace SpecOps
{
namespace MW2
{
    bool HasGameBegun = false;
    std::unordered_map<int, Client> Clients;

    __declspec(naked) void SCR_DrawScreenFieldStub(int refreshedUI)
    {
        __asm
        {
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            li r3, 1
        }
    }

    __declspec(naked) void ClientCommandStub(int clientNum, const char* s)
    {
        __asm
        {
            nop
            nop
            nop
            nop
            nop
            nop
            nop
            li r3, 2
        }
    }

    void Init()
    {
        Xam::XNotify("Hayzen - MW2 Spec Ops Detected");

        Sleep(200);

        Memory::HookFunctionStart((DWORD*)0x821354B0, (DWORD*)SCR_DrawScreenFieldStub, (DWORD)SCR_DrawScreenFieldHook);
        Memory::HookFunctionStart((DWORD*)0x821EFFD0, (DWORD*)ClientCommandStub, (DWORD)ClientCommandHook);
    }

    void SetupGame(int clientNum)
    {
        Verify(clientNum);
        HasGameBegun = true;
    }

    bool Verify(int clientNum)
    {
        if (clientNum < 0 || clientNum > 17)
            return false;

        if (Clients.find(clientNum) != Clients.end() && Clients[clientNum].IsInitialized())
            return false;

        Cbuf_AddText(0, "set loc_warnings 0");
        Cbuf_AddText(0, "set loc_warningsUI 0");

        Clients[clientNum] = Client(clientNum);

        return true;
    }

    void SafeReset()
    {
        if (Clients.size() != 0)
            Clients.clear();

        if (HasGameBegun)
            HasGameBegun = false;
    }

    void SCR_DrawScreenFieldHook(int refreshedUI)
    {
        SCR_DrawScreenFieldStub(refreshedUI);

        for (size_t i = 0; i < Clients.size(); i++)
            Clients[i].GetMenu().Update();
    }

    void ClientCommandHook(int clientNum, const char* s)
    {
        ClientCommandStub(clientNum, s);

        if (!strcmp(s, GAME_START) && !HasGameBegun)
            SetupGame(clientNum);
        
        if (HasGameBegun && Clients.find(clientNum) != Clients.end())
            Clients[clientNum].GetMenu().OnEvent(s);
    }
}
}