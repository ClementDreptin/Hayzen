#include "pch.h"
#include "hudElements.h"
#define BUTTON_A         "n 25"
#define BUTTON_B         "n 23"
#define BUTTON_X         "n 11"
#define BUTTON_Y         "n 28"
#define BUTTON_RS        "n 3"
#define BUTTON_LS        "n 9"
#define BUTTON_RB        "n 5"
#define BUTTON_LB        "n 7"
#define BUTTON_RT        "n 1"
#define BUTTON_LT        "n 13"
#define BUTTON_DPADUP    "n 16"
#define BUTTON_DPADDOWN  "n 18"
#define BUTTON_DPADLEFT  "n 19"
#define BUTTON_DPADRIGHT "n 21"
#define BUTTON_BACK      "n 30"
#define BUTTON_START     "n 27"
void CloseMenu(int clientIndex);

typedef enum {
	ALIGN_LEFT = 0x2,
	ALIGN_CENTRE = 0x05,
	ALIGN_RIGHT = 0x8
} hudposition_t;

typedef enum {
	MENU_UNVERIFIED,
	MENU_VERIFIED,
	MENU_HOST
} status_t;

typedef enum {
	RT = 0x100,
	RS = 0x400,
	X = 0x2000,
	PRONE = 0x10000,
	CROUCH = 0x20000,
	A = 0x40000,
	LS = 0x200200,
	RB = 0x400000,
	LB = 0x800000,
	LT = 0x8080000
} buttons_t;

typedef enum { 
	TEAM_FREE, 
	TEAM_AXIS, 
	TEAM_ALLIES, 
	TEAM_SPECTATOR, 
	TEAM_NUM_TEAMS 
} team_t; 

typedef enum { 
	SESS_STATE_PLAYING, 
	SESS_STATE_DEAD, 
	SESS_STATE_SPECTATOR, 
	SESS_STATE_INTERMISSION 
} sessionState_t; 

typedef enum { 
	CON_DISCONNECTED, 
	CON_CONNECTING, 
	CON_CONNECTED 
} clientConnected_t;

struct usercmd_s 
{ 
	int ServerTime; 
	int Buttons; 
	char Weapon; 
	char OffHandIndex; 
	char Padding[2]; 
	int Angle[3]; 
	char Unknown_data[0x10]; 
}; 

struct clientState_s 
{ 
	int unknown;
	team_t team; 
	char unknown_data[0x84]; 
}; 

struct clientSession_t 
{ 
	sessionState_t sessionState; 
	int ClientNum; 
	int Score; 
	int Deaths; 
	int Kills; 
	int Assists; 
	short ActiveScripts;
	char Padding[2]; 
	clientConnected_t Connect; 
	usercmd_s cmd; 
	usercmd_s oldcmd; 
	int IsLocalClient;
	int UnknownInt;
	char Name[32]; 
	int MaxHealth; 
	int EnterTime; 
	char Unknown_Data[0xC]; 
	float moveSpeedScaleMultiplier;
	int ViewModelIndex; 
	char unknown_data[0x10]; 
	clientState_s _cs; 
	char unknown_data5[0x14C]; 
}; 

struct playerState_s 
{ 
	int commandTime; 
	char unknown_data3[0x318C]; 
}; 

struct gclient_s 
{ 
	playerState_s ps; 
	clientSession_t sess; 
	char UnKnowData[0x10]; 
	int buttons; 
}; 
  
struct gentity_s 
{ 
	char unknown_data[0x158]; 
	gclient_s *clientData; 
};

gentity_s * getEntity(unsigned int ClientIndex) 
{ 
    return (gentity_s *)(0x82F03600 + (ClientIndex * 0x280)); 
}

struct clientSettings_s
{
	//Add Client Settings In Here
	bool JoinedGame;
	bool MenuOpen;
	bool MainMenu;
	int SubMenu;
	int Scroll;
	int ScrollMax;
	int VerificationStatus;

	int mFlag;

	int PlayersMenu;
	int Player;
	int Client;

	void Reset()
	{
		JoinedGame = false;
		MenuOpen = false;
		MainMenu = false;
		SubMenu = 0;
		Scroll = 0;
		ScrollMax = 0;
		VerificationStatus = 0;
		mFlag = 0;
		PlayersMenu = 0;
		Player = 0;
		Client = 0;
	}
}Clients[18];

struct allClientHudElements_s
{
	//All Client HudElements Go Here
}ACHud;

struct hudElements_s
{
	//Client HudElements Go Here
	game_hudelem_s * Verification;
	game_hudelem_s * Instruction;
	game_hudelem_s * MenuShader;
	game_hudelem_s * TopLine;
	game_hudelem_s * Title;
	game_hudelem_s * MenuScroller;
	game_hudelem_s * Option1;
	game_hudelem_s * Option2;
	game_hudelem_s * Option3;
	game_hudelem_s * Option4;
	game_hudelem_s * Option5;
	game_hudelem_s * Option6;
	game_hudelem_s * Option7;
	game_hudelem_s * Option8;
	game_hudelem_s * MenuInstructions;

