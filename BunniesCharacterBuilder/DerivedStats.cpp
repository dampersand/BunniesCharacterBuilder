#include "DerivedStats.h"

derived::derived()
{
	statInitiator(&speed, L"Speed", 0, 0, 0);
	statInitiator(&size, L"Size", 0, 0, 0);
	statInitiator(&weight, L"Weight", 0, 0, 0);
	statInitiator(&dodge, L"Dodge", 0, 0, 0);
	statInitiator(&claw, L"Claw", 0, 0, 0);
	statInitiator(&bite, L"Bite", 0, 0, 0);
	statInitiator(&kick, L"Kick", 0, 0, 0);
	statInitiator(&move, L"Move", 0, 0, 0);
	statInitiator(&clawChance, L"Chance", 0, 0, 0);
	statInitiator(&biteChance, L"Chance", 0,0,0);
	statInitiator(&kickChance, L"Chance", 0,0,0);

	xSize = 0;
	ySize = 0;
}

void derived::statInitiator(bunnyStat* stat, std::wstring lab, int X, int Y, int am)
{
	stat->amount = am;
	stat->label = lab;
	stat->x = X;
	stat->y = Y;
	stat->hwnd = 0;
	stat->string = std::to_wstring(stat->amount);
}

int derived::getXSize()
{
	return xSize;
}

int derived::getYSize()
{
	return ySize;
}

bunnyStat* derived::translateStatWord(statWord stat)
{
	switch(stat)
	{
	case SPD:
		return &speed;
	case SZ:
		return &size;
	case WT:
		return &weight;
	case DG:
		return &dodge;
	case CW:
		return &claw;
	case BT:
		return &bite;
	case KK:
		return &kick;
	case MV:
		return &move;
	case CWC:
		return &clawChance;
	case BTC:
		return &biteChance;
	case KKC:
		return &kickChance;
	default:
		return NULL;
	}
}

