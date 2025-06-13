#pragma once

// MW3 TU24 structs used by the game functions.
namespace MW3
{
namespace Game
{

enum WeaponAnimNumber
{
    WEAP_FORCE_IDLE = 0x1,
    WEAP_RELOAD = 0xD,
    WEAP_RELOAD_START = 0xF,
};

enum weaponstate_t
{
    WEAPON_READY = 0x0,
    WEAPON_FIRING = 0x6,
    WEAPON_RECHAMBERING = 0x7,
};

struct PlayerActiveWeaponState
{
    int weapAnim;
    int weaponTime;
    int weaponDelay;
    int weaponRestrictKickTime;
    int weaponState;
    char padding1[0x8];
};

static_assert(sizeof(PlayerActiveWeaponState) == 0x1C, "size of PlayerActiveWeaponState different than 0x1C");

struct PlayerEquippedWeaponState
{
    char padding1[0x1];
    bool dualWielding;
    char padding2[0xA];
};

static_assert(sizeof(PlayerEquippedWeaponState) == 0xC, "size of PlayerEquippedWeaponState different than 0xC");

struct PlayerWeaponCommonState
{
    char padding1[0xC];
    int weapon;
    int weapFlags;
    char padding2[0x164];
};

static_assert(sizeof(PlayerWeaponCommonState) == 0x178, "size of PlayerWeaponCommonState different than 0x178");

struct playerState_s
{
    char padding1[0xC];
    uint32_t pm_flags;
    char padding2[0xC];
    vec3 origin;
    char padding3[0x130];
    vec3 viewAngles;
    char padding4[0xD8];
    PlayerActiveWeaponState weapState[2];
    char padding5[0x3C];
    PlayerEquippedWeaponState weapEquippedData[15];
    PlayerWeaponCommonState weapCommon;
    char padding6[0x2E24];
};

static_assert(sizeof(playerState_s) == 0x3300, "size of playerState_s different than 0x3300");

struct usercmd_s
{
    char padding1[0x4];
    uint32_t buttons;
    char padding2[0x24];
};

static_assert(sizeof(usercmd_s) == 0x2C, "size of usercmd_s different than 0x2C");

struct pmove_t
{
    playerState_s *ps;
    usercmd_s cmd;
    char padding1[0x2C];
    uint32_t tracemask;
    char padding2[0xD8];
};

static_assert(sizeof(pmove_t) == 0x138, "size of pmove_t different than 0x138");

struct pml_t
{
    char padding1[0x80];
    uint32_t holdrand;
};

static_assert(sizeof(pml_t) == 0x84, "size of pml_t different than 0x84");

struct gclient_s
{
    playerState_s ps;
    char padding1[0x2FF];
    uint8_t mFlags;
    char padding2[0x380];
};

static_assert(sizeof(gclient_s) == 0x3980, "size of gclient_s different than 0x3980");

struct entityState_s
{
    int number;
    char padding1[0x88];
    int index;
    char padding2[0x6C];
};

static_assert(sizeof(entityState_s) == 0xFC, "size of entityState_s different than 0xFC");

struct entityShared_t
{
    int clientMask;
    bool linked;
    char bmodel;
    char svFlags;
    bool inuse;
    vec3 mins;
    vec3 maxs;
    int contents;
    vec3 absmin;
    vec3 absmax;
    vec3 currentOrigin;
    vec3 currentAngles;
    int ownerNum;
    int eventTime;
};

static_assert(sizeof(entityShared_t) == 0x5C, "size of entityShared_t different than 0x5C");

struct gentity_s
{
    entityState_s state;
    entityShared_t r;
    gclient_s *client;
    char padding1[0x28];
    int flags;
    char padding2[0xF8];
};

static_assert(sizeof(gentity_s) == 0x280, "size of gentity_s different than 0x280");

struct client_t
{
    char padding1[0x2141C];
    gentity_s *gentity;
    char padding2[0x47760];
};

static_assert(sizeof(client_t) == 0x68B80, "size of client_t different than 0x68B80");

}
}
