#pragma once

// MW2 TU9 structs used by the game functions.
namespace MW2
{
namespace Game
{

struct playerState_s
{
    char padding1[0xC];
    uint32_t pm_flags;
    char padding2[0xC];
    vec3 origin;
    char padding3[0xE4];
    vec3 viewAngles;
    char padding4[0x3068];
};

static_assert(sizeof(playerState_s) == 0x3180, "size of playerState_s different than 0x3180");

struct pmove_t
{
    playerState_s *ps;
    char padding1[0x50];
    uint32_t tracemask;
    char padding2[0xD0];
};

static_assert(sizeof(pmove_t) == 0x128, "size of pmove_t different than 0x128");

struct gclient_s
{
    playerState_s ps;
    char padding1[0x2A0];
    int mFlags;
    char padding2[0x2DC];
};

static_assert(sizeof(gclient_s) == 0x3700, "size of gclient_s different than 0x3700");

struct entityState_s
{
    int number;
    char padding1[0x84];
    int index;
    char padding2[0x70];
};

static_assert(sizeof(entityState_s) == 0xFC, "size of entityState_s different than 0xFC");

struct entityShared_t
{
    char padding1[0x5];
    char bmodel;
    char padding2[0x1A];
    int contents;
    char padding3[0x18];
    vec3 currentOrigin;
    vec3 currentAngles;
    char padding4[0x8];
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
    char padding1[0x21294];
    gentity_s *gentity;
    char padding2[0x76CE8];
};

static_assert(sizeof(client_t) == 0x97F80, "size of client_t different than 0x97F80");

}
}
