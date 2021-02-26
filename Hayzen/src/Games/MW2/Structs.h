#pragma once

namespace MW2
{
	struct vec2
	{
		float x;
		float y;
	};
	
	struct vec3
	{
		float x;
		float y;
		float z;
	};
	
	enum he_type_t : int
	{
		HE_TYPE_FREE = 0x0,
		HE_TYPE_TEXT = 0x1,
		HE_TYPE_VALUE = 0x2,
		HE_TYPE_PLAYERNAME = 0x3,
		HE_TYPE_MATERIAL = 0x4,
		HE_TYPE_TIMER_DOWN = 0x5,
		HE_TYPE_TIMER_UP = 0x6,
		HE_TYPE_TIMER_STATIC = 0x7,
		HE_TYPE_TENTHS_TIMER_DOWN = 0x8,
		HE_TYPE_TENTHS_TIMER_UP = 0x9,
		HE_TYPE_TENTHS_TIMER_STATIC = 0xA,
		HE_TYPE_CLOCK_DOWN = 0xB,
		HE_TYPE_CLOCK_UP = 0xC,
		HE_TYPE_WAYPOINT = 0xD,
		HE_TYPE_MAPNAME = 0xE,
		HE_TYPE_GAMETYPE = 0xF,
		HE_TYPE_COUNT = 0x10,
	};
	
	union hudelem_color_t
	{
		struct
		{
			char r;
			char g;
			char b;
			char a;
		};
		int rgba;
	};
	
	struct hudelem_s
	{
		he_type_t type;
		float x;
		float y;
		float z;
		int targetEntNum;
		float fontScale;
		float fromFontScale;
		int font;
		int alignOrg;
		int alignScreen;
		hudelem_color_t color;
		hudelem_color_t fromColor;
		int fadeStartTime;
		int fadeTime;
		int label;
		int width;
		int height;
		int materialIndex;
	#if defined(WAW) || defined(COD4)
		int offscreenMaterialIdx;
	#endif
		int fromWidth;
		int fromHeight;
		int scaleStartTime;
		int scaleTime;
		float fromX;
		float fromY;
		int fromAlignOrg;
		int fromAlignScreen;
		int moveStartTime;
		int moveTime;
		int time;
		int duration;
		float value;
		int text;
		float sort;
		hudelem_color_t glowColor;
		int fxBirthTime;
		int fxLetterTime;
		int fxDecayStartTime;
		int fxDecayDuration;
		int soundID;
		int flags;
	};
	
	struct game_hudelem_s
	{
		hudelem_s hud;
		int clientNum;
		int team;
		int archived;
	#ifdef GHOSTS
		int currentShowInKillcam;
	#endif
	};
	
	enum team_t {
		TEAM_FREE,
		TEAM_AXIS,
		TEAM_ALLIES,
		TEAM_SPECTATOR
	};
	
	enum serverState_t {
		SS_DEAD,
		SS_LOADING,
		SS_GAME
	};
	
	enum sessionState_t {
		SESS_STATE_PLAYING,
		SESS_STATE_DEAD,
		SESS_STATE_SPECTATOR,
		SESS_STATE_INTERMISSION
	};
	
	enum clientConnected_t {
		CON_DISCONNECTED,
		CON_CONNECTING,
		CON_CONNECTED
	};
	
	enum objectiveState_t {
		OBJST_EMPTY,
		OBJST_ACTIVE,
		OBJST_INVISIBLE,
		OBJST_DONE,
		OBJST_CURRENT,
		OBJST_FAILED
	};
	
	enum DvarSetSource {
		DVAR_SOURCE_INTERNAL,
		DVAR_SOURCE_EXTERNAL,
		DVAR_SOURCE_SCRIPT
	};
	
	enum trType_t {
		TR_STATIONARY,
		TR_INTERPOLATE,
		TR_LINEAR,
		TR_LINEAR_STOP,
		TR_SINE,
		TR_GRAVITY,
		TR_ACCELERATE,
		TR_DECELERATE,
		TR_PHYSICS,
		TR_RAGDOLL
	};
	
	enum entityType_t {
		ET_GENERAL,
		ET_PLAYER,
		ET_PLAYER_CORPSE,
		ET_ITEM,
		ET_MISSILE,
		ET_INVISIBLE,
		ET_SCRIPTMOVER,
		ET_SOUND_BLEND,
		ET_FX,
		ET_LOOP_FX,
		ET_PRIMARY_LIGHT,
		ET_TURRET,
		ET_HELICOPTER,
		ET_PLANE,
		ET_VEHICLE,
		ET_VEHICLE_COLLMAP,
		ET_VEHICLE_CORPSE,
		ET_VEHICLE_SPAWNER
	};
	
	enum weapType_t {
		WEAPTYPE_BULLET,
		WEAPTYPE_GRENADE,
		WEAPTYPE_PROJECTILE,
		WEAPTYPE_RIOTSHIELD
	};
	
