#include "pch.h"
#include "Games\SpecOps\MW2\MW2.h"

#include "Games\SpecOps\MW2\Functions.h"
#include "Games\SpecOps\MW2\Events.h"

namespace SpecOps
{
namespace MW2
{
    BOOL HasGameBegun = FALSE;
    std::unordered_map<INT, Client> Clients;

    __declspec(naked) VOID SCR_DrawScreenFieldStub(INT refreshedUI)
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

    __declspec(naked) VOID ClientCommandStub(INT clientNum, LPCSTR s)
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

    VOID Init()
    {
        Xam::XNotify("Hayzen - MW2 Spec Ops Detected");

        Sleep(200);

        Memory::HookFunctionStart((DWORD*)0x821354B0, (LPDWORD)SCR_DrawScreenFieldStub, (DWORD)SCR_DrawScreenFieldHook);
        Memory::HookFunctionStart((DWORD*)0x821EFFD0, (LPDWORD)ClientCommandStub, (DWORD)ClientCommandHook);
    }

    VOID SetupGame(INT clientNum)
    {
        Verify(clientNum);
        HasGameBegun = TRUE;
    }

    BOOL Verify(INT clientNum)
    {
        if (clientNum < 0 || clientNum > 17)
            return FALSE;

        if (Clients.find(clientNum) != Clients.end() && Clients[clientNum].IsInitialized())
            return FALSE;

        Cbuf_AddText(0, "set loc_warnings 0");
        Cbuf_AddText(0, "set loc_warningsUI 0");

        Clients[clientNum] = Client(clientNum);

        return TRUE;
    }

    VOID SafeReset()
    {
        if (Clients.size() != 0)
            Clients.clear();

        if (HasGameBegun)
            HasGameBegun = FALSE;
    }

    VOID SCR_DrawScreenFieldHook(INT refreshedUI)
    {
        SCR_DrawScreenFieldStub(refreshedUI);

        for (size_t i = 0; i < Clients.size(); i++)
            Clients[i].GetMenu().Update();
    }

    VOID ClientCommandHook(INT clientNum, LPCSTR s)
    {
        ClientCommandStub(clientNum, s);

        if (!strcmp(s, GAME_START) && !HasGameBegun)
            SetupGame(clientNum);
        
        if (HasGameBegun && Clients.find(clientNum) != Clients.end())
            Clients[clientNum].GetMenu().OnEvent(s);
    }
}
}