void derived::createBox(HWND hwnd, bunnyStat* stat, int length)
{
	stat->hwnd = CreateWindow(TEXT("EDIT"), stat->string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stat->x, stat->y, length, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
}

void derived::callError(std::wstring function)
{
	MessageBox(NULL, function.c_str(), NULL, MB_OK);
}

void derived::paintText(bunnyStat* stat, HDC hdc)
{
	if (!stat)
	{
		callError(L"Empty pointer at derived::paintText");
		return;
	}
	SetTextAlign(hdc, TA_RIGHT);

	TextOut (hdc, stat->x - XSPACINGSHORT, stat->y, stat->label.c_str(), stat->label.length());

	SetTextAlign(hdc, TA_LEFT); //reset text align

}

void derived::createBoxes(HWND hwnd, int x, int y)
{
	size.x = x + TAB;
	size.y = y;
	weight.x = size.x + TAB + BOXLENGTH;
	weight.y = size.y;
	speed.x = x + TAB;
	speed.y = size.y + BOXHEIGHT + YSPACING;
	dodge.x = speed.x + TAB + BOXLENGTH;
	dodge.y = speed.y;
	move.x = x + TAB;
	move.y = dodge.y + BOXHEIGHT + YSPACING;
	claw.x = x + TAB;
	claw.y = move.y + YSPACING + BOXHEIGHT;
	clawChance.x = claw.x + LONGBOXLENGTH + TAB;
	clawChance.y = claw.y;
	bite.x = x + TAB;
	bite.y = claw.y + YSPACING + BOXHEIGHT;
	biteChance.x = bite.x + LONGBOXLENGTH + TAB;
	biteChance.y = bite.y;
	kick.x = x + TAB;
	kick.y = bite.y + YSPACING + BOXHEIGHT;
	kickChance.x = kick.x + LONGBOXLENGTH + TAB;
	kickChance.y = kick.y;

	createBox(hwnd, &size, BOXLENGTH);
	createBox(hwnd, &weight, 1.5*BOXLENGTH);
	createBox(hwnd, &speed, BOXLENGTH);
	createBox(hwnd, &dodge, BOXLENGTH);
	createBox(hwnd, &claw, LONGBOXLENGTH);
	createBox(hwnd, &clawChance, BOXLENGTH);
	createBox(hwnd, &bite, LONGBOXLENGTH);
	createBox(hwnd, &biteChance, BOXLENGTH);
	createBox(hwnd, &kick, LONGBOXLENGTH);
	createBox(hwnd, &kickChance, BOXLENGTH);
	createBox(hwnd, &move, BOXLENGTH);

	xSize = max(kickChance.x + BOXLENGTH, weight.x + BOXLENGTH);
	ySize = kick.y + BOXHEIGHT;
	ySize += YSPACING;

}

void derived::paintAll(HDC hdc)
{
	paintText(&size, hdc);
	paintText(&weight, hdc);
	paintText(&speed, hdc);
	paintText(&dodge, hdc);
	paintText(&claw, hdc);
	paintText(&clawChance, hdc);
	paintText(&kick, hdc);
	paintText(&kickChance, hdc);
	paintText(&bite, hdc);
	paintText(&biteChance, hdc);
	paintText(&move, hdc);
}

int derived::getStat(statWord stat)
{
	bunnyStat* answer = translateStatWord(stat);

	return answer->amount;
}

void derived::calculateStat(statWord stat, messengerData data)
{
	bunnyStat* answer = translateStatWord(stat);
	bool flag = FALSE; //flag that specifies whether string has been assigned or not

	switch(stat)
	{
	case SPD: //Speed calculation
		answer->amount = (data.dexterity + data.health)/2;
		break;
	case SZ: //Size calculation, in stretched out length.
		answer->amount = 15 + (data.strength - 10);
		break;
	case WT: //Weight calculation, in R-lbs
		answer->amount = 160+((data.strength - 10)*10) + data.fat;
		break;
	case DG: //Dodge calculation
		answer->amount = ((data.speed/2) + 1) + max(data.bunFu*(1/5), data.brawl*(1/10));
		break;
	case CW: //Claw calculation
		{
		int modifier = thrustTableMod(data.strength); //determine base thrust modifier
		int dice = thrustTable(data.strength); //determine base thrust damage
		std::wstring intermediary;
		std::wstring modSign;

		modifier += max(data.bunFu*(1/5), data.brawl*(1/10)) -2;

		if (modifier >= 0)
			modSign = L"+";
		else
			modSign = L"";
		
		intermediary = std::to_wstring(dice);
		intermediary.append(L"d");
		intermediary.append(modSign);
		intermediary.append(std::to_wstring(modifier));
		answer->string = intermediary;
		flag = TRUE;
		break;
		}
	case CWC: //Claw chance calculation
		answer->amount = max(data.dexterity, max(data.bunFu, data.brawl));
		break;
	case BT: //Bite calculation
		{
		int modifier = swingTableMod(data.strength); //determine base swing modifier
		int dice = swingTable(data.strength); //determine base swing damage
		std::wstring intermediary;
		std::wstring modSign;

		modifier += max(data.bunFu*(1/5), data.brawl*(1/10)) -1;

		if (modifier >= 0)
			modSign = L"+";
		else
			modSign = L"";
		
		intermediary = std::to_wstring(dice);
		intermediary.append(L"d");
		intermediary.append(modSign);
		intermediary.append(std::to_wstring(modifier));
		answer->string = intermediary;
		flag = TRUE;
		break;
		}
	case BTC: //Bite chance calculation
		answer->amount = max(data.dexterity, max(data.bunFu, data.brawl));
		break;
	case KK: //Kick calculation
		{
		int modifier = thrustTableMod(max(data.dexterity - 2, max(data.bunFu - 2, data.brawl - 2)));
		int dice = thrustTable(max(data.dexterity - 2, max(data.bunFu - 2, data.brawl - 2)));
		std::wstring intermediary;
		std::wstring modSign;

		modifier += max(data.bunFu*(1/5), data.brawl*(1/10)) -1;

		if (modifier >= 0)
			modSign = L"+";
		else
			modSign = L"";
		
		intermediary = std::to_wstring(dice);
		intermediary.append(L"d");
		intermediary.append(modSign);
		intermediary.append(std::to_wstring(modifier));
		answer->string = intermediary;
		flag = TRUE;
		break;
		}
	case KKC: //Kick chance calculation
		answer->amount = max(data.dexterity - 2, max(data.bunFu - 2, data.brawl - 2));
		break;
	case MV: //Move calculation
		answer->amount = data.speed + (data.run/8);
		break;
	default:
		callError(L"Unknown Stat at derived::calculateStat");
		break;
	}

	if (!flag)
		answer->string = std::to_wstring(answer->amount);

	SendMessage(answer->hwnd,WM_SETTEXT,FALSE,(LPARAM) answer->string.c_str());

}

int derived::thrustTable(int strength)
{
	if (strength < 0)
		callError(L"Incorrect strength value given to thrustTable, assuming dice = 1");
	if (strength < 19) //determine dice based on thrust
			return 1;
		else if (strength < 27)
			return 2;
		else if (strength < 35)
			return 3;
		else if (strength < 45)
			return 4;
		else if (strength < 55)
			return 5;
		else if (strength < 60)
			return 6;
		else
			return ((strength+10) - ((strength+10)%10)) / 10;
}

int derived::thrustTableMod(int strength)
{
	switch(strength)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		return (strength - 10);
	case 7:
	case 8:
		return -3;
	case 9:
	case 10:
		return -2;
	case 11:
	case 12:
	case 19:
	case 20:
	case 27:
	case 28:
	case 35:
	case 36:
	case 60:
	case 61:
	case 62:
	case 63:
	case 64:
		return -1;
	case 13:
	case 14:
	case 21:
	case 22:
	case 29:
	case 30:
	case 37:
	case 38:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 90:
	case 91:
	case 92:
	case 93:
	case 94:
		return 0;
	case 15:
	case 16:
	case 23:
	case 24:
	case 31:
	case 32:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
		return 1;
	case 17:
	case 18:
	case 25:
	case 26:
	case 33:
	case 34:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 95:
	case 96:
	case 97:
	case 98:
	case 99:
		return 2;
	default:
		return 0;
	}
}

