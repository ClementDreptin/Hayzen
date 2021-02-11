#include "structs.h"

void OpenMenu(int clientIndex)
{
	Clients[clientIndex].MenuOpen = true;
	Clients[clientIndex].MainMenu = true;
	Clients[clientIndex].SubMenu = 0;
	Clients[clientIndex].Scroll = 1;
	Clients[clientIndex].PlayersMenu = 0;
	Clients[clientIndex].Player = 0;
	Clients[clientIndex].Client = Clients[clientIndex].Player - 1;

	*(int*)(getPlayerState(clientIndex) + 0x3420) = 4;

	setShader(Hud[clientIndex].MenuShader, "black", 320, 200, 0, 0, 0x05, 0.0, 0, 0, 0, 210);
	scaleOverTime(Hud[clientIndex].MenuShader, 350, 300, 500);
	setShader(Hud[clientIndex].MenuScroller, "white", 1000, 80, 285, 30, 0x05, 0.1, 255, 0, 0, 255);
	MoveOverTime(Hud[clientIndex].MenuScroller, 350, 320, 80, 350);
	setShader(Hud[clientIndex].TopLine, "white", -1000, 50, 300, 5, 0x05, 0.1, 255, 255, 255, 255);
	MoveOverTime(Hud[clientIndex].TopLine, 350, 320, 50, 350);

	setText(Hud[clientIndex].Title, "Main Menu", 4, 3, 320, 20, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Title, 350, 255, 255, 255, 255, 700);
	setText(Hud[clientIndex].Option1, "Sub Menu 1", 4, 2, 320, 80, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option1, 350, 255, 255, 255, 255, 700);
	setText(Hud[clientIndex].Option2, "Sub Menu 2", 4, 2, 320, 110, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option2, 350, 255, 255, 255, 255, 700);
	setText(Hud[clientIndex].Option3, "Sub Menu 3", 4, 2, 320, 140, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option3, 350, 255, 255, 255, 255, 700);
	setText(Hud[clientIndex].Option4, "Sub Menu 4", 4, 2, 320, 170, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option4, 350, 255, 255, 255, 255, 700);
	setText(Hud[clientIndex].Option5, "Sub Menu 5", 4, 2, 320, 200, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option5, 350, 255, 255, 255, 255, 700);
	setText(Hud[clientIndex].Option6, "Sub Menu 6", 4, 2, 320, 230, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option6, 350, 255, 255, 255, 255, 700);
	setText(Hud[clientIndex].MenuInstructions, "[{+gostand}] - Select --- [{+stance}] - Back --- [{+actionslot 1}] AND [{+actionslot 2}] - NAVIGATE", 4, 1.1, 320, 400, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].MenuInstructions, 350, 255, 255, 255, 255, 700);
	if (Clients[clientIndex].VerificationStatus == MENU_HOST)
	{
		Clients[clientIndex].ScrollMax = 8;
		setText(Hud[clientIndex].Option7, "^5Player Menu", 4, 2, 320, 260, 0x05, 1.0, 0, 0, 0, 0);
		setText(Hud[clientIndex].Option8, "^3Host Mods", 4, 2, 320, 290, 0x05, 1.0, 0, 0, 0, 0);
	}
	else
	{
		Clients[clientIndex].ScrollMax = 6;
		setText(Hud[clientIndex].Option7, "^1LOCKED!!!", 4, 2, 320, 260, 0x05, 1.0, 0, 0, 0, 0);
		setText(Hud[clientIndex].Option8, "^1LOCKED!!!", 4, 2, 320, 290, 0x05, 1.0, 0, 0, 0, 0);
	}
	FadeOverTime(Hud[clientIndex].Option7, 350, 255, 255, 255, 255, 700);
	FadeOverTime(Hud[clientIndex].Option8, 350, 255, 255, 255, 255, 700);
}

void CloseMenu(int clientIndex)
{
	Clients[clientIndex].MenuOpen = false;
	Clients[clientIndex].MainMenu = false;
	Clients[clientIndex].SubMenu = 0;
	Clients[clientIndex].Scroll = 0;
	Clients[clientIndex].ScrollMax = 0;
	Clients[clientIndex].PlayersMenu = 0;
	Clients[clientIndex].Player = 0;
	Clients[clientIndex].Client = Clients[clientIndex].Player - 1;

	*(int*)(getPlayerState(clientIndex) + 0x3420) = Clients[clientIndex].mFlag;

	FadeOverTime(Hud[clientIndex].MenuShader, 150, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].MenuScroller, 150, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].TopLine, 150, 0, 0, 0, 0);

	FadeOverTime(Hud[clientIndex].Title, 150, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option1, 150, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option2, 150, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option3, 150, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option4, 150, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option5, 150, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option6, 150, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option7, 150, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option8, 150, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].MenuInstructions, 150, 0, 0, 0, 0);
}

