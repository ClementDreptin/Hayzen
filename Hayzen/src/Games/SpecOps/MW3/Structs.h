#pragma once

namespace SpecOps
{
namespace MW3
{
    using Math::vec2;
    using Math::vec3;

    struct Font_s
    {
        INT fontName;
        INT pixelHeight;
        INT glyphCount;
        INT material;
        INT glowMaterial;
        INT glyphs;
    };

    struct playerState_s
    {
        CHAR padding1[0x10];
        INT otherFlags;
        CHAR padding2[0x8];
        vec3 origin;
        CHAR padding3[0xE4];
        vec3 viewAngles;
        CHAR padding4[0xAC3C];
    };

    struct gclient_s
    {
        playerState_s ps;
        CHAR padding1[0xB0];
        INT mFlags;
        CHAR padding2[0x2C0];
    };

    struct entityState_s
    {
        CHAR padding1[0x84];
        SHORT number;
        SHORT otherEntityNum;
        SHORT groundEntityNum;
        SHORT index;
        CHAR padding3[0x24];
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
        CHAR padding1[0x160];
    };
}
}