	enum weapClass_t {
		WEAPCLASS_RIFLE,
		WEAPCLASS_SNIPER,
		WEAPCLASS_MG,
		WEAPCLASS_SMG,
		WEAPCLASS_SPREAD,
		WEAPCLASS_PISTOL,
		WEAPCLASS_GRENADE,
		WEAPCLASS_ROCKETLAUNCHER,
		WEAPCLASS_TURRET,
		WEAPCLASS_THROWINGKNIFE,
		WEAPCLASS_NON_PLAER,
		WEAPCLASS_ITEM
	};
	
	enum PlayerHandIndex {
		WEAPON_HAND_RIGHT,
		WEAPON_HAND_LEFT,
		NUM_WEAPON_HANDS,
		WEAPON_HAND_DEFAULT = 0,
	};
	
	enum pmtype_t {
		PM_NORMAL,
		PM_NORMAL_LINKED,
		PM_NOCLIP,
		PM_UFO,
		PM_SPECTATOR = 5,
		PM_INTERMISSION,
		PM_LASTSTAND,
		PM_DEAD,
		PM_DEAD_LINKED
	};
	
	enum eventType_t {
		EV_NONE,
		EV_FOLIAGE_SOUND,
		EV_STOP_WEAPON_SOUND,
		EV_SOUND_ALIAS,
		EV_SOUND_ALIAS_AS_MASTER,
		EV_STOPSOUNDS,
		EV_STANCE_FORCE_STAND,
		EV_STANCE_FORCE_CROUCH,
		EV_STANCE_FORCE_PRONE,
		EV_STANCE_INVALID,
		EV_ITEM_PICKUP,
		EV_AMMO_PICKUP,
		EV_NOAMMO,
		EV_EMPTYCLIP,
		EV_EMPTY_OFFHAND_PRIMARY,
		EV_EMPTY_OFFHAND_SECONDARY,
		EV_OFFHAND_END_NOTIFY,
		EV_RESET_ADS,
		EV_RELOAD,
		EV_RELOAD_FROM_EMPTY,
		EV_RELOAD_START,
		EV_RELOAD_END,
		EV_RELOAD_START_NOTIFY,
		EV_RELOAD_ADDAMMO,
		EV_RAISE_WEAPON,
		EV_FIRST_RAISE_WEAPON,
		EV_PUTAWAY_WEAPON,
		EV_WEAPON_ALT,
		EV_WEAPON_SWITCH_STARTED,
		EV_PULLBACK_WEAPON,
		EV_FIRE_WEAPON,
		EV_FIRE_WEAPON_LASTSHOT,
		EV_FIRE_RICOCHET,
		EV_RECHAMBER_WEAPON,
		EV_EJECT_BRASS,
		EV_FIRE_WEAPON_LEFT,
		EV_FIRE_WEAPON_LASTSHOT_LEFT,
		EV_EJECT_BRASS_LEFT,
		EV_HITCLIENT_FIRE_WEAPON,
		EV_HITCLIENT_FIRE_WEAPON_LASTSHOT,
		EV_HITCLIENT_FIRE_WEAPON_LEFT,
		EV_HITCLIENT_FIRE_WEAPON_LASTSHOT_LEFT,
		EV_SV_FIRE_WEAPON,
		EV_SV_FIRE_WEAPON_LASTSHOT,
		EV_SV_FIRE_WEAPON_LEFT,
		EV_SV_FIRE_WEAPON_LASTSHOT_LEFT,
		EV_MELEE_SWIPE,
		EV_FIRE_MELEE,
		EV_PREP_OFFHAND,
		EV_USE_OFFHAND,
		EV_SWITCH_OFFHAND,
		EV_MELEE_HIT,
		EV_MELEE_MISS,
		EV_MELEE_BLOOD,
		EV_FIRE_TURRET,
		EV_FIRE_SENTRY,
		EV_FIRE_QUADBARREL_1,
		EV_FIRE_QUADBARREL_2,
		EV_BULLET_HIT,
		EV_BULLET_HIT_SHIELD,
		EV_BULLET_HIT_EXPLODE,
		EV_BULLET_HIT_CLIENT_SMALL,
		EV_BULLET_HIT_CLIENT_LARGE,
		EV_BULLET_HIT_CLIENT_EXPLODE,
		EV_BULLET_HIT_CLIENT_SHIELD,
		EV_EXPLOSIVE_IMPACT_ON_SHIELD,
		EV_EXPLOSIVE_SPLASH_ON_SHIELD,
		EV_GRENADE_BOUNCE,
		EV_GRENADE_STICK,
		EV_GRENADE_REST,
		EV_GRENADE_EXPLODE,
		EV_GRENADE_PICKUP,
		EV_GRENADE_LETGO,
		EV_ROCKET_EXPLODE,
		EV_ROCKET_EXPLODE_NOMARKS,
		EV_FLASHBANG_EXPLODE,
		EV_CUSTOM_EXPLODE,
		EV_CUSTOM_EXPLODE_NOMARKS,
		EV_CHANGE_TO_DUD,
		EV_DUD_EXPLODE,
		EV_DUD_IMPACT,
		EV_TROPHY_EXPLODE,
		EV_BULLET,
		EV_PLAY_FX,
		EV_PLAY_FX_ON_TAG,
		EV_STOP_FX_ON_TAG,
		EV_PLAY_FX_ON_TAG_FOR_CLIENTS,
		EV_PHYS_EXPLOSION_SPHERE,
		EV_PHYS_EXPLOSION_CYLINDER,
		EV_PHYS_EXPLOSION_JOLT,
		EV_RADIUSDAMAGE,
		EV_PHYS_JITTER,
		EV_EARTHQUAKE,
		EV_GRENADE_SUICIDE,
		EV_DETONATE,
		EV_NIGHTVISION_WEAR,
		EV_NIGHTVISION_REMOVE,
		EV_MISSILE_REMOTE_BOOST,
		EV_PLAY_RUMBLE_ON_ENT,
		EV_PLAY_RUMBLE_ON_POS,
		EV_PLAY_RUMBLELOOP_ON_ENT,
		EV_PLAY_RUMBLELOOP_ON_POS,
		EV_STOP_RUMBLE,
		EV_STOP_ALL_RUMBLES,
		EV_OBITUARY,
		EV_NO_PRIMARY_GRENADE_HINT,
		EV_NO_SECONDARY_GRENADE_HINT,
		EV_TARGET_TOO_CLOSE_HINT,
		EV_TARGET_NOT_ENOUGH_CLEARANCE_HINT,
		EV_LOCKON_REQUIRED_HINT,
		EV_VEHICLE_COLLISION,
		EV_VEHICLE_SUSPENSION_SOFT,
		EV_VEHICLE_SUSPENSION_HARD,
		EV_FOOTSTEP_SPRINT,
		EV_FOOTSTEP_RUN,
		EV_FOOTSTEP_WALK,
		EV_FOOTSTEP_PRONE,
		EV_JUMP,
		EV_LANDING_DEFAULT,
		EV_LANDING_BARK,
		EV_LANDING_BRICK,
		EV_LANDING_CARPET,
		EV_LANDING_CLOTH,
		EV_LANDING_CONCRETE,
		EV_LANDING_DIRT,
		EV_LANDING_FLESH,
		EV_LANDING_FOLIAGE,
		EV_LANDING_GLASS,
		EV_LANDING_GRASS,
		EV_LANDING_GRAVEL,
		EV_LANDING_ICE,
		EV_LANDING_METAL,
		EV_LANDING_MUD,
		EV_LANDING_PAPER,
		EV_LANDING_PLASTER,
		EV_LANDING_ROCK,
		EV_LANDING_SAND,
		EV_LANDING_SNOW,
		EV_LANDING_WATER,
		EV_LANDING_WOOD,
		EV_LANDING_ASPHALT,
		EV_LANDING_CERAMIC,
		EV_LANDING_PLASTIC,
		EV_LANDING_RUBBER,
		EV_LANDING_CUSHION,
		EV_LANDING_FRUIT,
		EV_LANDING_PAINTEDMETAL,
		EV_LANDING_RIOTSHIELD,
		EV_LANDING_SLUSH,
		EV_LANDING_PAIN_DEFAULT,
		EV_LANDING_PAIN_BARK,
		EV_LANDING_PAIN_BRICK,
		EV_LANDING_PAIN_CARPET,
		EV_LANDING_PAIN_CLOTH,
		EV_LANDING_PAIN_CONCRETE,
		EV_LANDING_PAIN_DIRT,
		EV_LANDING_PAIN_FLESH,
		EV_LANDING_PAIN_FOLIAGE,
		EV_LANDING_PAIN_GLASS,
		EV_LANDING_PAIN_GRASS,
		EV_LANDING_PAIN_GRAVEL,
		EV_LANDING_PAIN_ICE,
		EV_LANDING_PAIN_METAL,
		EV_LANDING_PAIN_MUD,
		EV_LANDING_PAIN_PAPER,
		EV_LANDING_PAIN_PLASTER,
		EV_LANDING_PAIN_ROCK,
		EV_LANDING_PAIN_SAND,
		EV_LANDING_PAIN_SNOW,
		EV_LANDING_PAIN_WATER,
		EV_LANDING_PAIN_WOOD,
		EV_LANDING_PAIN_ASPHALT,
		EV_LANDING_PAIN_CERAMIC,
		EV_LANDING_PAIN_PLASTIC,
		EV_LANDING_PAIN_RUBBER,
		EV_LANDING_PAIN_CUSHION,
		EV_LANDING_PAIN_FRUIT,
		EV_LANDING_PAIN_PAINTEDMETAL,
		EV_LANDING_PAIN_RIOTSHIELD,
		EV_LANDING_PAIN_SLUSH,
		EV_MANTLE
	};
	
