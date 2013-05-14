#include "BaseStats.h"
#include <stdlib.h>

baseStats::baseStats() //constructor
{


	stats.Strength.amount = 10;
	stats.Intelligence.amount = 10;
	stats.Dexterity.amount = 10;
	stats.Health.amount = 10;
	stats.Strength.x = 0;
	stats.Strength.y = 0;
	stats.Intelligence.x = 0;
	stats.Intelligence.y = 0;
	stats.Dexterity.x = 0;
	stats.Dexterity.y = 0;
	stats.Health.x = 0;
	stats.Health.y = 0;
	stats.Strength.label = L"Strength";
	stats.Dexterity.label = L"Dexterity";
	stats.Intelligence.label = L"Intelligence";
	stats.Health.label = L"Health";
	stats.Strength.hwnd = NULL;
	stats.Strength.addButton = NULL;
	stats.Strength.subtractButton = NULL;
	stats.Intelligence.hwnd = NULL;
	stats.Intelligence.addButton = NULL;
	stats.Intelligence.subtractButton = NULL;
	stats.Dexterity.hwnd = NULL;
	stats.Dexterity.addButton = NULL;
	stats.Dexterity.subtractButton = NULL;
	stats.Health.hwnd = NULL;
	stats.Health.addButton = NULL;
	stats.Health.subtractButton = NULL;

	stats.Strength.string = std::to_wstring(stats.Strength.amount);
	stats.Intelligence.string = std::to_wstring(stats.Intelligence.amount);
	stats.Dexterity.string = std::to_wstring(stats.Dexterity.amount);
	stats.Health.string = std::to_wstring(stats.Health.amount);

	xSize = 0;
	ySize = 0;

	committed = FALSE;
	disabled = TRUE;
}


int baseStats::getStatInt(statWord stat) //returns -1 on poorly requested stat, returns stat otherwise.
{
	oneStat* answer = translateStatWord(stat);
	if (!answer)
	{
		callError(L"Attempted to translate unknown statWord at getStatInt");
		return -1;
	}
	return answer->amount;
}

LPCWSTR baseStats::getStatString(statWord stat)
{
	oneStat* answer = translateStatWord(stat);
	if (!answer)
	{
		callError(L"Attempted to translate unknown statWord at getStatString");
		return L"-1";
	}
	return answer->string.c_str();
}

int baseStats::changeStat(statWord stat, int addMe) //returns number of points used
{
	oneStat* statChanger = translateStatWord(stat); //determine stat to change, 0 if some mistake
	if (!statChanger)
		callError(L"Attempted to translate unknown statWord at changeStat");

	/*determine point cost*/
	int dir = 0;
	if (addMe > 0)
		dir = 1;
	else if (addMe < 0)
		dir = -1;
	int pointChange = getPointChange(statChanger->amount, dir);

	statChanger->amount += addMe; //change the stat, then reverse impossible changes
	if (statChanger->amount < 1)
	{
		statChanger->amount = 1;
		pointChange = 0;
	}
	if (statChanger->amount > 99)
	{
		statChanger->amount = 99;
		pointChange = 0;
	}

	statChanger->string = std::to_wstring(statChanger->amount); //display new amount
	SendMessage(statChanger->hwnd,WM_SETTEXT,FALSE,(LPARAM) statChanger->string.c_str());

	return -pointChange;
}

int baseStats::changeStat(statWord stat, int amount, int modifier) //returns -1 on poorly requested stat, 0 otherwise.
{
	return baseStats::changeStat(stat, amount*modifier);
}

int baseStats::getXSize()
{
	return xSize;
}

int baseStats::getYSize()
{
	return ySize;
}

