#pragma once

// CoD4 TU4 structs used by the game functions.
namespace CoD4
{
namespace Game
{

struct playerState_s
{
    char padding1[0xC];
    uint32_t pm_flags;
    char padding2[0xC];
    vec3 origin;
    char padding3[0x8C];
    int eFlags;
    char padding4[0x54];
    vec3 viewAngles;
    char padding5[0x2E4C];
};

static_assert(sizeof(playerState_s) == 0x2F64, "size of playerState_s different than 0x2F64");

struct pmove_t
{
    playerState_s *ps;
    char padding1[0x40];
    uint32_t tracemask;
    char padding2[0xD0];
};

static_assert(sizeof(pmove_t) == 0x118, "size of pmove_t different than 0x118");

struct gclient_s
{
    playerState_s ps;
    char padding1[0x148];

    // This is technically "ufo" on CoD4 but we keep the same name as the other games to
    // avoid creating specific cases in the common functions
    int mFlags;

    int bFrozen;
    char padding2[0x100];
};

static_assert(sizeof(gclient_s) == 0x31B4, "size of gclient_s different than 0x31B4");

struct entityState_s
{
    int number;
    char padding1[0x84];
    int index;
    char padding2[0x68];
};

static_assert(sizeof(entityState_s) == 0xF4, "size of entityState_s different than 0xF4");

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
    char padding1[0x20];
    int flags;
    char padding2[0xF4];
};

static_assert(sizeof(gentity_s) == 0x278, "size of gentity_s different than 0x278");

struct client_t
{
    char padding1[0x21280];
    gentity_s *gentity;
    char padding2[0x81A04];
};

static_assert(sizeof(client_t) == 0xA2C88, "size of client_t different than 0xA2C88");

}
}
