#include "pch.h"
#include "Games\MW2\MW2.h"

#include "Games\MW2\MenuFunctions.h"


LPCSTR (*SL_ConvertToString)(UINT stringValue) = (LPCSTR(*)(UINT))0x82241898;
UINT (*FindEntityId)(INT entnum, UINT classnum) = (UINT(*)(INT, UINT))0x82244438;
bool (*Session_IsHost)(DWORD sessionDataPtr, INT clientNum) = (bool(*)(DWORD, INT))0x82320138;

BOOL IsHost(INT iClientNum)
{
    return Session_IsHost(0x83AC3DB0, iClientNum);
}


//--------------------------------------------------------------------------------------
// Name: Init()
// Desc: Set the draw function pointers and the function hooks.
//--------------------------------------------------------------------------------------
VOID MW2::Init()
{
    Xam::XNotify("Hayzen - MW2 Multiplayer Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // NOP cheat protection
    Memory::Write<INT>(0x8216906C, 0x60000000);
    Memory::Write<INT>(0x821690E4, 0x60000000);

    // Set the draw function addresses
    m_dwDrawTextFnAddr = 0x82350278;
    m_dwDrawRectangleFnAddr = 0x821384D8;
    m_dwRegisterFontFnAddr = 0x8234DCB0;
    m_dwRegisterMaterialFnAddr = 0x8234E510;

    // Set the draw function pointers with the addresses above
    Game::Init();

    // Create the structure of the menu
    CreateStructure();

    // Set up the function hooks
    Memory::HookFunctionStart((LPDWORD)0x8214BEB8, (LPDWORD)SCR_DrawScreenFieldStub, (DWORD)SCR_DrawScreenFieldHook);
    Memory::HookFunctionStart((LPDWORD)0x82209710, (LPDWORD)Scr_NotifyStub, (DWORD)Scr_NotifyHook);
    Memory::HookFunctionStart((LPDWORD)0x82253140, (LPDWORD)SV_ExecuteClientCommandStub, (DWORD)SV_ExecuteClientCommandHook);
}


//--------------------------------------------------------------------------------------
// Name: CreateStructure()
// Desc: Create the structure of the menu and save it a static member.
//--------------------------------------------------------------------------------------
VOID MW2::CreateStructure()
{
    s_Structure["Cod Jumper"] = std::vector<Option>();
    s_Structure["Cod Jumper"].emplace_back(Option("Option 1", 0, MW2MenuFunctions::Option1Callback));
    s_Structure["Cod Jumper"].emplace_back(Option("Option 2", 1, MW2MenuFunctions::Option2Callback));
}


//--------------------------------------------------------------------------------------
// Name: Scr_NotifyHook()
// Desc: Initialize the menu when the game starts.
//--------------------------------------------------------------------------------------
VOID MW2::Scr_NotifyHook(gentity_s* entity, USHORT stringValue, UINT paramCount)
{
    // Call the original Scr_Notify function
    Scr_NotifyStub(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    INT iClientNum = entity->state.number;
    if (!IsHost(iClientNum))
        return;

    // Get the string representing the event
    LPCSTR szNotify = SL_ConvertToString(stringValue);
   
    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure it's not already initialized
    if (!strcmp(szNotify, "begin") && !s_Menu.IsInitialized())
        s_Menu.Init(iClientNum, &s_Structure);
}


//--------------------------------------------------------------------------------------
// Name: SV_ExecuteClientCommandHook()
// Desc: Stop the menu when the game ends.
//--------------------------------------------------------------------------------------
VOID MW2::SV_ExecuteClientCommandHook(INT client, LPCSTR s, INT clientOK, INT fromOldServer)
{
    // Call the original Scr_Notify SV_ExecuteClientCommand
    SV_ExecuteClientCommandStub(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    INT iClientNum = (client - Memory::Read<INT>(0x83623B98)) / 0x97F80;
    if (!IsHost(iClientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "disconnect"))
        s_Menu.Stop();
}

//--------------------------------------------------------------------------------------
// Name: Scr_NotifyStub()
// Desc: Stub to hold the original code of Scr_Notify.
//--------------------------------------------------------------------------------------
VOID __declspec(naked) MW2::Scr_NotifyStub(gentity_s* entity, USHORT stringValue, UINT paramCount)
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


//--------------------------------------------------------------------------------------
// Name: SV_ExecuteClientCommandStub()
// Desc: Stub to hold the original code of SV_ExecuteClientCommand.
//--------------------------------------------------------------------------------------
VOID __declspec(naked) MW2::SV_ExecuteClientCommandStub(INT client, LPCSTR s, INT clientOK, INT fromOldServer)
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


/* #include "Games\MW2\Client.h"
#include "Games\MW2\Events.h"
#include "Games\MW2\Functions.h"

namespace MW2
{
    BOOL HasGameBegun = FALSE;
    std::unordered_map<INT, Client> Clients;

    VOID Scr_NotifyStub(gentity_s* entity, USHORT stringValue, UINT paramCount);
    VOID Scr_NotifyHook(gentity_s* entity, USHORT stringValue, UINT paramCount);

    VOID SV_ExecuteClientCommandStub(INT client, LPCSTR s, INT clientOK, INT fromOldServer);
    VOID SV_ExecuteClientCommandHook(INT client, LPCSTR s, INT clientOK, INT fromOldServer);

    VOID Init()
    {
        Xam::XNotify("Hayzen - MW2 Multiplayer Detected");

        Sleep(200);

        // NOP cheat protection
        Memory::Write<INT>(0x8216906C, 0x60000000);
        Memory::Write<INT>(0x821690E4, 0x60000000);

        Memory::HookFunctionStart((LPDWORD)0x82209710, (LPDWORD)Scr_NotifyStub, (DWORD)Scr_NotifyHook);
        Memory::HookFunctionStart((LPDWORD)0x82253140, (LPDWORD)SV_ExecuteClientCommandStub, (DWORD)SV_ExecuteClientCommandHook);
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
        {
            Menu::FreeBot();
            HasGameBegun = FALSE;
        }
    }

    VOID SetupGame(INT clientNum)
    {
        if (IsHost(clientNum))
        {
            Verify(clientNum);
            HasGameBegun = TRUE;
        }
    }

    VOID ResetGame(INT clientNum, BOOL resetBot = TRUE)
    {
        Clients.erase(clientNum);

        if (IsHost(clientNum))
        {
            if (resetBot)
                Menu::FreeBot();

            HasGameBegun = FALSE;
        }
    }
} */
