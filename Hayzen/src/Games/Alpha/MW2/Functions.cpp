#include "pch.h"
#include "Games\Alpha\MW2\Functions.h"

#include "Games\Alpha\MW2\Structs.h"

namespace Alpha
{
namespace MW2
{
    const char* (*SL_ConvertToString)(unsigned int stringValue) = (const char*(*)(unsigned int))0x8229A730;

    void (*SV)(int clientNum, int type, const char* text) = (void(*)(int, int, const char*))0x822B6140;

    void (*R_AddCmdDrawText)(const char* text, int maxChars, Font_s* font, float x, float y, float xScale, float yScale, float rotation, const float* color, int style) =
        (void(*)(const char* , int, Font_s* , float, float, float, float, float, const float*, int))0x823BB4D8;

    void (*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float* color, void* material) =
        (void(*)(float, float, float, float, float, float, float, float, const float*, void*))0x823BAC18;

    Font_s* (*R_RegisterFont)(const char* font, int imageTrack) = (Font_s*(*)(const char*, int))0x823B6D58;

    void* (*Material_RegisterHandle)(const char* name, int imageTrack) = (void*(*)(const char *, int))0x823B6928;

    void (*Cbuf_AddText)(int localClientNum, const char* text) = (void(*)(int, const char*))0x8226F590;

    bool (*Dvar_GetBool)(const char* dvarName) = (bool(*)(const char*))0x82303B00;

    float (*Dvar_GetFloat)(const char *dvarName) = (float(*)(const char*))0x82303BD0;

    const char* (*Dvar_GetString)(const char* dvarName) = (const char*(*)(const char*))0x82303CC0;

    playerState_s* (*GetPlayerState)(int clientNum) = (playerState_s*(*)(int))0x8222C108;

    void (*SP_script_model)(gentity_s* mSelf) = (void(*)(gentity_s*))0x82250A20;

    gentity_s* (*G_Spawn)() = (gentity_s*(*)())0x82258508;

    void (*G_SetModel)(gentity_s* ent, const char* modelName) = (void(*)(gentity_s*, const char*))0x82257B90;

    void (*SV_LinkEntity)(gentity_s* gEnt) = (void(*)(gentity_s*))0x822C1598;

    void (*SV_UnlinkEntity)(gentity_s* gEnt) = (void(*)(gentity_s*))0x822C14B0;

    void (*SV_SetBrushModel)(gentity_s* ent) = (void(*)(gentity_s*))0x822B63B8;

    gentity_s* (*SV_AddTestClient)() = (gentity_s*(*)())0x822B5F00;

    void (*SV_ExecuteClientCommand)(int client, const char* s, int clientOK, int fromOldServer) = (void(*)(int, const char*, int, int))0x822B4700;

    void (*TeleportPlayer)(gentity_s* player, float* origin, float* angles) = (void(*)(gentity_s*, float*, float*))0x8222E5A0;

    gentity_s* GetEntity(int entNum)
    {
        return (gentity_s*)(0x82D47D80 + sizeof(gentity_s) * entNum);
    }

    void SetClientDvar(int clientNum, const std::string& dvar, const std::string& value)
    {
        SV(clientNum, 0, Formatter::Format("v %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
    }

    void iPrintLn(int clientNum, const std::string& text)
    {
        SV(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
    }

    gentity_s* GetCurrentMapBrushModel()
    {
        std::string mapName = Dvar_GetString("ui_mapname");

        if (mapName == "mp_afghan")
            return (gentity_s*)0x82D60880;
        if (mapName == "mp_checkpoint")
            return (gentity_s*)0x82D76B80;
        if (mapName == "mp_derail")
            return (gentity_s*)0x82D95800;
        if (mapName == "mp_estate")
            return (gentity_s*)0x82D5B600;
        if (mapName == "mp_favela")
            return (gentity_s*)0x82D9A300;
        if (mapName == "mp_highrise")
            return (gentity_s*)0x82DC4380;
        if (mapName == "mp_invasion")
            return (gentity_s*)0x82D7E100;
        if (mapName == "mp_quarry")
            return (gentity_s*)0x82D70F00;
        if (mapName == "mp_rundown")
            return (gentity_s*)0x82D93780;
        if (mapName == "mp_rust")
            return (gentity_s*)0x82D5A980;
        if (mapName == "mp_boneyard")
            return (gentity_s*)0x82D64E80;
        if (mapName == "mp_nightshift")
            return (gentity_s*)0x82D58180;
        if (mapName == "mp_subbase")
            return (gentity_s*)0x82D7CF80;
        if (mapName == "mp_terminal")
            return (gentity_s*)0x82D61A00;
        if (mapName == "mp_underpass")
            return (gentity_s*)0x82D52000;
        else
            return nullptr;
    }
}
}