#pragma once


// MW3 TU24 structs used by the game functions.
namespace MW3Structs
{

struct playerState_s
{
    CHAR padding1[0x1C];
    vec3 origin;
    CHAR padding2[0x130];
    vec3 viewAngles;
    CHAR padding3[0x319C];
};

struct gclient_s
{
    playerState_s ps;
    CHAR padding1[0x2FF];
    BYTE mFlags;
    CHAR padding2[0x380];
};

struct clientState_s
{
    CHAR padding1[0x44];
    CHAR name[32];
    CHAR padding2[0x7C];
};

struct entityState_s
{
    INT number;
    CHAR padding1[0x88];
    INT index;
    CHAR padding2[0x6C];
};

struct entityShared_t
{
    INT clientMask;
    bool linked;
    CHAR bmodel;
    CHAR svFlags;
    bool inuse;
    vec3 mins;
    vec3 maxs;
    INT contents;
    vec3 absmin;
    vec3 absmax;
    vec3 currentOrigin;
    vec3 currentAngles;
    INT ownerNum;
    INT eventTime;
};

struct gentity_s
{
    entityState_s state;
    entityShared_t r;
    gclient_s *client;
    CHAR padding1[0x28];
    INT flags;
    CHAR padding2[0xF8];
};

}
