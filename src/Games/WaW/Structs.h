#pragma once

// WaW TU4 structs used by the game functions.
namespace WaW
{
namespace Game
{

struct playerState_s
{
    char padding1[0xC];
    uint32_t pm_flags;
    char padding2[0x10];
    vec3 origin;
    char padding3[0xA0];
    int eFlags;
    char padding4[0x54];
    vec3 viewAngles;
    char padding5[0x39EC];
};

static_assert(sizeof(playerState_s) == 0x3B1C, "size of playerState_s different than 0x3B1C");

struct pmove_t
{
    playerState_s *ps;
    char padding1[0x58];
    uint32_t tracemask;
    char padding2[0xD0];
};

static_assert(sizeof(pmove_t) == 0x130, "size of pmove_t different than 0x130");

struct gclient_s
{
    playerState_s ps;
    char padding1[0x4];

    // This is technically "ufo" on WaW but we keep the same name as the other games to
    // avoid creating specific cases in the common functions
    int mFlags;

    char padding2[0x148];
};

static_assert(sizeof(gclient_s) == 0x3C6C, "size of gclient_s different than 0x3C6C");

struct entityState_s
{
    int number;
    char padding1[0xA8];
    int index;
    char padding2[0x6C];
};

static_assert(sizeof(entityState_s) == 0x11C, "size of entityState_s different than 0x11C");

struct entityShared_t
{
    char padding1[0x1];
    char bmodel;
    char padding2[0x2A];
    int contents;
    char padding3[0x18];
    vec3 currentOrigin;
    vec3 currentAngles;
    char padding4[0x8];
};

static_assert(sizeof(entityShared_t) == 0x68, "size of entityShared_t different than 0x68");

struct gentity_s
{
    entityState_s state;
    entityShared_t r;
    gclient_s *client;
    char padding1[0x2C];
    int flags;
    char padding2[0x178];
};

static_assert(sizeof(gentity_s) == 0x330, "size of gentity_s different than 0x330");

struct client_t
{
    char padding1[0x213F4];
    gentity_s *gentity;
    char padding2[0x96234];
};

static_assert(sizeof(client_t) == 0xB762C, "size of client_t different than 0xB762C");

}
}