	enum vehicleManualMode_t {
		VEH_MANUAL_OFF,
		VEH_MANUAL_ON,
		VEH_MANUAL_TRANS
	};
	
	enum helicopterMoveState_t {
		VEH_MOVESTATE_STOP,
		VEH_MOVESTATE_MOVE,
		VEH_MOVESTATE_HOVER
	};
	
	enum vehicleDrivingState_t {
		VEH_DRIVE_NONE,
		VEH_DRIVE_PATH_CONSTRAINED,
		VEH_DRIVE_PATH_FOLLOW,
		VEH_DRIVE_TRACK,
		VEH_DRIVE_AI,
		VEH_DRIVE_PLAYER
	};
	
	enum DvarType : char {
		DVAR_TYPE_BOOL,
		DVAR_TYPE_FLOAT,
		DVAR_TYPE_VEC2,
		DVAR_TYPE_VEC3,
		DVAR_TYPE_VEC4,
		DVAR_TYPE_INT,
		DVAR_TYPE_ENUM,
		DVAR_TYPE_STRING,
		DVAR_TYPE_COLOR,
		DVAR_TYPE_DEVTWEAK
	};
	
	struct VariableStackBuffer {
		const char* pos;
		unsigned short size;
		unsigned short bufLen;
		unsigned int localId;
		char time;
		char buf[1];
	};
	
