#pragma once

namespace Alpha
{
namespace MW2
{
	using Math::vec2;
	using Math::vec3;

	struct Font_s
	{
		int fontName;
		int pixelHeight;
		int glyphCount;
		int material;
		int glowMaterial;
		int glyphs;
	};

	struct playerState_s
	{
		char padding1[0x1C];
		vec3 origin;
		char padding2[0xE4];
		vec3 viewAngles;
		char padding3[0x3068];
	};

	struct gclient_s
	{
		playerState_s ps;
		char padding1[0x580];
	};
	
	struct entityState_s
	{
		int number;
		char padding1[0x8C];
		int index;
		char padding2[0x68];
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
		char padding2[0xF8];
	};
}
}