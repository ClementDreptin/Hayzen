#pragma once

namespace MW2
{
    using Math::vec2;
    using Math::vec3;

    enum he_type_t
    {
        HE_TYPE_FREE,
        HE_TYPE_TEXT,
        HE_TYPE_VALUE,
        HE_TYPE_PLAYERNAME,
        HE_TYPE_MATERIAL,
        HE_TYPE_MAPNAME,
        HE_TYPE_GAMETYPE,
        HE_TYPE_TIMER_DOWN,
        HE_TYPE_TIMER_UP,
        HE_TYPE_TIMER_STATIC,
        HE_TYPE_TENTHS_TIMER_DOWN,
        HE_TYPE_TENTHS_TIMER_UP,
        HE_TYPE_CLOCK_DOWN,
        HE_TYPE_CLOCK_UP,
        HE_TYPE_WAYPOINT,
        HE_TYPE_COUNT,
    };

    typedef enum align_t
    {
        ALIGN_TOP_LEFT = 0,
        ALIGN_MIDDLE_LEFT = 1,
        ALIGN_BOTTOM_LEFT = 2,
        ALIGN_TOP_MIDDLE = 4,
        ALIGN_MIDDLE_MIDDLE = 5,
        ALIGN_BOTTOM_MIDDLE = 6,
        ALIGN_TOP_RIGHT = 8,
        ALIGN_MIDDLE_RIGHT = 9,
        ALIGN_BOTTOM_RIGHT = 10
    };
    
    struct hudelem_color_t
    {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE a;
    };

    struct hudelem_s
    {
        he_type_t type;
        FLOAT y;
        FLOAT x;
        FLOAT z;
        INT targetEntNum;
        FLOAT fontScale;
        FLOAT fromFontScale;
        INT fontScaleStartTime;
        INT fontScaleTime;
        INT label;
        INT font;
        align_t alignOrg;
        align_t alignScreen;
        hudelem_color_t color;
        hudelem_color_t fromColor;
        INT fadeStartTime;
        INT fadeTime;
        INT height;
        INT width;
        INT materialIndex;
        INT fromHeight;
        INT fromWidth;
        INT scaleStartTime;
        INT scaleTime;
        FLOAT fromY;
        FLOAT fromX;
        INT fromAlignOrg;
        INT fromAlignScreen;
        INT moveStartTime;
        INT moveTime;
        FLOAT value;
        INT time;
        INT duration;
        INT text;
        FLOAT sort;
        hudelem_color_t glowColor;
        INT fxBirthTime;
        INT fxLetterTime;
        INT fxDecayStartTime;
        INT fxDecayDuration;
        INT soundID;
        INT flags;
    };
 
    struct game_hudelem_s
    {
        hudelem_s elem;
        INT clientNum;
        INT teamNum;
        INT archived;
    };

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