	union variableUnion {
		int intValue;
		float floatValue;
		unsigned int stringValue;
		float* vectorValue;
		const char* codePosValue;
		unsigned int pointerValue;
		VariableStackBuffer* stackValue;
		unsigned int entityOffset;
	};
	
	struct variableValue_s {
		variableUnion u; // 0x0
		int type; // 0x4
	};
	
	struct trace_t {
		float fraction; // 0x0
		vec3 normal; // 0x4
		int surfaceFlags; // 0x10
		int contents; // 0x14
		const char* material; // 0x18
		char hitType; // 0x1C
		unsigned short hitId; // 0x1E
		unsigned short modelIndex; // 0x20
		unsigned short partName; // 0x22
		unsigned short partGroup; // 0x24
		bool allsolid; // 0x26
		bool startsolid; // 0x27
		bool walkable; // 0x28
	};
	
	struct material_s {
		char* name; // 0x0
	};
	
	struct weaponVarientDef_s {
		char padding[0x8]; // 0x0
		char* displayName; // 0x8
		char padding2[0x24]; // 0x0
		float penetrationMultiplier; // 0x30
		char padding3[0x14]; // 0x34
		material_s* hudIcon; // 0x48
	
		const char* getWeaponShader() {
			return hudIcon == nullptr ? "NULL" : hudIcon->name;
		}
	};
	
	struct weaponDef_s {
		char padding[0x2C]; // 0x0
		weapType_t weapType; // 0x2C
		weapClass_t weapClass; // 0x30
		int penetrateType; // 0x34
		char padding2[0x624]; // 0x38
		bool rifleBullet; // 0x65C
		char padding3[4]; // 0x65D
		bool bBulletImpactExplode; // 0x661
	};
	
	struct weaponParms {
		vec3 forward; // 0x0
		vec3 right; // 0xC
		vec3 up; // 0x18
		vec3 muzzleTrace; // 0x24
		vec3 gunForward; // 0x30
		int weaponIndex; // 0x3C
		weaponDef_s* weaponDef; // 0x40
		weaponVarientDef_s* weaponVariant; // 0x44
	};
	
	struct EntHandle {
		short number; // 0x0
		short infoIndex; // 0x2
	};
	
	struct trajectory_t {
		int trTime; // 0x0
		trType_t trType; // 0x4
		int trDuration; // 0x8
		vec3 trBase; // 0xC
		vec3 trDelta; // 0x18
	};
	
	struct LerpEntityStatePlayer {
		int leanf; // 0x0
		int movementDir; // 0x4
		int torsoPitch; // 0x8
		int waistPitch; // 0xC
		int offhandWeapon; // 0x10
		int lastSpawnTime; // 0x14
	};
	
	struct LerpEntityStateAnonymous {
		int data[9]; // 0x0
	};
	
	struct LerpEntityStateMissile {
		int launchTime; // 0x0
		int flightMode; // 0x4
	};
	
	struct LerpEntityStateScriptMover {
		int entToTakeMarksFrom; // 0x0
		int xModelIndex; // 0x4
		int animIndex; // 0x8
		int animTime; // 0xC
	};
	
	struct LerpEntityStateSoundBlend {
		int lerp; // 0x0
	};
	
	struct LerpEntityStateLoopFx {
		int cullDist; // 0x0
		int period; // 0x4
	};
	
	struct LerpEntityStateTurret {
		float gunAngles[3]; // 0x0
		int lastBarrelRotChangeTime; // 0x4
		int lastBarrelRotChangeRate; // 0x8
		int lastHeatChangeTime; // 0xC
		int lastHeatChangeLevel; // 0x10
	};
	
