#include "pch.h"
#include "Games\MW3\Functions.h"

#include "Games\MW3\Structs.h"

namespace MW3
{
    LPCSTR (*SL_ConvertToString)(UINT stringValue) = (LPCSTR(*)(UINT))0x822B5120;

    VOID (*SV)(INT clientNum, INT type, LPCSTR text) = (VOID(*)(INT, INT, LPCSTR))0x822C9340;

    LPCSTR (*Dvar_GetString)(LPCSTR dvarName) = (LPCSTR(*)(LPCSTR))0x8232E488;

    game_hudelem_s* (*HudElem_Alloc)(INT clientNum, INT teamNum) = (game_hudelem_s*(*)(INT, INT))0x8223D260;

    VOID (*HudElem_Free)(game_hudelem_s* hud) = (VOID(*)(game_hudelem_s*))0x8223D390;

    INT (*G_MaterialIndex)(LPCSTR name) = (INT(*)(LPCSTR))0x8226E180;

    INT (*G_LocalizedStringIndex)(LPCSTR string) = (INT(*)(LPCSTR))0x8226E0A0;

    clientState_s* (*GetClientState)(INT clientNum) = (clientState_s*(*)(INT))0x82244130;

    playerState_s* (*GetPlayerState)(INT clientNum) = (playerState_s*(*)(INT))0x82244148;

    bool (*Session_IsHost)(DWORD sessionDataPtr, INT clientNum) = (bool(*)(DWORD, INT))0x823BFE78;

    VOID (*SP_script_model)(gentity_s* mSelf) = (VOID(*)(gentity_s*))0x82268138;

    gentity_s*(*G_Spawn)() = (gentity_s*(*)())0x8226F378;

    VOID (*G_SetModel)(gentity_s* ent, LPCSTR modelName) = (VOID(*)(gentity_s*, LPCSTR))0x8226EAA0;

    VOID (*SV_LinkEntity)(gentity_s* gEnt) = (VOID(*)(gentity_s*))0x822D6E78;

    VOID (*SV_UnlinkEntity)(gentity_s* gEnt) = (VOID(*)(gentity_s*))0x822D6D90;

    VOID (*SV_SetBrushModel)(gentity_s* ent) = (VOID(*)(gentity_s*))0x822C95C8;

    VOID (*TeleportPlayer)(gentity_s* player, PFLOAT origin, PFLOAT angles) = (VOID(*)(gentity_s*, PFLOAT, PFLOAT))0x82246048;

    gclient_s* GetGClient(INT clientNum)
    {
        return (gclient_s*)(0x82F99580 + sizeof(gclient_s) * clientNum);
    }

    gentity_s* GetEntity(INT entNum)
    {
        return (gentity_s*)(0x82DCCC80 + sizeof(gentity_s) * entNum);
    }

    VOID SetClientDvar(INT clientNum, CONST std::string& dvar, CONST std::string& value)
    {
        SV(clientNum, 0, Formatter::Format("q %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
    }

    VOID iPrintLn(INT clientNum, CONST std::string& text)
    {
        SV(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
    }

    bool IsHost(INT clientNum)
    {
        return Session_IsHost(0x83BC0148, clientNum);
    }

    gentity_s* GetCurrentMapBrushModel()
    {
        std::string mapName = Dvar_GetString("ui_mapname");

        if (mapName == "mp_seatown")
            return (gentity_s*)0x82DD1280;
        if (mapName == "mp_mogadishu")
            return (gentity_s*)0x82E08A00;
        if (mapName == "mp_exchange")
            return (gentity_s*)0x82E14580;
        if (mapName == "mp_radar")
            return (gentity_s*)0x82DD3A80;
        if (mapName == "mp_terminal_cls")
            return (gentity_s*)0x82DF9C80;
        else
            return (gentity_s*)0x82DD1500;
    }
}