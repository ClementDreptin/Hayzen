#pragma once

// Spec Ops MW2 structs used by the game functions.
namespace SpecOpsMW2
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
    char padding4[0xAB90];
};

static_assert(sizeof(playerState_s) == 0xACA4, "size of playerState_s different than 0xACA4");

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
    int connected;
    char padding1[0xAC];
    int mFlags;
    char padding2[0x280];
};

static_assert(sizeof(gclient_s) == 0xAFD8, "size of gclient_s different than 0xAFD8");

struct entityState_s
{
    char padding1[0x7E];
    int16_t number;
    int16_t otherEntityNum;
    int16_t groundEntityNum;
    int16_t index;
    char padding3[0x26];
};

static_assert(sizeof(entityState_s) == 0xAC, "size of entityState_s different than 0xAC");

struct gentity_s
{
    entityState_s state;
    char padding1[0x1C8];
};

static_assert(sizeof(gentity_s) == 0x274, "size of gentity_s different than 0x274");

}
}
