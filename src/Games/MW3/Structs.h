#pragma once

// MW3 TU24 structs used by the game functions.
namespace MW3
{
namespace Game
{

struct playerState_s
{
    char padding1[0xC];
    uint32_t pm_flags;
    char padding2[0xC];
    vec3 origin;
    char padding3[0x130];
    vec3 viewAngles;
    char padding4[0x319C];
};

static_assert(sizeof(playerState_s) == 0x3300, "size of playerState_s different than 0x3300");

struct pmove_t
{
    playerState_s *ps;
    char padding1[0x58];
    uint32_t tracemask;
    char padding2[0xD8];
};

static_assert(sizeof(pmove_t) == 0x138, "size of pmove_t different than 0x138");

struct gclient_s
{
    playerState_s ps;
    char padding1[0x2FF];
    uint8_t mFlags;
    char padding2[0x380];
};

static_assert(sizeof(gclient_s) == 0x3980, "size of gclient_s different than 0x3980");

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
    char padding1[0x5];
    char bmodel;
    char padding2[0x1A];
    int contents;
    char padding3[0x18];
    vec3 currentOrigin;
    vec3 currentAngles;
    char padding4[0x8];
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

typedef enum _clientstate_t
{
    CS_FREE,
    CS_ZOMBIE,
    CS_RECONNECTING,
    CS_CONNECTED,
    CS_CLIENTLOADING,
    CS_ACTIVE,
} clientstate_t;

typedef enum _netadrtype_t
{
    NA_BOT,
    NA_BAD,
    NA_LOOPBACK,
    NA_BROADCAST,
    NA_IP,
} netadrtype_t;

struct netadr_t
{
    netadrtype_t type;
    char padding1[0xC];
};

static_assert(sizeof(netadr_t) == 0x10, "size of netadr_t different than 0x10");

struct clientHeader_t
{
    clientstate_t state;
    char padding1[0x65C];
};

static_assert(sizeof(clientHeader_t) == 0x660, "size of clientHeader_t different than 0x660");

struct client_t
{
    clientHeader_t header;
    char padding1[0x20DBC];
    gentity_s *gentity;
    char padding2[0x13EC6];
    uint16_t scriptId;
    char padding3[0x2];
    int bIsTestClient;
    char padding4[0x33890];
};

static_assert(sizeof(client_t) == 0x68B80, "size of client_t different than 0x68B80");

struct usercmd_s
{
    char padding1[0x2C];
};

static_assert(sizeof(usercmd_s) == 0x2C, "size of usercmd_s different than 0x10");

}
}