void OpenSub(int clientIndex, int Option)
{
	Clients[clientIndex].MainMenu = false;
	Clients[clientIndex].SubMenu = Option;
	Clients[clientIndex].Scroll = 1;
	Clients[clientIndex].PlayersMenu = 0;
	Clients[clientIndex].Client = 0;
	Clients[clientIndex].Player = 0;
	MoveOverTime(Hud[clientIndex].MenuScroller, 300, 320, (float)((30 * Clients[clientIndex].Scroll) + 50));
	switch (Option)
	{
		case 1:
			ChangeOptionText(Hud[clientIndex].Title, "Sub Menu 1");
			ChangeOptionText(Hud[clientIndex].Option1, "Option 1");
			ChangeOptionText(Hud[clientIndex].Option2, "Option 2");
			ChangeOptionText(Hud[clientIndex].Option3, "Option 3");
			ChangeOptionText(Hud[clientIndex].Option4, "Option 4");
			ChangeOptionText(Hud[clientIndex].Option5, "Option 5");
			ChangeOptionText(Hud[clientIndex].Option6, "Option 6");
			ChangeOptionText(Hud[clientIndex].Option7, "Option 7");
			ChangeOptionText(Hud[clientIndex].Option8, "Option 8");
			Clients[clientIndex].ScrollMax = 8;
		break;
		case 2:
			ChangeOptionText(Hud[clientIndex].Title, "Sub Menu 2");
			ChangeOptionText(Hud[clientIndex].Option1, "Option 1");
			ChangeOptionText(Hud[clientIndex].Option2, "Option 2");
			ChangeOptionText(Hud[clientIndex].Option3, "Option 3");
			ChangeOptionText(Hud[clientIndex].Option4, "Option 4");
			ChangeOptionText(Hud[clientIndex].Option5, "Option 5");
			ChangeOptionText(Hud[clientIndex].Option6, "Option 6");
			ChangeOptionText(Hud[clientIndex].Option7, "Option 7");
			ChangeOptionText(Hud[clientIndex].Option8, "Option 8");
			Clients[clientIndex].ScrollMax = 8;
		break;
		case 3:
			ChangeOptionText(Hud[clientIndex].Title, "Sub Menu 3");
			ChangeOptionText(Hud[clientIndex].Option1, "Option 1");
			ChangeOptionText(Hud[clientIndex].Option2, "Option 2");
			ChangeOptionText(Hud[clientIndex].Option3, "Option 3");
			ChangeOptionText(Hud[clientIndex].Option4, "Option 4");
			ChangeOptionText(Hud[clientIndex].Option5, "Option 5");
			ChangeOptionText(Hud[clientIndex].Option6, "Option 6");
			ChangeOptionText(Hud[clientIndex].Option7, "Option 7");
			ChangeOptionText(Hud[clientIndex].Option8, "Option 8");
			Clients[clientIndex].ScrollMax = 8;
		break;
		case 4:
			ChangeOptionText(Hud[clientIndex].Title, "Sub Menu 4");
			ChangeOptionText(Hud[clientIndex].Option1, "Option 1");
			ChangeOptionText(Hud[clientIndex].Option2, "Option 2");
			ChangeOptionText(Hud[clientIndex].Option3, "Option 3");
			ChangeOptionText(Hud[clientIndex].Option4, "Option 4");
			ChangeOptionText(Hud[clientIndex].Option5, "Option 5");
			ChangeOptionText(Hud[clientIndex].Option6, "Option 6");
			ChangeOptionText(Hud[clientIndex].Option7, "Option 7");
			ChangeOptionText(Hud[clientIndex].Option8, "Option 8");
			Clients[clientIndex].ScrollMax = 8;
		break;
		case 5:
			ChangeOptionText(Hud[clientIndex].Title, "Sub Menu 5");
			ChangeOptionText(Hud[clientIndex].Option1, "Option 1");
			ChangeOptionText(Hud[clientIndex].Option2, "Option 2");
			ChangeOptionText(Hud[clientIndex].Option3, "Option 3");
			ChangeOptionText(Hud[clientIndex].Option4, "Option 4");
			ChangeOptionText(Hud[clientIndex].Option5, "Option 5");
			ChangeOptionText(Hud[clientIndex].Option6, "Option 6");
			ChangeOptionText(Hud[clientIndex].Option7, "Option 7");
			ChangeOptionText(Hud[clientIndex].Option8, "Option 8");
			Clients[clientIndex].ScrollMax = 8;
		break;
		case 6:
			ChangeOptionText(Hud[clientIndex].Title, "Sub Menu 6");
			ChangeOptionText(Hud[clientIndex].Option1, "Option 1");
			ChangeOptionText(Hud[clientIndex].Option2, "Option 2");
			ChangeOptionText(Hud[clientIndex].Option3, "Option 3");
			ChangeOptionText(Hud[clientIndex].Option4, "Option 4");
			ChangeOptionText(Hud[clientIndex].Option5, "Option 5");
			ChangeOptionText(Hud[clientIndex].Option6, "Option 6");
			ChangeOptionText(Hud[clientIndex].Option7, "Option 7");
			ChangeOptionText(Hud[clientIndex].Option8, "Option 8");
			Clients[clientIndex].ScrollMax = 8;
		break;
		case 7:
			ChangeOptionText(Hud[clientIndex].Title, "Player Menu");
			ChangeOptionText(Hud[clientIndex].Option1, "Players (0-5)");
			ChangeOptionText(Hud[clientIndex].Option2, "Players (6-11)");
			ChangeOptionText(Hud[clientIndex].Option3, "Players (12-17)");
			ChangeOptionText(Hud[clientIndex].Option4, "");
			ChangeOptionText(Hud[clientIndex].Option5, "");
			ChangeOptionText(Hud[clientIndex].Option6, "");
			ChangeOptionText(Hud[clientIndex].Option7, "");
			ChangeOptionText(Hud[clientIndex].Option8, "");
			Clients[clientIndex].ScrollMax = 3;
		break;
		case 8:
			ChangeOptionText(Hud[clientIndex].Title, "Host Menu");
			ChangeOptionText(Hud[clientIndex].Option1, "Option 1");
			ChangeOptionText(Hud[clientIndex].Option2, "Option 2");
			ChangeOptionText(Hud[clientIndex].Option3, "Option 3");
			ChangeOptionText(Hud[clientIndex].Option4, "Option 4");
			ChangeOptionText(Hud[clientIndex].Option5, "Option 5");
			ChangeOptionText(Hud[clientIndex].Option6, "Option 6");
			ChangeOptionText(Hud[clientIndex].Option7, "Option 7");
			ChangeOptionText(Hud[clientIndex].Option8, "Option 8");
			Clients[clientIndex].ScrollMax = 8;
		break;
	}
}

