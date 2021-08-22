#include "pch.h"
#include "Games\SpecOps\MW3\MW3.h"

#include "Games\SpecOps\MW3\Events.h"

namespace SpecOps
{
namespace MW3
{
    BOOL HasGameBegun = FALSE;
    std::unordered_map<INT, Client> Clients;

    __declspec(naked) VOID DrawMenuDebugTextStub(INT dc)
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

    __declspec(naked) VOID PlayerCmd_AllowJumpStub()
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
            li r3, 3
        }
    }

    VOID Init()
    {
        Xam::XNotify("Hayzen - MW3 Spec Ops Detected");

        Sleep(200);

        Memory::HookFunctionStart((DWORD*)0x822E0488, (LPDWORD)DrawMenuDebugTextStub, (DWORD)DrawMenuDebugTextHook);
        Memory::HookFunctionStart((DWORD*)0x821FEFB0, (LPDWORD)ClientCommandStub, (DWORD)ClientCommandHook);
        Memory::HookFunctionStart((DWORD*)0x821FA680, (LPDWORD)PlayerCmd_AllowJumpStub, (DWORD)PlayerCmd_AllowJumpHook);
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

    VOID DrawMenuDebugTextHook(INT dc)
    {
        DrawMenuDebugTextStub(dc);

        for (size_t i = 0; i < Clients.size(); i++)
            Clients[i].GetMenu().Update();
    }

    VOID ClientCommandHook(INT clientNum, LPCSTR s)
    {
        ClientCommandStub(clientNum, s);

        if (!strcmp(s, GAME_START))
            SetupGame(clientNum);
        
        if (HasGameBegun && Clients.find(clientNum) != Clients.end())
            Clients[clientNum].GetMenu().OnEvent(s);
    }

    VOID PlayerCmd_AllowJumpHook()
    {
        /**
         * Making the PlayerCmd_AllowJump function not do anything so that you can jump in
         * missions where you normally can't. This is a bad practice and may have side effects.
         */
        return;
    }
}
}