void baseStats::createBoxes(HWND hwnd, int x, int y)
{
	stats.Strength.x = x + TAB;
	stats.Strength.y = y;
	stats.Intelligence.x = x + TAB;
	stats.Intelligence.y = y+BOXHEIGHT+YSPACING;
	stats.Dexterity.x = x + TAB;
	stats.Dexterity.y = y+2*BOXHEIGHT+2*YSPACING;
	stats.Health.x = x + TAB;
	stats.Health.y = y+3*BOXHEIGHT+3*YSPACING;
	xSize = stats.Strength.x + BOXLENGTH;
	ySize = stats.Health.y + BOXHEIGHT;
	stats.Strength.hwnd = CreateWindow(TEXT("EDIT"), stats.Strength.string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stats.Strength.x, stats.Strength.y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
	stats.Intelligence.hwnd = CreateWindow(TEXT("EDIT"), stats.Intelligence.string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stats.Intelligence.x, stats.Intelligence.y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
	stats.Dexterity.hwnd = CreateWindow(TEXT("EDIT"), stats.Dexterity.string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stats.Dexterity.x, stats.Dexterity.y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
	stats.Health.hwnd = CreateWindow(TEXT("EDIT"), stats.Health.string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stats.Health.x, stats.Health.y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
}

void baseStats::createButtons(HWND hwnd)
{
	createOneButton(ST, hwnd, ID_ST_ADD, ID_ST_SUB);
	createOneButton(IQ, hwnd, ID_IQ_ADD, ID_IQ_SUB);
	createOneButton(DX, hwnd, ID_DX_ADD, ID_DX_SUB);
	createOneButton(HT, hwnd, ID_HT_ADD, ID_HT_SUB);

	xSize += XSPACINGLONG + XSPACINGSHORT + 2*BOXLENGTH; //new buttons are this long... consider rewriting to be more UI dependent

	createToggleButton(hwnd, xSize, stats.Health.y - YSPACING);
}

void baseStats::createOneButton(statWord stat, HWND hwnd, buttonID idUp, buttonID idDown)
{
	oneStat *ptr = translateStatWord(stat);
	if (!ptr)
	{
		callError(L"Attempted to translate unknown statWord at createOneButton");
		return;
	}
	ptr->addButton = CreateWindow(TEXT("BUTTON"), TEXT("+"), WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON, ptr->x + XSPACINGLONG + BOXLENGTH, ptr->y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) idUp, NULL, NULL);
	ptr->subtractButton = CreateWindow(TEXT("BUTTON"), TEXT("-"), WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON, ptr->x + XSPACINGLONG + XSPACINGSHORT + 2*BOXLENGTH, ptr->y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) idDown, NULL, NULL);
	Button_Enable(ptr->addButton, FALSE);
	Button_Enable(ptr->subtractButton, FALSE);
}

void baseStats::createToggleButton(HWND hwnd, int x, int y)
{
	disableButton = CreateWindow(TEXT("BUTTON"), TEXT("DISABLED"), WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON, x + 1.5*XSPACINGLONG, y - BOXHEIGHT, 2*LONGBOXLENGTH, 2*BOXHEIGHT, hwnd, (HMENU) ID_TOGGLE_BASE, NULL, NULL);
	Button_Enable(disableButton, FALSE);
	xSize += 1.5*XSPACINGLONG + 2*LONGBOXLENGTH;
}

void baseStats::paintAll(HDC hdc)
{
	paintText(ST, hdc);
	paintText(IQ, hdc);
	paintText(DX, hdc);
	paintText(HT, hdc);
}

void baseStats::paintText(statWord stat, HDC hdc)
{
	oneStat *paintStat = translateStatWord(stat);
	if (!paintStat)
	{
		callError(L"Attempted to translate unknown statWord at paintText");
		return;
	}
	SetTextAlign(hdc, TA_RIGHT);

	TextOut (hdc, paintStat->x - XSPACINGSHORT, paintStat->y, paintStat->label.c_str(), paintStat->label.length());

	SetTextAlign(hdc, TA_LEFT); //reset text align

}

oneStat* baseStats::translateStatWord(statWord stat)
{
	switch(stat)
	{
	case ST:
		return &stats.Strength;

	case IQ:
		return &stats.Intelligence;

	case DX:
		return &stats.Dexterity;

	case HT:
		return &stats.Health;

	default:
		return NULL;
	}
}

void baseStats::callError(std::wstring info)
{
	MessageBox(NULL, info.c_str(), NULL, MB_OK);
	MessageBox(NULL, L"The program may continue, but check anything dependent on the base stats for errors.", NULL, MB_OK);
}

void baseStats::engineReceiver(WORD identifier, messengerData &data, bool dependency)
{
	int modifier = 0;
	statWord target = 0;
	switch(identifier)
	{
	case ID_ST_ADD:
	case ID_IQ_ADD:
	case ID_DX_ADD:
	case ID_HT_ADD:			//if it was a plus button
		modifier = 1;
		break;
	default:				//otherwise it was a minus button, or I don't care.
		modifier = -1;
		break;
	}

	switch(identifier)
	{
	case ID_ST_ADD:
	case ID_ST_SUB:
		target = ST;
		break;
	case ID_IQ_ADD:
	case ID_IQ_SUB:
		target = IQ;
		break;
	case ID_DX_ADD:
	case ID_DX_SUB:
		target = DX;
		break;
	case ID_HT_ADD:
	case ID_HT_SUB:
		target = HT;
		break;
	case ID_TOGGLE_GENERAL:
		toggleDisable(dependency);
		return;
	case ID_TOGGLE_BASE:
		toggleCommit();
		return;

	default:
		break;
	}


	data.points += changeStat(target,modifier);
}

void baseStats::toggleCommit()
{
	if (disabled) //insurance policy, should never get hit unless coding error
	{
		callError(L"Cannot lock until dependencies are locked");
		return;
	}

	if (committed)
	{
		committed = FALSE;
		SendMessage(disableButton,WM_SETTEXT,FALSE,(LPARAM) L"Lock Stats");
		Button_Enable(stats.Strength.addButton, TRUE);
		Button_Enable(stats.Strength.subtractButton, TRUE);
		Button_Enable(stats.Intelligence.addButton, TRUE);
		Button_Enable(stats.Intelligence.subtractButton, TRUE);
		Button_Enable(stats.Dexterity.addButton, TRUE);
		Button_Enable(stats.Dexterity.subtractButton, TRUE);
		Button_Enable(stats.Health.addButton, TRUE);
		Button_Enable(stats.Health.subtractButton, TRUE);
	}

	else
	{
		committed = TRUE;
		SendMessage(disableButton,WM_SETTEXT,FALSE,(LPARAM) L"Unlock");
		Button_Enable(stats.Strength.addButton, FALSE);
		Button_Enable(stats.Strength.subtractButton, FALSE);
		Button_Enable(stats.Intelligence.addButton, FALSE);
		Button_Enable(stats.Intelligence.subtractButton, FALSE);
		Button_Enable(stats.Dexterity.addButton, FALSE);
		Button_Enable(stats.Dexterity.subtractButton, FALSE);
		Button_Enable(stats.Health.addButton, FALSE);
		Button_Enable(stats.Health.subtractButton, FALSE);
	}
}

void baseStats::toggleDisable(bool dependency)
{
	if (dependency)
	{
		SendMessage(disableButton,WM_SETTEXT,FALSE,(LPARAM) L"Lock Stats");
		Button_Enable(disableButton, TRUE);
		disabled = FALSE;
		Button_Enable(stats.Strength.addButton, TRUE);
		Button_Enable(stats.Strength.subtractButton, TRUE);
		Button_Enable(stats.Intelligence.addButton, TRUE);
		Button_Enable(stats.Intelligence.subtractButton, TRUE);
		Button_Enable(stats.Dexterity.addButton, TRUE);
		Button_Enable(stats.Dexterity.subtractButton, TRUE);
		Button_Enable(stats.Health.addButton, TRUE);
		Button_Enable(stats.Health.subtractButton, TRUE);
	}
	else
	{
		Button_Enable(disableButton, FALSE);
		SendMessage(disableButton,WM_SETTEXT,FALSE,(LPARAM) L"DISABLED");
		disabled = TRUE;
		Button_Enable(stats.Strength.addButton, FALSE);
		Button_Enable(stats.Strength.subtractButton, FALSE);
		Button_Enable(stats.Intelligence.addButton, FALSE);
		Button_Enable(stats.Intelligence.subtractButton, FALSE);
		Button_Enable(stats.Dexterity.addButton, FALSE);
		Button_Enable(stats.Dexterity.subtractButton, FALSE);
		Button_Enable(stats.Health.addButton, FALSE);
		Button_Enable(stats.Health.subtractButton, FALSE);
	}
}

bool baseStats::isCommitted()
{
	return committed;
}

int baseStats::getPointChange(int initialStat, int direction)
{
	if (direction > 0) //if we are increasing the stat, we must lose points.
	{
		switch (initialStat)
		{
		case 1: //if we're going from 1 to 2...
		case 2:
		case 3: //if we're going from 3 to 4... etc
		case 4:
		case 5:
		case 6:
			return 10; //costs 10 points.
		case 7:
		case 8:
			return 5;
		case 9:
		case 10:
		case 11:
		case 12:
			return 10;
		case 13:
		case 14:
			return 15;
		case 15:
		case 16:
			return 20;
		default:
			return 25;
		}
	}

	else if (direction < 0) //if we are decreasing the stat, we must gain points.
	{
		switch (initialStat)
		{
		case 2: //if we're going from 2 to 1...
		case 3:
		case 4: //if we're going from 4 to 3...
		case 5:
		case 6:
		case 7:
			return -10;
		case 8:
		case 9:
			return -5;
		case 10:
		case 11:
		case 12:
		case 13:
			return -10;
		case 14:
		case 15:
			return -15;
		case 16:
		case 17:
			return -20;
		default:
			return -25;
		}
	}
	return 0;
}