void SubMenus(int clientIndex, int Sub, int Option)
{
	switch (Sub)
	{
		case 1:
			switch (Option)
			{
				case 1:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 2:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 3:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 4:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 5:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 6:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 7:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 8:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
			}
		break;
		case 2:
			switch (Option)
			{
				case 1:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 2:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 3:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 4:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 5:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 6:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 7:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 8:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
			}
		break;
		case 3:
			switch (Option)
			{
				case 1:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 2:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 3:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 4:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 5:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 6:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 7:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 8:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
			}
		break;
		case 4:
			switch (Option)
			{
				case 1:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 2:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 3:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 4:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 5:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 6:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 7:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 8:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
			}
		break;
		case 5:
			switch (Option)
			{
				case 1:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 2:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 3:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 4:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 5:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 6:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 7:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 8:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
			}
		break;
		case 6:
			switch (Option)
			{
				case 1:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 2:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 3:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 4:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 5:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 6:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 7:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 8:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
			}
		break;
		case 7:
			Clients[clientIndex].PlayersMenu = Option;
			Clients[clientIndex].Scroll = 1;
			Clients[clientIndex].ScrollMax = 6;
			Clients[clientIndex].SubMenu = 0;
			Clients[clientIndex].Player = 0;
			Clients[clientIndex].Client = 0;
			MoveOverTime(Hud[clientIndex].MenuScroller, 300, 320, (float)((30 * Clients[clientIndex].Scroll) + 50));
			switch (Option)
			{
				case 1:
					ChangeOptionText(Hud[clientIndex].Title, "Players Menu (0-5)");
					ChangeOptionText(Hud[clientIndex].Option1, Client(0).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option2, Client(1).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option3, Client(2).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option4, Client(3).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option5, Client(4).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option6, Client(5).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option7, "");
					ChangeOptionText(Hud[clientIndex].Option8, "");
				break;
				case 2:
					ChangeOptionText(Hud[clientIndex].Title, "Players Menu (6-11)");
					ChangeOptionText(Hud[clientIndex].Option1, Client(6).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option2, Client(7).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option3, Client(8).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option4, Client(9).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option5, Client(10).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option6, Client(11).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option7, "");
					ChangeOptionText(Hud[clientIndex].Option8, "");
				break;
				case 3:
					ChangeOptionText(Hud[clientIndex].Title, "Players Menu (12-17)");
					ChangeOptionText(Hud[clientIndex].Option1, Client(12).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option2, Client(13).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option3, Client(14).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option4, Client(15).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option5, Client(16).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option6, Client(17).GetNameWithStatus());
					ChangeOptionText(Hud[clientIndex].Option7, "");
					ChangeOptionText(Hud[clientIndex].Option8, "");
				break;
			}
		break;
		case 8:
			switch (Option)
			{
				case 1:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 2:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 3:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 4:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 5:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 6:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 7:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
				case 8:
					SV(clientIndex, 0, LinkChar("f \"You Have Selected Sub Menu: %i, Option %i\"", Sub, Option));
				break;
			}
		break;
	}
}

