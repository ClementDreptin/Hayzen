#pragma once

// Spec Ops MW2 Alpha structs used by the game functions.
namespace SpecOpsAlphaMW2
{
namespace Game
{

struct playerState_s
{
    char padding1[0xC];
    uint32_t pm_flags;
    int otherFlags;
    char padding2[0x8];
    vec3 origin;
    char padding3[0xE0];
    vec3 viewAngles;
    char padding4[0x306C];
};

static_assert(sizeof(playerState_s) == 0x3180, "size of playerState_s different than 0x3180");

struct pmove_t
{
    playerState_s *ps;
    char padding1[0x80];
    uint32_t tracemask;
    char padding2[0xB8];
};

static_assert(sizeof(pmove_t) == 0x140, "size of pmove_t different than 0x140");

struct gclient_s
{
    playerState_s ps;
    char padding1[0x2C];
    int connected;
    char padding2[0x270];
    int mFlags;
    char padding3[0x2DC];
};

static_assert(sizeof(gclient_s) == 0x3700, "size of gclient_s different than 0x3700");

struct entityState_s
{
    int number;
    char padding1[0x78];
    int otherEntityNum;
    char padding2[0x4];
    int groundEntityNum;
    char padding3[0x8];
    int index;
    char padding4[0x6C];
};

static_assert(sizeof(entityState_s) == 0x100, "size of entityState_s different than 0x100");

struct entityShared_t
{
    bool linked;
    char bmodel;
    char svFlags;
    bool inuse;
    char padding1[0x18];
    int contents;
    char padding2[0x18];
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
    char padding1[0x128];
};

static_assert(sizeof(gentity_s) == 0x280, "size of gentity_s different than 0x280");

}
}
