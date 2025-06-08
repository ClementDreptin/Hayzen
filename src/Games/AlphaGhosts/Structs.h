#pragma once

// Ghosts alpha structs used by the game functions.
namespace AlphaGhosts
{
namespace Game
{

struct playerState_s
{
    char padding1[0xC];
    uint32_t pm_flags;
    char padding2[0xC];
    vec3 origin;
    char padding3[0x144];
    vec3 viewAngles;
    char padding4[0x3508];
};

static_assert(sizeof(playerState_s) == 0x3680, "size of playerState_s different than 0x3680");

struct pmove_t
{
    playerState_s *ps;
    char padding1[0x70];
    uint32_t tracemask;
    char padding2[0xF0];
};

static_assert(sizeof(pmove_t) == 0x168, "size of pmove_t different than 0x168");

struct gclient_s
{
    playerState_s ps;
    char padding1[0x360];
    uint8_t mFlags;
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
