#include "pch.h"
#include "Games\MW3\MW3.h"

#include "Games\MW3\Events.h"
#include "Games\MW3\Functions.h"

namespace MW3
{
    bool HasGameBegun = false;
    std::unordered_map<int, Client> Clients;

    __declspec(naked) void Scr_NotifyStub(gentity_s* entity, unsigned short stringValue, unsigned int paramCount)
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

    __declspec(naked) void SV_ExecuteClientCommandStub(int client, const char* s, int clientOK, int fromOldServer)
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
        Xam::XNotify("Hayzen - MW3 Multiplayer Detected");

        Sleep(200);

        // NOP cheat protection
        Memory::Write<int>(0x821ABA24, 0x60000000);
        Memory::Write<int>(0x821ABA9C, 0x60000000);

        Memory::HookFunctionStart((DWORD*)0x8226AF98, (DWORD*)Scr_NotifyStub, (DWORD)Scr_NotifyHook);
        Memory::HookFunctionStart((DWORD*)0x822C78A0, (DWORD*)SV_ExecuteClientCommandStub, (DWORD)SV_ExecuteClientCommandHook);
    }

    void SetupGame(int clientNum)
    {
        if (IsHost(clientNum))
        {
            Verify(clientNum);
            HasGameBegun = true;
        }
    }

    void ResetGame(int clientNum)
    {
        Clients.erase(clientNum);

        if (IsHost(clientNum))
            HasGameBegun = false;
    }

    bool Verify(int clientNum)
    {
        if (clientNum < 0 || clientNum > 17)
            return false;

        if (Clients.find(clientNum) != Clients.end() && Clients[clientNum].IsInitialized())
            return false;

        SetClientDvar(clientNum, "loc_warnings", "0");
        SetClientDvar(clientNum, "loc_warningsUI", "0");

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

    void Scr_NotifyHook(gentity_s* entity, unsigned short stringValue, unsigned int paramCount)
    {
        Scr_NotifyStub(entity, stringValue, paramCount);

        int clientNum = entity->state.number;

        const char* notify = SL_ConvertToString(stringValue);

        if (!strcmp(notify, GAME_START))
        {
            // "begin" can happen multiple times a game in round-based gamemodes so we need to reset the menu
            // and recreate it at the beggining of each round if we want the menu to work after the first round
            if (HasGameBegun)
                ResetGame(clientNum);

            SetupGame(clientNum);
        }
    }

    void SV_ExecuteClientCommandHook(int client, const char* s, int clientOK, int fromOldServer)
    {
        SV_ExecuteClientCommandStub(client, s, clientOK, fromOldServer);

        int clientNum = (client - Memory::Read<int>(0x836C6310)) / 0x68B80;

        if (HasGameBegun && Clients.find(clientNum) != Clients.end())
            Clients[clientNum].GetMenu().OnEvent(s);

        if (!strcmp(s, "matchdatadone") && Clients.find(clientNum) != Clients.end())
            ResetGame(clientNum);
    }
}