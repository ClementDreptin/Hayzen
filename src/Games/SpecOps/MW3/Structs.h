#pragma once

// Spec Ops MW3 structs used by the game functions.
namespace SpecOpsMW3
{
namespace Game
{

struct playerState_s
{
    char padding1[0xC];
    int pm_flags;
    int otherFlags;
    char padding2[0x8];
    vec3 origin;
    char padding3[0xE4];
    vec3 viewAngles;
    char padding4[0xAC3C];
};

static_assert(sizeof(playerState_s) == 0xAD54, "size of playerState_s different than 0xAD54");

struct gclient_s
{
    playerState_s ps;
    int connected;
    char padding1[0xAC];
    int mFlags;
    char padding2[0x2C0];
};

static_assert(sizeof(gclient_s) == 0xB0C8, "size of gclient_s different than 0xB0C8");

struct entityState_s
{
    char padding1[0x84];
    int16_t number;
    int16_t otherEntityNum;
    int16_t groundEntityNum;
    int16_t index;
    char padding3[0x24];
};

static_assert(sizeof(entityState_s) == 0xB0, "size of entityState_s different than 0xB0");

struct entityShared_t
{
    bool linked;
    char bmodel;
    int clientMask;
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

static_assert(sizeof(entityShared_t) == 0x60, "size of entityShared_t different than 0x60");

struct gentity_s
{
    entityState_s state;
    entityShared_t r;
    char padding1[0x160];
};

static_assert(sizeof(gentity_s) == 0x270, "size of gentity_s different than 0x270");

}
}
