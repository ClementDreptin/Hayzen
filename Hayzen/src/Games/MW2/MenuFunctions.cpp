#include "pch.h"
#include "Games\MW2\MenuFunctions.h"

using namespace MW2GameFunctions;


//--------------------------------------------------------------------------------------
// Name: ToggleGodMode()
// Desc: Toggle God Mode.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::ToggleGodMode(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    CONST INT GOD_MODE_ON = 4097;
    CONST INT GOD_MODE_OFF = 4096;

    if (GetEntity(iClientNum)->flags == GOD_MODE_OFF)
    {
        GetEntity(iClientNum)->flags = GOD_MODE_ON;
        iPrintLn(iClientNum, "God Mode ^2On");
    }
    else
    {
        GetEntity(iClientNum)->flags = GOD_MODE_OFF;
        iPrintLn(iClientNum, "God Mode ^1Off");
    }
}


//--------------------------------------------------------------------------------------
// Name: ToggleFallDamage()
// Desc: Toggle fall damage.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::ToggleFallDamage(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    DWORD dwAddress = 0x82019C48;

    if (Memory::Read<FLOAT>(dwAddress) == 128.0f)
    {
        Memory::Write<FLOAT>(dwAddress, 9999.0f);
        iPrintLn(iClientNum, "Fall Damage ^2Off");
    }
    else
    {
        Memory::Write<FLOAT>(dwAddress, 128.0f);
        iPrintLn(iClientNum, "Fall Damage ^1On");
    }
}


//--------------------------------------------------------------------------------------
// Name: ToggleAmmo()
// Desc: Toggle unlimited ammo.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::ToggleAmmo(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    DWORD dwAddress = 0x820E1724;
    DWORD dwDefaultValue = 0x7D1D4850;
    DWORD dwModifiedValue = 0x7D284B78;

    if (Memory::Read<DWORD>(dwAddress) == dwDefaultValue)
    {
        Memory::Write<DWORD>(dwAddress, dwModifiedValue);
        iPrintLn(iClientNum, "Unlimited Ammo ^2On");
    }
    else
    {
        Memory::Write<DWORD>(dwAddress, dwDefaultValue);
        iPrintLn(iClientNum, "Unlimited Ammo ^1Off");
    }
}


//--------------------------------------------------------------------------------------
// Name: ToggleElevators()
// Desc: Toggle elvators.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::ToggleElevators(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    DWORD dwBranchAddress = 0x820D8360;
    WORD wDefaultValue = 0x419A;
    WORD wModifiedValue = 0x4800;

    if (Memory::Read<WORD>(dwBranchAddress) == wDefaultValue)
    {
        Memory::Write<WORD>(dwBranchAddress, wModifiedValue);
        iPrintLn(iClientNum, "Elevators ^2On");
    }
    else
    {
        Memory::Write<WORD>(dwBranchAddress, wDefaultValue);
        iPrintLn(iClientNum, "Elevators ^1Off");
    }
}


//--------------------------------------------------------------------------------------
// Name: SpawnCP()
// Desc: Spawn a care package.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::SpawnCP(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    gentity_s* currentMapBrushModel = GetCurrentMapBrushModel();

    // Return early if the map is not supported
    if (!currentMapBrushModel)
    {
        iPrintLn(iClientNum, "^1You cannot spawn a Care Package on this map!");
        return;
    }

    // Get the current player's position
    FLOAT distance = 150.0f;
    vec3 origin = GetPlayerState(iClientNum)->origin;
    FLOAT viewY = GetPlayerState(iClientNum)->viewAngles.y;

    // Spawn an entity 150 units in front of the player and oriented towards
    // where they are looking at
    gentity_s* entity = G_Spawn();
    entity->r.currentOrigin = Math::ToFront(origin, viewY, distance);
    entity->r.currentAngles.y = viewY;

    // Apply the care package mesh to the entity
    G_SetModel(entity, "com_plasticcase_friendly");
    SP_script_model(entity);
    SV_UnlinkEntity(entity);
    entity->r.bmodel = 4;
    entity->state.index = currentMapBrushModel->state.index;

    // Make the care package solid
    INT contents = entity->r.contents;
    SV_SetBrushModel(entity);
    contents |= entity->r.contents;
    entity->r.contents = contents;

    // Register the entity for the scene 
    SV_LinkEntity(entity);
}


//--------------------------------------------------------------------------------------
// Name: Knockback()
// Desc: Open a keyboard to allow the user to change the knockback value.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::Knockback(Menu*)
{
    // Get the value from the user via the virtual keyboard
    std::string strValue = Xam::ShowKeyboard("Knockback", "Recommended value: 30000", "30000", 6, VKBD_LATIN_NUMERIC);

    // If the user did not enter anything, set the value to its default value
    if (strValue == "")
        strValue = "1000";

    // Set the g_knockback value to what the user entered
    SetClientDvar(-1, "g_knockback", strValue);

    iPrintLn(-1, "Knockback set to ^2" + strValue);
}


//--------------------------------------------------------------------------------------
// Name: ToggleSaveLoadBinds()
// Desc: Toggle save and load binds.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::ToggleSaveLoadBinds(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();

    if (!pMenu->BindsEnabled())
        iPrintLn(iClientNum, "Press [{+frag}] to ^2Save^7 and [{+smoke}] to ^2Load");
    else
        iPrintLn(iClientNum, "Save and Load binds ^1Off");

    pMenu->ToggleBinds();
}


//--------------------------------------------------------------------------------------
// Name: SavePosition()
// Desc: Save the current player's position.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::SavePosition(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();

    pMenu->SetSavedPos(GetPlayerState(iClientNum)->origin);
    pMenu->SetSavedAngles(GetPlayerState(iClientNum)->viewAngles);

    iPrintLn(iClientNum, "Position ^2Saved");
}


//--------------------------------------------------------------------------------------
// Name: LoadPosition()
// Desc: Load the previously saved player's position.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::LoadPosition(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();
    CONST vec3& SavedPos = pMenu->GetSavedPos();
    CONST vec3& SavedAngles = pMenu->GetSavedAngles();

    // Make sure the player previously saved their position
    if (SavedPos == vec3(0.0f, 0.0f, 0.0f) || SavedAngles == vec3(0.0f, 0.0f, 0.0f))
    {
        iPrintLn(iClientNum, "^1Save a position first!");
        return;
    }

    TeleportPlayer(GetEntity(iClientNum), (PFLOAT)&SavedPos, (PFLOAT)&SavedAngles);
}


//--------------------------------------------------------------------------------------
// Name: ToggleUFO()
// Desc: Toggle UFO.
//--------------------------------------------------------------------------------------
VOID MW2MenuFunctions::ToggleUFO(Menu* pMenu)
{
    INT iClientNum = pMenu->GetClientNum();

    if (GetGClient(iClientNum)->mFlags != 2)
    {
        GetGClient(iClientNum)->mFlags = 2;
        iPrintLn(iClientNum, "Ufo ^2On");
    }
    else
    {
        GetGClient(iClientNum)->mFlags = 0;
        iPrintLn(iClientNum, "Ufo ^1Off");
    }
}
