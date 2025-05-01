#pragma once

// MW2 Alpha structs used by the game functions.
namespace AlphaMW2
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
    char padding4[0x310];
    uint32_t perks[2];
    char padding5[0x2D50];
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

struct clientState_s
{
    char padding1[0x70];
    uint32_t perks[2];
    char padding2[0x1C];
};

static_assert(sizeof(clientState_s) == 0x94, "size of clientState_s different than 0x94");

struct clientSession_t
{
    char padding1[0xCC];
    clientState_s _cs;
    char padding2[0x140];
};

static_assert(sizeof(clientSession_t) == 0x2A0, "size of clientSession_t different than 0x2A0");

struct gclient_s
{
    playerState_s ps;
    clientSession_t sess;
    int mFlags;
    char padding2[0x2DC];
};

static_assert(sizeof(gclient_s) == 0x3700, "size of gclient_s different than 0x3700");

struct entityState_s
{
    int number;
    char padding1[0x8C];
    int index;
    char padding2[0x68];
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
    char padding1[0x2128C];
    gentity_s *gentity;
    char padding2[0x76CF0];
};

static_assert(sizeof(client_t) == 0x97F80, "size of client_t different than 0x97F80");

}
}
