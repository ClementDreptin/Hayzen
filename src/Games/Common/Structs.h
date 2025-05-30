#pragma once

#pragma warning(push)
#pragma warning(disable : 4480)

enum DvarType : uint8_t
{
    DVAR_TYPE_BOOL = 0x0,
    DVAR_TYPE_FLOAT = 0x1,
    DVAR_TYPE_FLOAT_2 = 0x2,
    DVAR_TYPE_FLOAT_3 = 0x3,
    DVAR_TYPE_FLOAT_4 = 0x4,
    DVAR_TYPE_INT = 0x5,
    DVAR_TYPE_ENUM = 0x6,
    DVAR_TYPE_STRING = 0x7,
    DVAR_TYPE_COLOR = 0x8,
    DVAR_TYPE_FLOAT_3_COLOR = 0x9,
    DVAR_TYPE_COUNT = 0xA,
};

#pragma warning(pop)

struct EnumLimit
{
    int stringCount;
    const char **strings;
};

static_assert(sizeof(EnumLimit) == 0x8, "sizeof EnumLimit different than 0x8");

struct IntLimit
{
    int min;
    int max;
};

static_assert(sizeof(IntLimit) == 0x8, "sizeof IntLimit different than 0x8");

struct FloatLimit
{
    float min;
    float max;
};

static_assert(sizeof(FloatLimit) == 0x8, "sizeof FloatLimit different than 0x8");

union DvarLimits {
    EnumLimit enumeration;
    IntLimit integer;
    FloatLimit value;
    FloatLimit vector;
};

static_assert(sizeof(DvarLimits) == 0x8, "sizeof DvarLimits different than 0x8");

union DvarValue {
    bool enabled;
    int integer;
    uint32_t unsignedInt;
    float value;
    float vector[4];
    const char *string;
    uint8_t color[4];
};

static_assert(sizeof(DvarValue) == 0x10, "sizeof DvarValue different than 0x10");

struct dvar_t
{
    const char *name;
    const char *description;
    uint16_t flags;
    DvarType type;
    bool modified;
    DvarValue current;
    DvarValue latched;
    DvarValue reset;
    DvarLimits domain;
    dvar_t *hashNext;
};

static_assert(sizeof(dvar_t) == 0x48, "sizeof dvar_t different than 0x48");
