#pragma once


// Spec Ops MW3 structs used by the game functions.
namespace SpecOpsMW3Structs
{

struct playerState_s
{
    char padding1[0x10];
    int otherFlags;
    char padding2[0x8];
    vec3 origin;
    char padding3[0xE4];
    vec3 viewAngles;
    char padding4[0xAC3C];
};

struct gclient_s
{
    playerState_s ps;
    int connected;
    char padding1[0xAC];
    int mFlags;
    char padding2[0x2C0];
};

struct entityState_s
{
    char padding1[0x84];
    int16_t number;
    int16_t otherEntityNum;
    int16_t groundEntityNum;
    int16_t index;
    char padding3[0x24];
};

struct entityShared_t
{
    bool linked;
    char bmodel;
    int clientMask;
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
    char padding1[0x160];
};

}
