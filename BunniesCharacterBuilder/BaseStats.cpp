#include "BaseStats.h"
#include <stdlib.h>

baseStats::baseStats(messengerData &data) //constructor
{
	statList = &data;

	statList->makeNewStat(ST, 10, 0, 0, L"Strength", L"10", 0, 0, 0);
	statList->makeNewStat(IQ, 10, 0, 0, L"Intelligence", L"10", 0, 0, 0);
	statList->makeNewStat(DX, 10, 0, 0, L"Dexterity", L"10", 0, 0, 0);
	statList->makeNewStat(HT, 10, 0, 0, L"Health", L"10", 0, 0, 0);

	xSize = 0;
	ySize = 0;
	xStart = 0;
	yStart = 0;

	committed = FALSE;
	disabled = TRUE;
}

int baseStats::changeStat(statWord stat, int addMe) //returns number of points used, if required in future.
{
	bunnyStat* statChanger = statList->getStat(stat); //determine stat to change, 0 if some mistake
	if (statChanger->identifier == EMPTY)
		callError(L"Empty pointer at baseStats::changeStat");

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

	statChanger->string = std::to_wstring(statChanger->amount); //set new amount to displayable string
	statChanger->updateStat(); //display string
	statList->editStat(statChanger); //commit all stat changes

	//update and display point changes
	statList->changeStat(PT, -pointChange);

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

void baseStats::createBoxes(HWND hwnd)
{
	int x = xStart;
	int y = yStart;

	//get temporary stat pointers
	bunnyStat *strength = statList->getStat(ST);
	bunnyStat *intelligence = statList->getStat(IQ);
	bunnyStat *dexterity = statList->getStat(DX);
	bunnyStat *health = statList->getStat(HT);

	//set location of boxes - one day I will rewrite this so the UI does it.
	strength->x = x + TAB;
	strength->y = y;
	intelligence->x = x + TAB;
	intelligence->y = y+BOXHEIGHT+YSPACING;
	dexterity->x = x + TAB;
	dexterity->y = y+2*BOXHEIGHT+2*YSPACING;
	health->x = x+TAB;
	health->y = y+3*BOXHEIGHT+ 3*YSPACING;

	//update sizes - can be rewritten to select largest x from a list... but then ya gotta make a local stat list.
	xSize = strength->x + BOXLENGTH;
	ySize = health->y + BOXHEIGHT;

	//create windows - consider making a helper function
	strength->hwnd = CreateWindow(TEXT("EDIT"), strength->string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, strength->x, strength->y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
	intelligence->hwnd = CreateWindow(TEXT("EDIT"), intelligence->string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, intelligence->x, intelligence->y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
	dexterity->hwnd = CreateWindow(TEXT("EDIT"), dexterity->string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, dexterity->x, dexterity->y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
	health->hwnd = CreateWindow(TEXT("EDIT"), health->string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, health->x, health->y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);

	//update statList with initial values and information
	statList->editStat(strength);
	statList->editStat(dexterity);
	statList->editStat(intelligence);
	statList->editStat(health);
}

void baseStats::createButtons(HWND hwnd)
{
	createOneButton(ST, hwnd, ID_ST_ADD, ID_ST_SUB);
	createOneButton(IQ, hwnd, ID_IQ_ADD, ID_IQ_SUB);
	createOneButton(DX, hwnd, ID_DX_ADD, ID_DX_SUB);
	createOneButton(HT, hwnd, ID_HT_ADD, ID_HT_SUB);

	xSize += XSPACINGLONG + XSPACINGSHORT + 2*BOXLENGTH; //new buttons are this long... consider rewriting to be more UI dependent

	createToggleButton(hwnd, xSize, statList->getStat(HT)->y - YSPACING);
}

void baseStats::createOneButton(statWord stat, HWND hwnd, buttonID idUp, buttonID idDown)
{
	//get a temporary pointer
	bunnyStat *ptr = statList->getStat(stat);

	if (!ptr)
	{
		callError(L"Attempted to translate unknown statWord at baseStats::createOneButton");
		return;
	}
	//create addition or subtraction buttons
	ptr->addButton = CreateWindow(TEXT("BUTTON"), TEXT("+"), WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON, ptr->x + XSPACINGLONG + BOXLENGTH, ptr->y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) idUp, NULL, NULL);
	ptr->subtractButton = CreateWindow(TEXT("BUTTON"), TEXT("-"), WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON, ptr->x + XSPACINGLONG + XSPACINGSHORT + 2*BOXLENGTH, ptr->y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) idDown, NULL, NULL);
	Button_Enable(ptr->addButton, FALSE);
	Button_Enable(ptr->subtractButton, FALSE);

	//update statList
	statList->editStat(ptr);
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
	bunnyStat *paintStat = statList->getStat(stat);
	if (paintStat->identifier == EMPTY)
	{
		callError(L"Attempted to translate unknown statWord at paintText");
		return;
	}
	SetTextAlign(hdc, TA_RIGHT);

	TextOut (hdc, paintStat->x - XSPACINGSHORT, paintStat->y, paintStat->label.c_str(), paintStat->label.length());

	SetTextAlign(hdc, TA_LEFT); //reset text align

}

void baseStats::callError(std::wstring info)
{
	MessageBox(NULL, info.c_str(), NULL, MB_OK);
	MessageBox(NULL, L"The program may continue, but check anything dependent on the base stats for errors.", NULL, MB_OK);
}

void baseStats::engineReceiver(WORD identifier, bool dependency)
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
		return;
	}

	if (target) //this section will run if an add- or subtract button was pressed
	{
		changeStat(target, modifier); //change the stat in question
	}

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
		setStatButtonState(ST, !committed);
		setStatButtonState(IQ, !committed);
		setStatButtonState(DX, !committed);
		setStatButtonState(HT, !committed);
	}

	else
	{
		committed = TRUE;
		SendMessage(disableButton,WM_SETTEXT,FALSE,(LPARAM) L"Unlock");
		setStatButtonState(ST, !committed);
		setStatButtonState(IQ, !committed);
		setStatButtonState(DX, !committed);
		setStatButtonState(HT, !committed);
	}
}

void baseStats::toggleDisable(bool dependency)
{
	if (dependency)
	{
		SendMessage(disableButton,WM_SETTEXT,FALSE,(LPARAM) L"Lock Stats");
		Button_Enable(disableButton, TRUE);
		disabled = FALSE;
		setStatButtonState(ST, !disabled);
		setStatButtonState(IQ, !disabled);
		setStatButtonState(DX, !disabled);
		setStatButtonState(HT, !disabled);
	}
	else
	{
		Button_Enable(disableButton, FALSE);
		SendMessage(disableButton,WM_SETTEXT,FALSE,(LPARAM) L"DISABLED");
		disabled = TRUE;
		setStatButtonState(ST, !disabled);
		setStatButtonState(IQ, !disabled);
		setStatButtonState(DX, !disabled);
		setStatButtonState(HT, !disabled);
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

void baseStats::setStart(int x, int y)
{
	xStart = x;
	yStart = y;
}

int baseStats::getXStart()
{
	return xStart;
}

int baseStats::getYStart()
{
	return yStart;
}

void baseStats::setStatButtonState(statWord stat, bool state)
{
	bunnyStat* answer = statList->getStat(stat);

	if (answer->identifier == EMPTY)
	{
		callError(L"empty pointer at baseStats::setStatButtonState");
		return;
	}

	Button_Enable(answer->addButton, state);
	Button_Enable(answer->subtractButton, state);
}