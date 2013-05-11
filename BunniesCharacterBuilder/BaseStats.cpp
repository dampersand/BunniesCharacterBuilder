#include "BaseStats.h"
#include <stdlib.h>

baseStats::baseStats() //constructor
{
	stats.Strength.amount = 0;
	stats.Intelligence.amount = 0;
	stats.Dexterity.amount = 0;
	stats.Health.amount = 0;
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

	
	return 0;
}

int baseStats::changeStat(statWord stat, int amount, int modifier) //returns -1 on poorly requested stat, 0 otherwise.
{
	return baseStats::changeStat(stat, amount*modifier);
}

void baseStats::createBoxes(int x, int y, int height, HWND hwnd)
{
	stats.Strength.x = x + TAB;
	stats.Strength.y = y;
	stats.Intelligence.x = x + TAB;
	stats.Intelligence.y = y+height+SPACING;
	stats.Dexterity.x = x + TAB;
	stats.Dexterity.y = y+2*height+2*SPACING;
	stats.Health.x = x + TAB;
	stats.Health.y = y+3*height+3*SPACING;
	CreateWindow(TEXT("EDIT"), stats.Strength.string, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stats.Strength.x, stats.Strength.y, 20, height, hwnd, (HMENU) 1, NULL, NULL);
	CreateWindow(TEXT("EDIT"), stats.Intelligence.string, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stats.Intelligence.x, stats.Intelligence.y, 20, height, hwnd, (HMENU) 1, NULL, NULL);
	CreateWindow(TEXT("EDIT"), stats.Dexterity.string, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stats.Dexterity.x, stats.Dexterity.y, 20, height, hwnd, (HMENU) 1, NULL, NULL);
	CreateWindow(TEXT("EDIT"), stats.Health.string, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_READONLY, stats.Health.x, stats.Health.y, 20, height, hwnd, (HMENU) 1, NULL, NULL);
	InvalidateRect(hwnd, NULL, NULL);

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

	TextOut (hdc, paintStat->x - BOXDISTANCE, paintStat->y, paintStat->label.c_str(), paintStat->label.length());

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