	struct LerpEntityStateVehicle {
		int indices; // 0x0
		int flags; // 0x4
		int bodyPitch; // 0x8
		int bodyRoll; // 0xC
		int steerYaw; // 0x10
		int gunPitch; // 0x14
		int gunYaw; // 0x18
		int playerIndex; // 0x1C
		int pad; // 0x20
	};
	
	struct LerpEntityStatePlane {
		int ownerNum; // 0x0
		int enemyIcon; // 0x4
		int friendIcon; // 0x8
	};
	
	union LerpEntityStateTypeUnion {
		LerpEntityStateTurret turret;
		LerpEntityStateLoopFx loopFx;
		LerpEntityStatePlayer player;
		LerpEntityStateScriptMover scriptMover;
		LerpEntityStateVehicle vehicle;
		LerpEntityStatePlane plane;
		LerpEntityStateMissile missile;
		LerpEntityStateSoundBlend soundBlend;
		LerpEntityStateAnonymous anonymous;
	};
	
	struct LerpEntityState {
		int eFlags; // 0x0
		trajectory_t pos; // 0x4
		trajectory_t apos; // 0x24
		LerpEntityStateTypeUnion u; // 0x34
	};
	
	struct entityState_s {
		int number; // 0x0
		int eType; // 0x4
		int time2; // 0x8
		LerpEntityState lerp; // 0xC
		int otherEntityNum; // 0x7C
		int attackerEntityNum; // 0x80
		int groundEntityNum; // 0x84
		union {
			int brushmodel;
			int item;
			int xmodel;
			int primaryLight;
		} index; // 0x88
		int loopSound; // 0x8C
		int surfType; // 0x90
		int clientNum; // 0x94
		int headIconTeam; // 0x98
		int headIcon; // 0x9C
		unsigned int eventParm; // 0xA0
		int solid; // 0xA4
		int eventSequence; // 0xA8
		int events[4]; // 0xAC
		int eventParms[4]; // 0xBC
		int weapon; // 0xCC
		int legsAnim; // 0xD0
		int torsoAnim; // 0xD4
		union {
			int scale;
			int eventParm2;
			int helicopterStage;
		} un1; // 0xD8
	
		union {
			int hintString;
			int grenadeInPickupRange;
			int vehicleXModel;
		} un2; // 0xDC
		int clientLinkInfo; // 0xE0
		int partBits[6]; // 0xE4
	};
	
	struct enitityShared_t {
		int clientMask; // 0x0
		bool linked; // 0x4
		char bmodel; // 0x5
		char svFlags; // 0x6
		bool inuse; // 0x7
		vec3 mins; // 0x8
		vec3 maxs; // 0x14
		int contents; // 0x20
		vec3 absmin; // 0x24
		vec3 absmax; // 0x30
		vec3 currentOrigin; // 0x3C
		vec3 currentAngles; // 0x48
		EntHandle ownerNum; // 0x54
		int eventTime; // 0x58
	};
	
	struct hudData_s {
		hudelem_s current[31]; // 0x0
		hudelem_s archival[31]; // 0x1458
	};
	
	struct playerState_s {
		int commandTime; // 0x0
		int pm_type; // 0x4
		int bobCycle; // 0x8
		int pm_flags; // 0xC
		int otherFlags; // 0x10
		int linkFlags; // 0x14
		int pm_time; // 0x18
		vec3 origin; // 0x1C
		vec3 velocity; // 0x28
		int grenadeTimerLeft; // 0x34
		int throwBackGrenadeOwner; // 0x38
		char padding[0x18]; // 0x3C
		int gravity; // 0x54
		float leanf; // 0x58
		int speed; // 0x5C
		vec3 deltaAngles; // 0x60
		int groundEntityNum; // 0x6C
		vec3 vLadderVec; // 0x70
		int jumpTime; // 0x7C
		float jumpOriginZ; // 0x80
		char padding2[0x24]; // 0x84
		int corpseIndex; // 0xA8
		int movementDir; // 0xAC
		int eFlags; // 0xB0
		char padding3[0x50]; // 0xB4
		int clientNum; // 0x104
		int viewModelIndex; // 0x108
		vec3 viewAngles; // 0x10C
		char padding4[0x38];
		int health; // 0x150
		char padding5[0x4]; // 0x154
		int maxHealth; // 0x158
		char padding6[0x58]; // 0x15C
		int locationSelectionInfo; // 0x1B4
		char padding7[0x1C]; // 0x1B8
		float moveSpeedScaleMultiplier; // 0x1D4
		char padding8[0xDC]; // 0x1D8
		int weaponId; // 0x2B4
		char padding9[4]; // 0x2B8
		int weaponState; // 0x2BC
		float weaponPosFrac; // 0x2C0
		float aimSpreadScale; // 0x2C4
		char padding10[0x8C]; // 0x2C8
		int secondaryWeaponAmmoCount; // 0x354
		char padding11[0x14]; // 0x158
		int primaryWeaponAmmoCount; // 0x36C
		char padding12[0xB8]; // 0x370
		int perks; // 0x428
		int perkSlotCount; // 0x42C
		char padding13[0x5C]; // 0x430
		int shellshockIndex; // 0x48C
		int shellshockTime; // 0x490
		int shellshockDuration; // 0x494
		char padding14[0x3B0]; // 0x370
		hudData_s hud; // 0x848
		char padding15[0x98]; // 0x318C
	};
	
