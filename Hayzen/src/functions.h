#include "pch.h"
#include <stdio.h>

char Buffer[0x1000];

void(_cdecl *SV)(int clientNum, int type, const char *text) = (void(*)(int, int, const char*))0x822548D8;//

bool(_cdecl *Dvar_GetBool)(const char* DvarName) = (bool(*)(const char*))0x8229EEE8;//

const char* (_cdecl *Dvar_GetString)(const char* Dvar) = (const char*(*)(const char*))0x8229F0A8;//

float(_cdecl *Dvar_GetFloat)(const char *Dvar) = (float(*)(const char*))0x8229F028;

int(_cdecl *Dvar_GetInt)(const char *Dvar) = (int(*)(const char*))0x8229EF50;//

unsigned long(_cdecl *getClientState)(int clientNum) = (unsigned long(*)(int))0x821E6610;//

void(_cdecl *SV_Cmd_TokenizeString)(const char *text_in) = (void(*)(const char*))0x822252B0;//

void(_cdecl *ClientCommand)(unsigned long clientIndex) = (void(*)(unsigned long))0x821DC5A0;//

void(_cdecl *SV_Cmd_EndTokenizedString)() = (void(*)())0x822252D0;//

const char * (_cdecl *G_SoundAliasIndex)(const char *name) = (const char*(*)(const char*))0x8220CFB0;//

bool compareString(const char *Str1, const char *Str2)
{
	int Result = 0;
	while (!(Result = *(unsigned char *) Str1 - *(unsigned char *) Str2) && *Str2) ++Str1, ++Str2;
	if (Result == 0)
		return true;
	else
		return false;
}

char* LinkChar(const char* Text, ...)
{
	va_list pArgList;
    va_start( pArgList, Text );
    vsprintf( Buffer, Text, pArgList );
	va_end(pArgList);
	return Buffer;
} 

void PatchInJump(DWORD* Address, DWORD Destination, BOOL Linked)
{
    if(Destination & 0x8000)
		Address[0] = 0x3D600000 + (((Destination >> 16) & 0xFFFF) + 1);
    else
		Address[0] = 0x3D600000 + ((Destination >> 16) & 0xFFFF);

    Address[1] = 0x396B0000 + (Destination & 0xFFFF);
    Address[2] = 0x7D6903A6;

    if(Linked)
		Address[3] = 0x4E800421;
    else
		Address[3] = 0x4E800420;
}

struct XenonUserData
{
        int signInState;
        char name[0x20];
};

int _Host()
{
	int TempHost;
	char Temp[18][15];
	XenonUserData *xenonUserData = (XenonUserData *)0x838BA820;
	for (int ID = 0; ID < 18; ID++)
	{
		strcpy(Temp[ID], (char*)getClientState(ID) + 0x44);
		if (compareString(xenonUserData[0].name, Temp[ID]))
			TempHost = ID;
	}
	return TempHost;
}

int getPlayerState(int client)
{
	return (0x830CBF80 + (client * 280));
}

bool KeyDown(const char* s, const void* Button)
{
	if (!memcmp(s, Button, 4))
		return true;
	else
		return false;
}

void DebugPrint(const char* Text, ...)
{
	va_list Temp;
	va_start(Temp, Text);
	vsprintf(Buffer, Text, Temp);
	va_end(Temp);
	printf("[Menu Base]: %s\n", Buffer);
}