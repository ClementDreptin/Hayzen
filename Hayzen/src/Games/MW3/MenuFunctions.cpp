#include "pch.h"
#include "Games\MW3\MenuFunctions.h"

using namespace MW3GameFunctions;


void MW3MenuFunctions::ToggleGodMode(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();
    const int GOD_MODE_ON = 4097;
    const int GOD_MODE_OFF = 4096;

    if (GetEntity(iClientNum)->flags == GOD_MODE_OFF)
    {
        GetEntity(iClientNum)->flags = GOD_MODE_ON;
        pMenu->SetFeedbackText("God Mode ^2On");
    }
    else
    {
        GetEntity(iClientNum)->flags = GOD_MODE_OFF;
        pMenu->SetFeedbackText("God Mode ^1Off");
    }
}

void MW3MenuFunctions::ToggleFallDamage(Menu *pMenu)
{
    DWORD dwAddress = 0x82000C04;

    if (Memory::Read<float>(dwAddress) == 128.0f)
    {
        Memory::Write<float>(dwAddress, 9999.0f);
        pMenu->SetFeedbackText("Fall Damage ^2Off");
    }
    else
    {
        Memory::Write<float>(dwAddress, 128.0f);
        pMenu->SetFeedbackText("Fall Damage ^1On");
    }
}

void MW3MenuFunctions::ToggleAmmo(Menu *pMenu)
{
    DWORD dwAddress = 0x820F63E4;
    DWORD dwDefaultValue = 0x7D3D5050;
    DWORD dwModifiedValue = 0x7D495378;

    if (Memory::Read<DWORD>(dwAddress) == dwDefaultValue)
    {
        Memory::Write<DWORD>(dwAddress, dwModifiedValue);
        pMenu->SetFeedbackText("Unlimited Ammo ^2On");
    }
    else
    {
        Memory::Write<DWORD>(dwAddress, dwDefaultValue);
        pMenu->SetFeedbackText("Unlimited Ammo ^1Off");
    }
}

void MW3MenuFunctions::SpawnCP(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();
    gentity_s *currentMapBrushModel = GetCurrentMapBrushModel();

    // Return early if the map is not supported
    if (!currentMapBrushModel)
    {
        pMenu->SetFeedbackText("^1You cannot spawn a Care Package on this map!");
        return;
    }

    // Get the player's current position
    float distance = 150.0f;
    vec3 origin = GetPlayerState(iClientNum)->origin;
    float viewY = GetPlayerState(iClientNum)->viewAngles.y;

    // Spawn an entity 150 units in front of the player and oriented towards
    // where they are looking at
    gentity_s *entity = G_Spawn();
    entity->r.currentOrigin = Math::ToFront(origin, viewY, distance);
    entity->r.currentAngles.y = viewY;

    // Apply the care package mesh to the entity
    G_SetModel(entity, "com_plasticcase_friendly");
    SP_script_model(entity);
    SV_UnlinkEntity(entity);
    entity->r.bmodel = 4;
    entity->state.index = currentMapBrushModel->state.index;

    // Make the care package solid
    int contents = entity->r.contents;
    SV_SetBrushModel(entity);
    contents |= entity->r.contents;
    entity->r.contents = contents;

    // Register the entity for the scene 
    SV_LinkEntity(entity);
}

void MW3MenuFunctions::ToggleSaveLoadBinds(Menu *pMenu)
{    
    if (!pMenu->BindsEnabled())
        pMenu->SetFeedbackText("Press " CHAR_RB " to ^2Save^7 and " CHAR_LB " to ^2Load");
    else
        pMenu->SetFeedbackText("Save and Load binds ^1Off");

    pMenu->ToggleBinds();
}

void MW3MenuFunctions::SavePosition(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    pMenu->SetSavedPos(GetPlayerState(iClientNum)->origin);
    pMenu->SetSavedAngles(GetPlayerState(iClientNum)->viewAngles);

    pMenu->SetFeedbackText("Position ^2Saved");
}

void MW3MenuFunctions::LoadPosition(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();
    const vec3 &SavedPos = pMenu->GetSavedPos();
    const vec3 &SavedAngles = pMenu->GetSavedAngles();

    // Make sure the player previously saved their position
    if (SavedPos == vec3(0.0f, 0.0f, 0.0f) || SavedAngles == vec3(0.0f, 0.0f, 0.0f))
    {
        pMenu->SetFeedbackText("^1Save a position first!");
        return;
    }

    TeleportPlayer(GetEntity(iClientNum), reinterpret_cast<const float *>(&SavedPos), reinterpret_cast<const float *>(&SavedAngles));
}

void MW3MenuFunctions::ToggleUFO(Menu *pMenu)
{
    int iClientNum = pMenu->GetClientNum();

    if (GetGClient(iClientNum)->mFlags != 2)
    {
        GetGClient(iClientNum)->mFlags = 2;
        pMenu->SetFeedbackText("Ufo ^2On");
    }
    else
    {
        GetGClient(iClientNum)->mFlags = 0;
        pMenu->SetFeedbackText("Ufo ^1Off");
    }
}