	struct usercmd_s {
		int serverTime; // 0x0
		int buttons; // 0x4
		int angles[3]; // 0x8
		char forwardmove; // 0x14
		char rightmove; // 0x15
		char undefined2[2]; // 0x16
		float meleeChargeYaw; // 0x18
		char meleeChargeDist; // 0x1C
		char _padding[4]; // 0x1D
		char selectedLoc[2]; // 0x21
		char selectedLocAngle; // 0x3
		int unknown; // 0x24
	};
	
	struct pmove_t {
		playerState_s* ps; // 0x0
	};
	
	struct pml_t {
		vec3 forward;
		vec3 right;
		vec3 up;
		float frametime;
		int msec;
		int walking;
		int groundPlane;
		int almostGroundPlane;
		trace_t groundTrace;
		float impactSpeed;
		vec3 previous_origin;
		vec3 previous_velocity;
	};
	
	struct playerTeamState_t {
		int location; // 0x0
	};
	
	struct clientState_s {
		int clientNum; // 0x0
		int team; // 0x4
		int modelIndex; // 0x8
		int dualWielding; // 0xC
		int riotShieldNext; // 0x10
		int attachModelIndex[6]; // 0x14
		int attachTagIndex[6]; // 0x2C
		char name[32]; // 0x44
		float maxSprintTimeMultiplier; // 0x64
		int rank; // 0x68
		int prestige; // 0x6C
		int perks[2]; // 0x70
		int voiceConnectivityBits; // 0x78
		int clanIndex; // 0x7C
		char clanName[4]; // 0x80
		int attachedVehEntNum; // 0x84
		int attachedVehSlotIndex; // 0x88
	};
	
	struct clientSession_s {
		sessionState_t sessionState; // 0x3190
		int clientNum; // 0x3194
		int score; // 0x3198
		int deaths; // 0x319C
		int kills; // 0x31A0
		int assists; // 0x31A4
		short scriptPersId; // 0x31A8
		short undefined; // 0x31AA
		clientConnected_t connect; // 0x31AC
		usercmd_s cmd; // 0x3200
		usercmd_s oldCmd; // 0x31D8
		int isLocalClient; // 0x31F8
		int predictItemPickup; // 0x31FC
		char name[32]; // 0x31DC
		int maxHealth; // 0x3228
		int enterTime; // 0x322C
		playerTeamState_t teamState; // 0x3230
		int voteCount; // 0x3234
		int teamVoteCount; // 0x3238
		float moveSpeedScaleMultiplier; // 0x323C
		int viewmodelIndex; // 0x3240
		int noSpectate; // 0x3244
		int teamInfo; // 0x3248
		clientState_s cs; // 0x324C
		char padding2[16]; // 0x3250
		int psOffsetTime; // 0x32E8
		char padding3[0x130]; // 0x32EC
	};
	
	struct viewClamp {
		vec2 start; // 0x0
		vec2 current; // 0x8
		vec2 goal; // 0x10
	};
	
	struct viewClampState {
		viewClamp min; // 0x0
		viewClamp max; // 0x18
		float accelTime; // 0x30
		float decelTime; // 0x34
		float totalTime; // 0x38
		float startTime; // 0x3C
	};
	
	struct gclient_s {
		playerState_s ps; // 0x0
		clientSession_s sess; // 0x3180
		int spectatorClient; // 0x341C
		int mFlags; // 0x3420
		char padding[4]; // 0x3424
		int lastCmdTime; // 0x3428
		int buttons; // 0x342C
		int oldButtons; // 0x3430
		int latched_buttons; // 0x3434
		int buttonsSinceLastFrame; // 0x3438
		vec3 oldOrigin; // 0x343C
		float fGunPitch; // 0x3448
		float fGunYaw; // 0x344C
		char padding2[8]; // 0x3450
		vec3 damage_from; // 0x3458
		int damage_fromWorld; // 0x3464
		int accurateCount; // 0x3468
		int accuracy_shots; // 0x346C
		int accuracy_hits; // 0x3470
		int inactivityTime; // 0x3474
		int inactivityWarning; // 0x3478
		int lastVoiceTime; // 0x347C
		int switchTeamTime; // 0x3480
		float currentAimSpreadScale; // 0x3484
		float prevLinkedInvQuat[4]; // 0x3488
		bool link_rotationMovesEyePos; // 0x3498
		bool link_doCollision; // 0x3499
		bool link_useTagAnglesForViewAngles; // 0x349A
		bool link_useBaseAnglesForViewClamp; // 0x349B
		float linkAnglesFrac; // 0x349C
		viewClampState link_viewClamp; // 0x34A0
	};
	
