#pragma once

// Ghosts alpha structs used by the game functions.
namespace AlphaGhosts
{
namespace Game
{

enum WeaponAnimNumber
{
    WEAP_FORCE_IDLE = 0x1,
    WEAP_RELOAD = 0x12,
    WEAP_RELOAD_START = 0x14,
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
    char padding2[0x444];
};

static_assert(sizeof(PlayerWeaponCommonState) == 0x458, "size of PlayerWeaponCommonState different than 0x458");

struct playerState_s
{
    char padding1[0xC];
    uint32_t pm_flags;
    char padding2[0xC];
    vec3 origin;
    char padding3[0x144];
    vec3 viewAngles;
    char padding4[0x134];
    PlayerActiveWeaponState weapState[2];
    char padding5[0x3C];
    PlayerEquippedWeaponState weapEquippedData[15];
    PlayerWeaponCommonState weapCommon;
    char padding6[0x2E54];
};

static_assert(sizeof(playerState_s) == 0x3680, "size of playerState_s different than 0x3680");

struct usercmd_s
{
    char padding1[0x4];
    uint32_t buttons;
    char padding2[0x30];
};

static_assert(sizeof(usercmd_s) == 0x38, "size of usercmd_s different than 0x38");

struct pmove_t
{
    playerState_s *ps;
    usercmd_s cmd;
    char padding1[0x38];
    uint32_t tracemask;
    char padding2[0xF0];
};

static_assert(sizeof(pmove_t) == 0x168, "size of pmove_t different than 0x168");

struct pml_t
{
    char padding1[0x88];
    uint32_t holdrand;
    char padding2[0xC];
};

static_assert(sizeof(pml_t) == 0x98, "size of pml_t different than 0x98");

struct gclient_s
{
    playerState_s ps;
    char padding1[0x360];
    int mFlags;
    char padding2[0x41C];
};

static_assert(sizeof(gclient_s) == 0x3E00, "size of gclient_s different than 0x3E00");

struct entityState_s
{
    char padding1[0x8C];
    int index;
    int clientNum;
    char padding2[0x6C];
};

static_assert(sizeof(entityState_s) == 0x100, "size of entityState_s different than 0x100");

struct entityShared_t
{
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

static_assert(sizeof(entityShared_t) == 0x58, "size of entityShared_t different than 0x58");

struct gentity_s
{
    entityState_s state;
    entityShared_t r;
    gclient_s *client;
    char padding1[0x30];
    int flags;
    char padding2[0xF0];
};

static_assert(sizeof(gentity_s) == 0x280, "size of gentity_s different than 0x280");

struct client_t
{
    char padding1[0x31C90];
    gentity_s *gentity;
    char padding2[0x3CCEC];
};

static_assert(sizeof(client_t) == 0x6E980, "size of client_t different than 0x6E980");

}
}
