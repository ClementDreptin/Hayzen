#pragma once


//--------------------------------------------------------------------------------------
// Spec Ops MW2 structs used by the game functions
//--------------------------------------------------------------------------------------
namespace SpecOpsMW2Structs
{

struct playerState_s
{
    CHAR padding1[0x10];
    INT otherFlags;
    CHAR padding2[0x8];
    vec3 origin;
    CHAR padding3[0xE0];
    vec3 viewAngles;
    CHAR padding4[0xAB90];
};

struct gclient_s
{
    playerState_s ps;
    CHAR padding1[0xB0];
    INT mFlags;
    CHAR padding2[0x27C];
};

struct entityState_s
{
    CHAR padding1[0x7E];
    SHORT number;
    SHORT otherEntityNum;
    SHORT groundEntityNum;
    SHORT index;
    CHAR padding3[0x26];
};

struct entityShared_t
{
    bool linked;
    CHAR bmodel;
    INT clientMask;
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
    CHAR padding1[0x168];
};

}
