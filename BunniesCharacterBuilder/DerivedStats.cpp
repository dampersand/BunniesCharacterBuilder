#include "DerivedStats.h"

derived::derived(messengerData &data)
{
	statList = &data;

	statList->makeNewStat(SPD, 0, 0, 0, L"Speed", L"0", 0, 0, 0);
	statList->makeNewStat(SZ, 0, 0, 0, L"Size", L"0", 0, 0, 0);
	statList->makeNewStat(WT, 0, 0, 0, L"Weight", L"0", 0, 0, 0);
	statList->makeNewStat(DG, 0, 0, 0, L"Dodge", L"0", 0, 0, 0);
	statList->makeNewStat(CW, 0, 0, 0, L"Claw", L"0", 0, 0, 0);
	statList->makeNewStat(BT, 0, 0, 0, L"Bite", L"0", 0, 0, 0);
	statList->makeNewStat(KK, 0, 0, 0, L"Kick", L"0", 0, 0, 0);
	statList->makeNewStat(MV, 0, 0, 0, L"Move", L"0", 0, 0, 0);
	statList->makeNewStat(CWC, 0, 0, 0, L"Chance", L"0", 0, 0, 0);
	statList->makeNewStat(BTC, 0, 0, 0, L"Chance", L"0", 0, 0, 0);
	statList->makeNewStat(KKC, 0, 0, 0, L"Chance", L"0", 0, 0, 0);


	xSize = 0;
	ySize = 0;
	xStart = 0;
	yStart = 0;
}

int derived::getXSize()
{
	return xSize;
}

int derived::getYSize()
{
	return ySize;
}

void derived::createBoxes(HWND hwnd)
{
	//temporary variables
	int x = xStart;
	int y = yStart;

	//temporary statlist
	bunnyStat* size = statList->getStat(SZ);
	bunnyStat* weight = statList->getStat(WT);
	bunnyStat* speed = statList->getStat(SPD);
	bunnyStat* dodge = statList->getStat(DG);
	bunnyStat* move = statList->getStat(MV);
	bunnyStat* claw = statList->getStat(CW);
	bunnyStat* bite = statList->getStat(BT);
	bunnyStat* kick = statList->getStat(KK);
	bunnyStat* clawChance = statList->getStat(CWC);
	bunnyStat* biteChance = statList->getStat(BTC);
	bunnyStat* kickChance = statList->getStat(KKC);

	//set initial statlist positions
	size->x = x + TAB;
	size->y = y;
	weight->x = size->x + TAB + BOXLENGTH;
	weight->y = size->y;
	speed->x = x + TAB;
	speed->y = size->y + BOXHEIGHT + YSPACING;
	dodge->x = speed->x + TAB + BOXLENGTH;
	dodge->y = speed->y;
	move->x = x + TAB;
	move->y = dodge->y + BOXHEIGHT + YSPACING;
	claw->x = x + TAB;
	claw->y = move->y + YSPACING + BOXHEIGHT;
	clawChance->x = claw->x + LONGBOXLENGTH + TAB;
	clawChance->y = claw->y;
	bite->x = x + TAB;
	bite->y = claw->y + YSPACING + BOXHEIGHT;
	biteChance->x = bite->x + LONGBOXLENGTH + TAB;
	biteChance->y = bite->y;
	kick->x = x + TAB;
	kick->y = bite->y + YSPACING + BOXHEIGHT;
	kickChance->x = kick->x + LONGBOXLENGTH + TAB;
	kickChance->y = kick->y;

	//create display boxes
	createBox(hwnd, size, BOXLENGTH);
	createBox(hwnd, weight, 1.5*BOXLENGTH);
	createBox(hwnd, speed, BOXLENGTH);
	createBox(hwnd, dodge, BOXLENGTH);
	createBox(hwnd, claw, LONGBOXLENGTH);
	createBox(hwnd, clawChance, BOXLENGTH);
	createBox(hwnd, bite, LONGBOXLENGTH);
	createBox(hwnd, biteChance, BOXLENGTH);
	createBox(hwnd, kick, LONGBOXLENGTH);
	createBox(hwnd, kickChance, BOXLENGTH);
	createBox(hwnd, move, BOXLENGTH);

	xSize = max(kickChance->x + BOXLENGTH, weight->x + BOXLENGTH);
	ySize = kick->y + BOXHEIGHT;
	ySize += YSPACING;

	//update stats
	statList->editStat(size);
	statList->editStat(weight);
	statList->editStat(speed);
	statList->editStat(dodge);
	statList->editStat(claw);
	statList->editStat(clawChance);
	statList->editStat(bite);
	statList->editStat(biteChance);
	statList->editStat(kick);
	statList->editStat(kickChance);
	statList->editStat(move);

}

