#pragma once

// NX1 structs used by the game functions.
namespace NX1
{
namespace Game
{

struct playerState_s
{
    char padding1[0x1C];
    vec3 origin;
    char padding2[0xF0];
    vec3 viewAngles;
    char padding3[0xB514];
};

static_assert(sizeof(playerState_s) == 0xB638, "size of playerState_s different than 0xB638");

struct gclient_s
{
    char padding1[0x3724];
    int mFlags;
    char padding2[0x3D8];
};

static_assert(sizeof(gclient_s) == 0x3B00, "size of gclient_s different than 0x3B00");

struct entityState_s
{
    uint32_t number;
    char padding1[0x94];
    int index; // "index" was kept to be consistent with the other games but it's called "solid" in the NX1, "index" is something else
    char padding2[0x24];
};

static_assert(sizeof(entityState_s) == 0xC0, "size of entityState_s different than 0xC0");

struct entityShared_t
{
    char padding1[0x1];
    char bmodel;
    char padding2[0x1A];
    int contents;
    char padding3[0x18];
    vec3 currentOrigin;
    vec3 currentAngles;
    char padding4[0x8];
};

static_assert(sizeof(entityShared_t) == 0x58, "size of entityShared_t different than 0x58");

struct gentity_s
{
    entityState_s state;
    char padding1[0xC0];
    entityShared_t r;
    char padding2[0x2C];
    int flags;
    char padding3[0xF8];
};

static_assert(sizeof(gentity_s) == 0x300, "size of gentity_s different than 0x300");

struct client_t
{
    char padding1[0x5494];
    gentity_s *gentity;
    char padding2[0x7CCE8];
};

static_assert(sizeof(client_t) == 0x82180, "size of client_t different than 0x82180");

}
}
