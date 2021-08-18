#include "pch.h"
#include "Games\MW2\MW2.h"

#include "Games\MW2\Events.h"
#include "Games\MW2\Functions.h"

namespace MW2
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
        Xam::XNotify("Hayzen - MW2 Multiplayer Detected");

        Sleep(200);

        // NOP cheat protection
        Memory::Write<int>(0x8216906C, 0x60000000);
        Memory::Write<int>(0x821690E4, 0x60000000);

        Memory::HookFunctionStart((DWORD*)0x82209710, (DWORD*)Scr_NotifyStub, (DWORD)Scr_NotifyHook);
        Memory::HookFunctionStart((DWORD*)0x82253140, (DWORD*)SV_ExecuteClientCommandStub, (DWORD)SV_ExecuteClientCommandHook);
    }

    void SetupGame(int clientNum)
    {
        if (IsHost(clientNum))
        {
            Verify(clientNum);
            HasGameBegun = true;
        }
    }

    void ResetGame(int clientNum, bool resetBot)
    {
        Clients.erase(clientNum);

        if (IsHost(clientNum))
        {
            if (resetBot)
                Menu::FreeBot();

            HasGameBegun = false;
        }
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
        {
            Menu::FreeBot();
            HasGameBegun = false;
        }
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
                ResetGame(clientNum, false);

            SetupGame(clientNum);
        }
    }

    void SV_ExecuteClientCommandHook(int client, const char* s, int clientOK, int fromOldServer)
    {
        SV_ExecuteClientCommandStub(client, s, clientOK, fromOldServer);

        int clientNum = (client - Memory::Read<int>(0x83623B98)) / 0x97F80;

        if (HasGameBegun && Clients.find(clientNum) != Clients.end())
            Clients[clientNum].GetMenu().OnEvent(s);

        if (!strcmp(s, "disconnect") && Clients.find(clientNum) != Clients.end())
            ResetGame(clientNum);
    }
}