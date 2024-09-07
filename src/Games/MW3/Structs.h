#pragma once

// MW3 TU24 structs used by the game functions.
namespace MW3
{
namespace Game
{

struct playerState_s
{
    char padding1[0x1C];
    vec3 origin;
    char padding2[0x130];
    vec3 viewAngles;
    char padding3[0x319C];
};

static_assert(sizeof(playerState_s) == 0x3300, "size of playerState_s different than 0x3300");

struct gclient_s
{
    playerState_s ps;
    char padding1[0x2FF];
    uint8_t mFlags;
    char padding2[0x380];
};

static_assert(sizeof(gclient_s) == 0x3980, "size of gclient_s different than 0x3980");

struct clientState_s
{
    char padding1[0x44];
    char name[32];
    char padding2[0x7C];
};

static_assert(sizeof(clientState_s) == 0xE0, "size of clientState_s different than 0xE0");

struct entityState_s
{
    int number;
    char padding1[0x88];
    int index;
    char padding2[0x6C];
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
    char padding1[0x2141C];
    gentity_s *gentity;
    char padding2[0x47760];
};

static_assert(sizeof(client_t) == 0x68B80, "size of client_t different than 0x68B80");

}
}