void derived::createBox(HWND hwnd, bunnyStat* stat, int length)
{
	stat->hwnd = CreateWindow(TEXT("EDIT"), stat->string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stat->x, stat->y, length, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
}

void derived::callError(std::wstring function)
{
	MessageBox(NULL, function.c_str(), NULL, MB_OK | MB_TASKMODAL);
}

void derived::paintText(statWord stat, HDC hdc)
{
	bunnyStat* answer = statList->getStat(stat);

	if (answer->identifier == EMPTY)
	{
		callError(L"Empty pointer at derived::paintText");
		return;
	}
	SetTextAlign(hdc, TA_RIGHT);

	TextOut (hdc, answer->x - XSPACINGSHORT, answer->y, answer->label.c_str(), answer->label.length());

	SetTextAlign(hdc, TA_LEFT); //reset text align

}


void derived::paintAll(HDC hdc)
{
	paintText(SZ, hdc);
	paintText(WT, hdc);
	paintText(SPD, hdc);
	paintText(DG, hdc);
	paintText(CW, hdc);
	paintText(CWC, hdc);
	paintText(KK, hdc);
	paintText(KKC, hdc);
	paintText(BT, hdc);
	paintText(BTC, hdc);
	paintText(MV, hdc);
}

void derived::calculateStat(statWord stat)
{
	bunnyStat* answer = statList->getStat(stat); //answer that we'll set
	bunnyStat *param1, *param2, *param3, *param4; //parameters to use for calculation

	bool flag = FALSE; //flag that specifies whether string has been assigned or not

	switch(stat)
	{
	case SPD: //Speed calculation
		{
			param1 = statList->getStat(DX);
			param2 = statList->getStat(HT);
			answer->amount = (param1->amount + param2->amount)/2;
			break;
		}

	case SZ: //Size calculation, in stretched out length.
		{
			param1 = statList->getStat(ST);
			answer->amount = 15 + (param1->amount - 10);
			break;
		}

	case WT: //Weight calculation, in R-lbs
		{
			param1 = statList->getStat(ST);
			param2 = statList->getStat(FAT); //TODO: fix calculation here.  Not sure how 'fat' works.
			answer->amount = 160+((param1->amount - 10)*10) + param2->amount;
			break;
		}

	case DG: //Dodge calculation
		{
			param1 = statList->getStat(SPD);
			param2 = statList->getStat(BF);
			param3 = statList->getStat(BW);
			answer->amount = ((param1->amount/2) + 1) + max(param2->amount*(1/5), param3->amount*(1/10));
			break;
		}

	case CW: //Claw calculation
		{
			param1 = statList->getStat(ST);
			param2 = statList->getStat(BF);
			param3 = statList->getStat(BW);
			int modifier = thrustTableMod(param1->amount); //determine base thrust modifier
			int dice = thrustTable(param1->amount); //determine base thrust damage
			std::wstring intermediary;
			std::wstring modSign;

			modifier += max(param2->amount*(1/5), param3->amount*(1/10)) -2;

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
		{
			param1 = statList->getStat(DX);
			param2 = statList->getStat(BF);
			param3 = statList->getStat(BW);
			answer->amount = max(param1->amount, max(param2->amount, param3->amount));
			break;
		}
	case BT: //Bite calculation
		{
			param1 = statList->getStat(ST);
			param2 = statList->getStat(BF);
			param3 = statList->getStat(BW);
			int modifier = swingTableMod(param1->amount); //determine base swing modifier
			int dice = swingTable(param1->amount); //determine base swing damage
			std::wstring intermediary;
			std::wstring modSign;

			modifier += max(param2->amount*(1/5), param3->amount*(1/10)) -1;

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
		{
			param1 = statList->getStat(DX);
			param2 = statList->getStat(BF);
			param3 = statList->getStat(BW);
			answer->amount = max(param1->amount, max(param2->amount, param3->amount));
			break;
		}
	case KK: //Kick calculation
		{
			param1 = statList->getStat(DX);
			param2 = statList->getStat(BF);
			param3 = statList->getStat(BW);
			int modifier = thrustTableMod(max(param1->amount - 2, max(param2->amount - 2, param3->amount - 2)));
			int dice = thrustTable(max(param1->amount - 2, max(param2->amount - 2, param3->amount - 2)));
			std::wstring intermediary;
			std::wstring modSign;

			modifier += max(param2->amount*(1/5), param3->amount*(1/10)) -1;

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
		{
			param1 = statList->getStat(DX);
			param2 = statList->getStat(BF);
			param3 = statList->getStat(BW);
			answer->amount = max(param1->amount - 2, max(param2->amount - 2, param3->amount - 2));
			break;
		}
	case MV: //Move calculation
		{
			param1 = statList->getStat(SPD);
			param2 = statList->getStat(RN);
			answer->amount = param1->amount + (param2->amount/8);
			break;
		}
	default:
		callError(L"Unknown Stat at derived::calculateStat");
		break;
	}

	if (!flag)
		answer->string = std::to_wstring(answer->amount);

	statList->editStat(answer); //update calculation
	answer->updateStat();

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

void derived::recalculateAll()
{
	calculateStat(SPD); //calculate speed
	calculateStat(SZ); //calculate size
	calculateStat(WT); //calculate weight
	calculateStat(DG);
	calculateStat(CW);
	calculateStat(BT);
	calculateStat(KK);
	calculateStat(CWC);
	calculateStat(BTC);
	calculateStat(KKC);
	calculateStat(MV);
}

void derived::setStart(int x, int y)
{
	xStart = x;
	yStart = y;
}

int derived::getXStart()
{
	return xStart;
}

int derived::getYStart()
{
	return yStart;
}