	struct turretInfo_s {
		int inuse; // 0x0
		int flags; // 0x4
		int fireTime; // 0x8
		vec2 arcmin; // 0xC
		vec2 arcmax; // 0x14
		float dropPitch; // 0x1C
		int stance; // 0x20
		int prevStance; // 0x24
		int fireSndDelay; // 0x28
		vec3 userOrigin; // 0x2C
		float playerSpread; // 0x38
		int state; // 0x3C
		EntHandle target; // 0x40
		vec3 targetOffset; // 0x44
		EntHandle manualTarget; // 0x50
		vec3 manualTargetOffset; // 0x54
		int targetTime; // 0x60
		int stateChangeTime; // 0x64
		int modeChangeTime; // 0x68
		float maxRangeSquared; // 0x6C
		int prevTargetIndex; // 0x70
		int eTeam; // 0x74
		int convergenceTime[2]; // 0x78
		float convergenceHeightPercent; // 0x80
		vec3 targetPos; // 0x84
		char padding[0x34]; // 0x88
	};
	
	struct item_ent_t {
		int ammoCount; // 0x0
		int clipAmmoCount[2]; // 0x4
		int weaponIndex; // 0xC
		bool dualWieldItem; // 0x10
	};
	
	struct trigger_ent_t {
		int threshold; // 0x0
		int accumulate; // 0x4
		int timestamp; // 0x8
		int singleUserEntIndex; // 0xC
		int damage; // 0x10
		bool requireLookAt; // 0x14
	};
	
	struct mover_ent_t {
		float decelTime; // 0x0
		float aDecelTime; // 0x4
		float speed; // 0x8
		float aSpeed; // 0xC
		float midTime; // 0x10
		float aMidTime; // 0x14
		float pos1[3]; // 0x18
		float pos2[3]; // 0x24
		float pos3[3]; // 0x30
		float apos1[3]; // 0x3C
		float apos2[3]; // 0x48
		float apos3[3]; // 0x54
	};
	
	struct corpse_ent_t {
		int deathAnimStartTime; // 0x0
	};
	
	struct VehicleTags {
		int player; // 0x0
		int detach; // 0x4
		int popout; // 0x8
		int body; // 0xC
		int turret; // 0x10
		int turret_base; // 0x14
		int barrel; // 0x18
		int flash[5]; // 0x1C
		int wheel[6]; // 0x34
	};
	
	struct scr_vehicle_s;

	struct tagInfo_s;
	
	struct gentity_s {
		entityState_s state; // 0x0
		enitityShared_t r; // 0xFC
		gclient_s* client; // 0x158
		char padding[4]; // 0x15C
		scr_vehicle_s* vehicle; // 0x160
		int physicsBody; // 0x164
		short modelId; // 0x168
		bool physicsObject; // 0x16A
		bool takedamage; // 0x16B
		char active; // 0x16C
		char handler; // 0x16D
		char team; // 0x16E
		bool freeAfterEvent; //0x16F
		char padding3[2]; // 0x170
		short classname; // 0x172
		short code_classname; // 0x174
		short target; // 0x178
		short targetname; // 0x17A
		int attachIgnoreCollision; // 0x17C
		int spawnFlags; // 0x180
		int flags; // 0x184
		int unlinkAfterEvent; // 0x188
		int clipMask; // 0x18C
		int processedFrame; // 0x190
		EntHandle parent; // 0x194
		int nextthink; // 0x198
		int health; // 0x19C
		int maxHealth; // 0x1A0
		int damage; // 0x1A4
		int count; // 0x1A8
		union {
			item_ent_t item[2];
			trigger_ent_t trigger;
			mover_ent_t mover;
			corpse_ent_t corpse;
		}; // 0x1AC
		EntHandle projEntity; // 0x20C
		EntHandle linkedEntity; // 0x210
		tagInfo_s* tagInfo; // 0x214
		gentity_s* tagChildren; // 0x218
		short attachModelNames[19]; // 0x21C
		short attachTagNames[19]; // 0x242
		int useCount; // 0x268
		int nextFree; // 0x26C
		int birthTime; // 0x270
		char padding4[10]; // 0x274
	};
	