void ViewPlayer(int clientIndex, int Menu, int Option)
{
	Clients[clientIndex].Scroll = 1;
	Clients[clientIndex].ScrollMax = 5;
	Clients[clientIndex].PlayersMenu = 0;
	Clients[clientIndex].Player = Option;
	MoveOverTime(Hud[clientIndex].MenuScroller, 300, 320, (float)((30 * Clients[clientIndex].Scroll) + 50));
	switch (Menu)
	{
		case 1:
			Clients[clientIndex].Client = Clients[clientIndex].Player - 1;
		break;
		case 2:
			Clients[clientIndex].Client = Clients[clientIndex].Player + 5;
		break;
		case 3:
			Clients[clientIndex].Client = Clients[clientIndex].Player + 11;
		break;
	}
	setText(Hud[clientIndex].Title, (char*)getClientState(Clients[clientIndex].Client) + 0x44, 4, 3, 320, 20, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Title, 350, 255, 255, 255, 255);
	setText(Hud[clientIndex].Option1, "Set Verified", 4, 2, 320, 80, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option1, 350, 255, 255, 255, 255);
	setText(Hud[clientIndex].Option2, "Set Unverified", 4, 2, 320, 110, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option2, 350, 255, 255, 255, 255);
	setText(Hud[clientIndex].Option3, "Give Level 70", 4, 2, 320, 140, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option3, 350, 255, 255, 255, 255);
	setText(Hud[clientIndex].Option4, "Derank", 4, 2, 320, 170, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option4, 350, 255, 255, 255, 255);
	setText(Hud[clientIndex].Option5, "Kick", 4, 2, 320, 200, 0x05, 1.0, 0, 0, 0, 0);
	FadeOverTime(Hud[clientIndex].Option5, 350, 255, 255, 255, 255);
	FadeOverTime(Hud[clientIndex].Option6, 350, 0, 0, 0, 0);
}