int derived::swingTable(int strength)
{
	if (strength < 0)
		callError(L"Incorrect strength value given to swingTable, assuming dice = 1");
	if (strength < 13)
		return 1;
	else if (strength < 17)
		return 2;
	else if (strength < 21)
		return 3;
	else if (strength < 24)
		return 4;
	else if (strength < 31)
		return 5;
	else if (strength < 39)
		return 6;
	else if (strength < 50)
		return 7;
	else
		return (((strength + 10) - ((strength + 10)%10)/10) + 2);
}

int derived::swingTableMod(int strength)
{
	switch(strength)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		return (strength - 10);
	case 11:
	case 15:
	case 19:
	case 23:
	case 27:
	case 28:
	case 35:
	case 36:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
		return 1;
	case 12:
	case 16:
	case 20:
	case 24:
	case 29:
	case 30:
	case 37:
	case 38:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 95:
	case 96:
	case 97:
	case 98:
	case 99:
		return 2;
	case 13:
	case 17:
	case 21:
	case 25:
	case 31:
	case 32:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
		return -1;
	case 14:
	case 18:
	case 22:
	case 26:
	case 33:
	case 34:
	case 60:
	case 61:
	case 62:
	case 63:
	case 64:
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 90:
	case 91:
	case 92:
	case 93:
	case 94:
		return 0;

	default:
		return 0;
	}
}

void derived::recalculateAll(messengerData data)
{
	messengerData newData = data; //speed size weight dodge claw bite kick move
	newData.strength = data.dexterity;
	newData.intelligence = data.health;
	calculateStat(SPD, newData); //calculate speed

	newData.strength = data.strength;
	calculateStat(SZ, newData); //calculate size

	calculateStat(WT, newData); //calculate weight

	newData.speed = getStat(SPD);
	calculateStat(DG, newData);

	newData.strength = data.strength;
	calculateStat(CW, newData);

	calculateStat(BT, newData);

	newData.strength = data.dexterity;
	calculateStat(KK, newData);

	calculateStat(CWC, newData);
	calculateStat(BTC, newData);
	calculateStat(KKC, newData);

	newData.speed = getStat(SPD);
	calculateStat(MV, newData);
}