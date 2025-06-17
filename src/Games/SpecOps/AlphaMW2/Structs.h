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
    char padding2[0x27C];
};

static_assert(sizeof(gclient_s) == 0xAFD4, "size of gclient_s different than 0xAFD4");

struct entityState_s
{
    char padding1[0x7E];
    uint16_t number;
    uint16_t otherEntityNum;
    uint16_t groundEntityNum;
    uint16_t index;
    char padding2[0x26];
};

static_assert(sizeof(entityState_s) == 0xAC, "size of entityState_s different than 0xAC");

struct gentity_s
{
    entityState_s state;
    char padding1[0x1C4];
};

static_assert(sizeof(gentity_s) == 0x270, "size of gentity_s different than 0x270");

}
}