	struct scr_vehicle_s {
		gentity_s* ent; // 0x0
		LerpEntityStateVehicle* lerp; // 0x4
		vehicleDrivingState_t drivingState; // 0x8
		char padding[322]; // 0xC
		EntHandle vehEnt; // 0x150
		char padding2[34]; // 0x154
		float maxPitchAngle; // 0x178
		float maxRollAngle; // 0x17C
		char padding3[176]; // 0x180
		int defIndex; // 0x230
		int team; // 0x234
		unsigned int useHintStringIndex; // 0x238
		helicopterMoveState_t heliMoveState; // 0x23C
		float speed; // 0x240
		char padding4[136]; // 0x244
		short lookAtText0; // 0x2CC
		short lookAtText1; // 0x2CE
		vehicleManualMode_t manualMode; // 0x2D0
		float manualSpeed; // 0x2D4
		float manualAccel; // 0x2D8
		float manualDecel; // 0x2DC
		float manualTime; // 0x2E0
		char padding5[16]; // 0x2E4
		float turningAbility; // 0x2F4
		int hasTarget; // 0x2F8
		char padding7[8]; // 0x2F0
		int stopAtGoal; // 0x304
		int stopping; // 0x308
		int targetEnt; // 0x30C
		EntHandle lookAtEnt; // 0x310
		char padding8[28]; // 0x314
		vec3 goalPosition; // 0x330
		float yawOverShoot; // 0x344
		char padding9[50]; // 0x348
		VehicleTags boneIndex; // 0x374
	
		// Vehicle functions
		int getVehicleServerDef();
	
		float getSpeed();
		float getGoalSpeedMph();
	
		void fireWeapon();
		void setMaxPitchRoll(float pitch, float roll);
		void setTurningAbility(float turn);
		void setJitterParams(vec3 range, float minperiod, float maxperiod);
		void setYawSpeed(float speed, float accel, float decel, float overshootPercent);
		void setWeapon(char* weaponName);
		void setGoalPos(vec3 origin, bool slowDownNearGoal);
		void setSpeed(float speed, float accel, float decel);
	};
	
	struct tagInfo_s {
		gentity_s* parent; // 0x0
		gentity_s* next; // 0x4
		short name; // 0x8
		bool blendToParent; // 0xA
		int index; // 0xC
		float axis[4][3]; // 0x10
		float parentInvAxis[4][3]; // 0x40
	};
	
	struct objective_t {
		objectiveState_t state; // 0x0
		vec3 origin; // 0x4
		int entNum; // 0x10
		int teamNum; // 0x14
		int icon; // 0x18
	};
	
	struct cached_tag_mat_t {
		int time; // 0x0
		int entnum; // 0x4
		short name; // 0x8
		float tagMat[4][3]; // 0xC
	};
	
	struct SpawnVar {
		bool spawnVarsValid; // 0x0
		int numSpawnVars; // 0x4
		char* spawnVars[64][2]; // 0x8
		int numSpawnVarChars; // 0x208
		char spawnVarChars[2048]; // 0x20C
	};
	
	struct level_locals_t {
		gclient_s* client; // 0x0
		gentity_s* entity; // 0x4
		int num_entities; // 0x8
		gentity_s* firstFreeEnt; // 0xC
		gentity_s* lastFreeEnt; // 0x10
		turretInfo_s* turret; // 0x14
		int iSearchFrame; // 0x18
		int initializing; // 0x1C
		int clientIsSpawning; // 0x20
		objective_t objectives[32]; // 0x24
		int maxclients; // 0x3A4
		int framenum; // 0x3A8
		int time; // 0x3AC
		int previousTime; // 0x3B0
		int frametime; // 0x3B4
		int startTime; // 0x3B8
		int teamScores[4]; // 0x3BC
		int lastTeammateHealthTime; // 0x3CC
		int bUpdateScoresForIntermission; // 0x3D0
		bool teamHasRadar[4]; // 0x3D4
		int pingUpdateTime; // 0x3D8
		int manualNameChange; // 0x3DC
		int numConnectedClients; //0x3E0
		int sortedClients[18]; // 0x3E4
		SpawnVar spawnVar; // 0x42C
		int savepersist; // 0xE38
		int droppedWeaponCue[32]; // 0xE3C
		float fFogOpaqueDist; // 0xEBC
		float fFogOpaqueDistSqrd; // 0xEC0
		int currentPlayerClone; // 0xEC4
		char padding[0x183C]; // 0xEC8
		cached_tag_mat_t cachedTagMat; // 0x2704
		int scriptPrintChannel; // 0x2740
		vec2 compassMapUpperLeft; // 0x2744
		vec2 compassMapWorldSize; // 0x274C
		vec2 compassNorth; // 0x2754
		char pading2[0xFA4]; // 0x275C
	};
	
	struct server_t {
		serverState_t state; // 0x0
		int timeResidual; // 0x4
		char padding[0x81A]; // 0x8
		short configStrings[0x3F8]; // 0x822
	};
	
	struct client_s {
		int state; // 0x0
		int sendAsActive; // 0x4
		char padding[0x644]; // 0x8
		char* dropReason; // 0x64C
		char lastClientCommandString[1024]; // 0x650
		char padding2[0x848]; // 0xA50
		char name[32]; // 0x1298
		char padding3[0x30830]; // 0x16B4
		char playerGuid[17]; // 0x31AE8
		short scriptId; // 0x31AE6
		int testClient; // 0x31AFC
	};
}