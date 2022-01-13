#pragma once


// MW3 TU24 structs used by the game functions.
namespace MW3Structs
{

struct playerState_s
{
    char padding1[0x1C];
    vec3 origin;
    char padding2[0x130];
    vec3 viewAngles;
    char padding3[0x319C];
};

struct gclient_s
{
    playerState_s ps;
    char padding1[0x2FF];
    uint8_t mFlags;
    char padding2[0x380];
};

struct clientState_s
{
    char padding1[0x44];
    char name[32];
    char padding2[0x7C];
};

struct entityState_s
{
    int number;
    char padding1[0x88];
    int index;
    char padding2[0x6C];
};

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

struct gentity_s
{
    entityState_s state;
    entityShared_t r;
    gclient_s *client;
    char padding1[0x28];
    int flags;
    char padding2[0xF8];
};

}
