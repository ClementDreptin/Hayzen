#include "pch.h"
#include "Games\Alpha\MW2\MW2.h"

#include "Games\Alpha\MW2\Client.h"
#include "Games\Alpha\MW2\Events.h"
#include "Games\Alpha\MW2\Functions.h"

namespace Alpha
{
namespace MW2
{
    BOOL HasGameBegun = FALSE;
    std::unordered_map<INT, Client> Clients;

    VOID SCR_DrawScreenFieldStub(CONST INT localClientNum, INT refreshedUI);
    VOID SCR_DrawScreenFieldHook(CONST INT localClientNum, INT refreshedUI);

    VOID Scr_NotifyStub(gentity_s* entity, USHORT stringValue, UINT paramCount);
    VOID Scr_NotifyHook(gentity_s* entity, USHORT stringValue, UINT paramCount);

    VOID SV_ExecuteClientCommandStub(INT client, LPCSTR s, INT clientOK, INT fromOldServer);
    VOID SV_ExecuteClientCommandHook(INT client, LPCSTR s, INT clientOK, INT fromOldServer);

    VOID Init()
    {
        Xam::XNotify("Hayzen - MW2 Alpha Multiplayer Detected");

        Sleep(200);

        Memory::HookFunctionStart((LPDWORD)0x8218B5F0, (LPDWORD)SCR_DrawScreenFieldStub, (DWORD)SCR_DrawScreenFieldHook);
        Memory::HookFunctionStart((LPDWORD)0x822539C0, (LPDWORD)Scr_NotifyStub, (DWORD)Scr_NotifyHook);
        Memory::HookFunctionStart((LPDWORD)0x822B4700, (LPDWORD)SV_ExecuteClientCommandStub, (DWORD)SV_ExecuteClientCommandHook);
    }

    BOOL Verify(INT clientNum)
    {
        if (clientNum < 0 || clientNum > 17)
            return FALSE;

        if (Clients.find(clientNum) != Clients.end() && Clients[clientNum].IsInitialized())
            return FALSE;

        SetClientDvar(clientNum, "loc_warnings", "0");
        SetClientDvar(clientNum, "loc_warningsAsErrors", "0");

        Clients[clientNum] = Client(clientNum);

        return TRUE;
    }

    VOID SafeReset()
    {
        if (Clients.size() != 0)
            Clients.clear();

        if (HasGameBegun)
        {
            Menu::FreeBot();
            HasGameBegun = FALSE;
        }
    }

    VOID SetupGame(INT clientNum)
    {
        Verify(clientNum);
        HasGameBegun = TRUE;
    }

    VOID ResetGame(INT clientNum, BOOL resetBot = TRUE)
    {
        Clients.erase(clientNum);

        if (resetBot)
            Menu::FreeBot();

        HasGameBegun = FALSE;
    }

    __declspec(naked) VOID SCR_DrawScreenFieldStub(CONST INT localClientNum, INT refreshedUI)
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
            li r3, 2
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
            li r3, 3
        }
    }

    VOID SCR_DrawScreenFieldHook(CONST INT localClientNum, INT refreshedUI)
    {
        SCR_DrawScreenFieldStub(localClientNum, refreshedUI);

        for (size_t i = 0; i < Clients.size(); i++)
            Clients[i].GetMenu().Update();
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
                ResetGame(clientNum, FALSE);

            SetupGame(clientNum);
        }
    }

    VOID SV_ExecuteClientCommandHook(INT client, LPCSTR s, INT clientOK, INT fromOldServer)
    {
        SV_ExecuteClientCommandStub(client, s, clientOK, fromOldServer);

        INT clientNum = (client - Memory::Read<INT>(0x83577D98)) / 0x97F80;

        if (HasGameBegun && Clients.find(clientNum) != Clients.end())
            Clients[clientNum].GetMenu().OnEvent(s);

        if (!strcmp(s, "matchdatadone") && Clients.find(clientNum) != Clients.end())
            ResetGame(clientNum);
    }
}
}