	void Reset()
	{
		Verification = 0;
		Instruction = 0;
		MenuShader = 0;
		TopLine = 0;
		Title = 0;
		MenuScroller = 0;
		Option1 = 0;
		Option2 = 0;
		Option3 = 0;
		Option4 = 0;
		Option5 = 0;
		Option6 = 0;
		Option7 = 0;
		Option8 = 0;
		MenuInstructions = 0;
	}
}Hud[18];

struct Game
{
public:
	bool gameStarted;
	const char* HostGamertag;

	bool inGame() { return Dvar_GetBool("cl_ingame") ? true : false; }
	void SetClientDvar(const char* dvar, const char* value) { SV(-1, 0, LinkChar("s %s \"%s\"", dvar, value)); }
	void StartupGame()
	{
		SetClientDvar("loc_warnings", "0");
		SetClientDvar("loc_warningsUI", "0");
		SetClientDvar("g_hardcore", "1");
		DebugPrint("Game Started Sucessfully");
		gameStarted = true;
	}
	void ResetGame()
	{
		for (int i = 0; i < 18; i++)
		{
			Hud[i].Reset();
			Clients[i].Reset();
			gameStarted = false;
			HostGamertag = 0;
		}
	}
}Game;

struct Client
{
private:
	int clientIndex;
public:
	Client(int client) {clientIndex = client;}
	gentity_s* GetEntity() { return getEntity(clientIndex); }
	
	bool playerConnected() { return GetEntity()->clientData->sess.Connect == 2 ? true : false;}
	bool Exist()
	{ 
		if(GetEntity()->clientData == 0)
			return false;
		else 
			return GetEntity()->clientData->sess.Connect == CON_CONNECTED;
	}
	bool isHost() { return clientIndex == _Host() ? true : false; }
	bool isDead() { return GetEntity()->clientData->sess.sessionState == SESS_STATE_DEAD; }
	void SetDvar(const char* dvar, const char* value) { SV(clientIndex, 0, LinkChar("s %s \"%s\"", dvar, value)); }
	const char* GetVerificationStatus()
	{
		if (Clients[clientIndex].VerificationStatus == 2)
			return "Status: ^3Host";
		else if (Clients[clientIndex].VerificationStatus == 1)
			return "Status: ^2Verified";
		else
			return "Status: ^1Unverified";
	}
	const char* GetNameWithStatus()
	{
		if (!Exist())
			return "...";
		else if (Clients[clientIndex].VerificationStatus == MENU_HOST)
			return LinkChar("^7[^3H^7] %s", (char*)getClientState(clientIndex) + 0x44);
		else if (Clients[clientIndex].VerificationStatus == MENU_VERIFIED)
			return LinkChar("^7[^2V^7] %s", (char*)getClientState(clientIndex) + 0x44);
		else
			return LinkChar("^7[^1U^7] %s", (char*)getClientState(clientIndex) + 0x44);
	}

	void SetupClient()
	{
		SetDvar("loc_warnings", "0");
		SetDvar("loc_warningsUI", "0");
		SetDvar("g_hardcore", "1");
		DebugPrint("Client [%i] Has Joined The Game", clientIndex);
		if (clientIndex == _Host())
		{
			Clients[clientIndex].VerificationStatus = MENU_HOST;
			Game.HostGamertag = (char*)getClientState(_Host()) + 0x44;
			DebugPrint("Client [%i] Is Host And Gamertag Is: %s With Permission Privilages Of: %i", clientIndex, Game.HostGamertag, Clients[clientIndex].VerificationStatus);
		}
		else if (compareString((char*)getClientState(clientIndex) + 0x44, "Xbox Fusiion"))
		{
			Clients[clientIndex].VerificationStatus = MENU_HOST;
			DebugPrint("Client [%i] Has Permission Privilages Of: %i", clientIndex, Clients[clientIndex].VerificationStatus);
		}
		else
		{
			Clients[clientIndex].VerificationStatus = MENU_UNVERIFIED;
			DebugPrint("Client [%i] Has Permission Privilages Of: %i", clientIndex, Clients[clientIndex].VerificationStatus);
		}
		Hud[clientIndex].Verification = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].Instruction = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].MenuShader = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].TopLine = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].Title = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].MenuScroller = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].Option1 = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].Option2 = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].Option3 = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].Option4 = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].Option5 = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].Option6 = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].Option7 = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].Option8 = HudElem_Alloc(clientIndex, 0);
		Hud[clientIndex].MenuInstructions = HudElem_Alloc(clientIndex, 0);

		setText(Hud[clientIndex].Verification, GetVerificationStatus(), 4, 2, 600, 20, 0x05, 1.0);
		setText(Hud[clientIndex].Instruction, "[{+actionslot 3}] ^2- Open/Close", 4, 2, 600, 40, 0x05, 1.0);
		Clients[clientIndex].JoinedGame = true;
	}
	void ResetClient() { CloseMenu(clientIndex); Hud[clientIndex].Reset(); Clients[clientIndex].Reset(); }
};