void EditPlayer(int clientIndex, int you, int Option)
{
	switch (Option)
	{
		case 1:
			if (Client(clientIndex).isHost())
				SV(you, 0, "f \"Cannot Demote The Host\"");
			else if (clientIndex == you)
				SV(you, 0, "f \"Cannot Demote Yourself\"");
			else
			{
				CloseMenu(clientIndex);
				Clients[clientIndex].VerificationStatus = MENU_VERIFIED;
				Hud[clientIndex].Verification->elem.text = G_LocalizedStringIndex(Client(clientIndex).GetVerificationStatus());
			}
		break;
		case 2:
			if (Client(clientIndex).isHost())
				SV(you, 0, "f \"Cannot Demote The Host\"");
			else if (clientIndex == you)
				SV(you, 0, "f \"Cannot Demote Yourself\"");
			else
			{
				CloseMenu(clientIndex);
				Clients[clientIndex].VerificationStatus = MENU_UNVERIFIED;
				Hud[clientIndex].Verification->elem.text = G_LocalizedStringIndex(Client(clientIndex).GetVerificationStatus());
			}
		break;
		case 3:
			SV(clientIndex, 1, "l mp_level_up");
			SV(clientIndex, 0, "J 2056 206426");
			SV(clientIndex, 0, "f \"^2You Are Now ^5Level 70\"");
		break;
		case 4:
			if (Client(clientIndex).isHost())
				SV(you, 0, "f \"Cannot Derank The Host\"");
			else if (clientIndex == you)
				SV(you, 0, "f \"Cannot Derank Yourself\"");
			else
			{
				Client(clientIndex).SetDvar("activeaction", "defaultstatsinit;disconnect");
				SV(you, 0, LinkChar("c \"Derank Set For %s\"", (char*)getClientState(clientIndex) + 0x44));
			}
		break;
		case 5:
			if (Client(clientIndex).isHost())
				SV(you, 0, "f \"Cannot Kick The Host\"");
			else if (clientIndex == you)
				SV(you, 0, "f \"Cannot Kick Yourself\"");
			else
			{
				SV(clientIndex, 0, "t \"Kicked From The Game Bitch\"");
			}
		break;
	}
}

void MonitorConnectionState(int clientIndex, const char* s)
{
	if (Client(clientIndex).playerConnected() && Clients[clientIndex].JoinedGame == false)
		Client(clientIndex).SetupClient();

	if (strstr(s, "disconnect"))
		Client(clientIndex).ResetClient();

	if (Clients[clientIndex].VerificationStatus > MENU_UNVERIFIED && KeyDown(s, BUTTON_DPADLEFT))
	{
		if (!Clients[clientIndex].MenuOpen)
			OpenMenu(clientIndex);
		else
			CloseMenu(clientIndex);
	}
	if (Clients[clientIndex].MenuOpen)
	{
		if (KeyDown(s, BUTTON_DPADDOWN))
		{
			if (Clients[clientIndex].Scroll == Clients[clientIndex].ScrollMax)
				Clients[clientIndex].Scroll = 1;
			else
				Clients[clientIndex].Scroll++;
			FadeOverTime(Hud[clientIndex].MenuScroller, 150, 255, 0, 0, 50);
			MoveOverTime(Hud[clientIndex].MenuScroller, 300, 320, (float)((30 * Clients[clientIndex].Scroll) + 50));
			FadeOverTime(Hud[clientIndex].MenuScroller, 150, 255, 0, 0, 255, 150);
		}

		if (KeyDown(s, BUTTON_DPADUP))
		{
			if (Clients[clientIndex].Scroll == 1)
				Clients[clientIndex].Scroll = Clients[clientIndex].ScrollMax;
			else
				Clients[clientIndex].Scroll--;
			FadeOverTime(Hud[clientIndex].MenuScroller, 150, 255, 0, 0, 50);
			MoveOverTime(Hud[clientIndex].MenuScroller, 300, 320, (float)((30 * Clients[clientIndex].Scroll) + 50));
			FadeOverTime(Hud[clientIndex].MenuScroller, 150, 255, 0, 0, 255, 150);
		}
	
		if (KeyDown(s, BUTTON_A))
		{
			if (Clients[clientIndex].MainMenu)
				OpenSub(clientIndex, Clients[clientIndex].Scroll);
			else if (Clients[clientIndex].SubMenu > 0)
				SubMenus(clientIndex, Clients[clientIndex].SubMenu, Clients[clientIndex].Scroll);
			else if (Clients[clientIndex].PlayersMenu == 1)
					ViewPlayer(clientIndex, 1, Clients[clientIndex].Scroll);
			else if (Clients[clientIndex].PlayersMenu == 2)
					ViewPlayer(clientIndex, 2, Clients[clientIndex].Scroll);
			else if (Clients[clientIndex].PlayersMenu == 3)
					ViewPlayer(clientIndex, 3, Clients[clientIndex].Scroll);
			else if (Clients[clientIndex].Player > 0)
					EditPlayer(Clients[clientIndex].Client, clientIndex,  Clients[clientIndex].Scroll);
		}

		if (KeyDown(s, BUTTON_B))
		{
			if (Clients[clientIndex].Player > 0)
				OpenSub(clientIndex, 7);
			else if (Clients[clientIndex].PlayersMenu > 0)
				OpenSub(clientIndex, 7);
			else if (Clients[clientIndex].SubMenu > 0)
				OpenMenu(clientIndex);
		}
	}
}
