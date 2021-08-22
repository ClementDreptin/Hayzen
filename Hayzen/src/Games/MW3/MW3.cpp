#include "pch.h"
#include "Games\MW3\MW3.h"

#include "Games\MW3\Events.h"
#include "Games\MW3\Functions.h"

namespace MW3
{
    BOOL HasGameBegun = FALSE;
    std::unordered_map<INT, Client> Clients;

    __declspec(naked) VOID Scr_NotifyStub(gentity_s* entity, USHORT stringValue, UINT paramCount)
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

    __declspec(naked) VOID SV_ExecuteClientCommandStub(INT client, LPCSTR s, INT clientOK, INT fromOldServer)
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
        Xam::XNotify("Hayzen - MW3 Multiplayer Detected");

        Sleep(200);

        // NOP cheat protection
        Memory::Write<INT>(0x821ABA24, 0x60000000);
        Memory::Write<INT>(0x821ABA9C, 0x60000000);

        Memory::HookFunctionStart((LPDWORD)0x8226AF98, (LPDWORD)Scr_NotifyStub, (DWORD)Scr_NotifyHook);
        Memory::HookFunctionStart((LPDWORD)0x822C78A0, (LPDWORD)SV_ExecuteClientCommandStub, (DWORD)SV_ExecuteClientCommandHook);
    }

    VOID SetupGame(INT clientNum)
    {
        if (IsHost(clientNum))
        {
            Verify(clientNum);
            HasGameBegun = TRUE;
        }
    }

    VOID ResetGame(INT clientNum)
    {
        Clients.erase(clientNum);

        if (IsHost(clientNum))
            HasGameBegun = FALSE;
    }

    BOOL Verify(INT clientNum)
    {
        if (clientNum < 0 || clientNum > 17)
            return FALSE;

        if (Clients.find(clientNum) != Clients.end() && Clients[clientNum].IsInitialized())
            return FALSE;

        SetClientDvar(clientNum, "loc_warnings", "0");
        SetClientDvar(clientNum, "loc_warningsUI", "0");

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

    VOID Scr_NotifyHook(gentity_s* entity, USHORT stringValue, UINT paramCount)
    {
        Scr_NotifyStub(entity, stringValue, paramCount);

        INT clientNum = entity->state.number;

        LPCSTR notify = SL_ConvertToString(stringValue);

        if (!strcmp(notify, GAME_START))
        {
            // "begin" can happen multiple times a game in round-based gamemodes so we need to reset the menu
            // and recreate it at the beggining of each round if we want the menu to work after the first round
            if (HasGameBegun)
                ResetGame(clientNum);

            SetupGame(clientNum);
        }
    }

    VOID SV_ExecuteClientCommandHook(INT client, LPCSTR s, INT clientOK, INT fromOldServer)
    {
        SV_ExecuteClientCommandStub(client, s, clientOK, fromOldServer);

        INT clientNum = (client - Memory::Read<INT>(0x836C6310)) / 0x68B80;

        if (HasGameBegun && Clients.find(clientNum) != Clients.end())
            Clients[clientNum].GetMenu().OnEvent(s);

        if (!strcmp(s, "matchdatadone") && Clients.find(clientNum) != Clients.end())
            ResetGame(clientNum);
    }
}