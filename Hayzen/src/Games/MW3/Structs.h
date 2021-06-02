#pragma once

namespace MW3
{
	using XexUtils::Math::vec2;
	using XexUtils::Math::vec3;
	
	enum he_type_t : int
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

	typedef enum align_t : int
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
		byte r;
		byte g;
		byte b;
		byte a;
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
		int fontScaleStartTime;
		int fontScaleTime;
		int font;
		align_t alignOrg;
		align_t alignScreen;
		hudelem_color_t color;
		hudelem_color_t fromColor;
		int fadeStartTime;
		int fadeTime;
		int label;
		int width;
		int height;
		int materialIndex;
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
		hudelem_s elem;
		int clientNum;
		int teamNum;
		int archived;
	};

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
		byte mFlags;
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
		gclient_s* client;
		char padding1[0x28];
		int flags;
	};
}