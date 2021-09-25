#pragma once


//--------------------------------------------------------------------------------------
// MW2 TU9 strucs used by the game functions
//--------------------------------------------------------------------------------------
namespace MW2Structs
{

struct playerState_s
{
    CHAR padding1[0x1C];
    vec3 origin;
    CHAR padding2[0xE4];
    vec3 viewAngles;
    CHAR padding3[0x3068];
};

struct gclient_s
{
    playerState_s ps;
    CHAR padding1[0x2A0];
    INT mFlags;
    CHAR padding2[0x2DC];
};

struct clientState_s
{
    CHAR padding1[0x44];
    CHAR name[32];
    CHAR padding2[0x28];
};

struct entityState_s
{
    INT number;
    CHAR padding1[0x84];
    INT index;
    CHAR padding2[0x70];
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
    gclient_s* client;
    CHAR padding1[0x28];
    INT flags;
    CHAR padding2[0xF8];
};

}
