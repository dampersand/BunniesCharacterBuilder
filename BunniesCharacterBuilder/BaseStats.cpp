#include "BaseStats.h"
#include <stdlib.h>

baseStats::baseStats() //constructor
{


	stats.Strength.amount = 0;
	stats.Intelligence.amount = 0;
	stats.Dexterity.amount = 0;
	stats.Health.amount = 0;
	stats.Strength.x = INITIALDISTANCE + TAB;
	stats.Strength.y = INITIALDISTANCE;
	stats.Intelligence.x = INITIALDISTANCE + TAB;
	stats.Intelligence.y = INITIALDISTANCE+BOXHEIGHT+YSPACING;
	stats.Dexterity.x = INITIALDISTANCE + TAB;
	stats.Dexterity.y = INITIALDISTANCE+2*BOXHEIGHT+2*YSPACING;
	stats.Health.x = INITIALDISTANCE + TAB;
	stats.Health.y = INITIALDISTANCE+3*BOXHEIGHT+3*YSPACING;
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

	_itow_s(stats.Strength.amount, stats.Strength.string,10);
	_itow_s(stats.Intelligence.amount,stats.Intelligence.string,10);
	_itow_s(stats.Dexterity.amount,stats.Dexterity.string,10);
	_itow_s(stats.Health.amount, stats.Health.string, 10);
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
	return answer->string;
}

int baseStats::changeStat(statWord stat, int addMe) //returns -1 on poorly requested stat, 0 otherwise.
{
	oneStat* statChanger = translateStatWord(stat);
	if (!statChanger)
	{
		callError(L"Attempted to translate unknown statWord at changeStat");
		return -1; //invalid statWord
	}

	statChanger->amount += addMe;
	if (statChanger->amount < 0)
		statChanger->amount = 0;
	if (statChanger->amount > 99)
		statChanger->amount = 99;
	_itow_s(statChanger->amount, statChanger->string, 10);
	SendMessage(statChanger->hwnd,WM_SETTEXT,FALSE,(LPARAM) statChanger->string);
	return 0;
}

int baseStats::changeStat(statWord stat, int amount, int modifier) //returns -1 on poorly requested stat, 0 otherwise.
{
	return baseStats::changeStat(stat, amount*modifier);
}

void baseStats::createBoxes(HWND hwnd)
{
	stats.Strength.hwnd = CreateWindow(TEXT("EDIT"), stats.Strength.string, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stats.Strength.x, stats.Strength.y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
	stats.Intelligence.hwnd = CreateWindow(TEXT("EDIT"), stats.Intelligence.string, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stats.Intelligence.x, stats.Intelligence.y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
	stats.Dexterity.hwnd = CreateWindow(TEXT("EDIT"), stats.Dexterity.string, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stats.Dexterity.x, stats.Dexterity.y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
	stats.Health.hwnd = CreateWindow(TEXT("EDIT"), stats.Health.string, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stats.Health.x, stats.Health.y, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
}

void baseStats::createButtons(HWND hwnd)
{
	createOneButton(ST, hwnd, ID_ST_ADD, ID_ST_SUB);
	createOneButton(IQ, hwnd, ID_IQ_ADD, ID_IQ_SUB);
	createOneButton(DX, hwnd, ID_DX_ADD, ID_DX_SUB);
	createOneButton(HT, hwnd, ID_HT_ADD, ID_HT_SUB);
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

void baseStats::engineReceiver(WORD identifier)
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
	default:				//otherwise it was a minus button
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
	default:
		break;
	}


	changeStat(target,modifier);
}
