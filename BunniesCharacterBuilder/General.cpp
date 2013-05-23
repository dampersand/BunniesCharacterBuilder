#include "General.h"
#include <stdlib.h>

generalInfo::generalInfo(messengerData &data) //constructor
{
	statList = &data;
	statList->makeNewStat(AG, 12, 0, 0, L"Age", L"12", 0, 0, 0);
	statList->makeNewStat(PT, 100, 0, 0, L"Points", L"100", 0, 0, 0);
	statList->makeNewStat(NM, 0, 0, 0, L"Name", L"", 0, 0, 0);
	disableButton = NULL;
	committed = FALSE;
	disabled = FALSE;

	xSize = 0;
	ySize = 0;
	xStart = 0;
	yStart = 0;
}

int generalInfo::addStat(statWord stat, int newStat)
{
	bunnyStat* answer = statList->getStat(stat);

	if (answer->identifier == EMPTY)
		callError(L"Empty pointer at generalInfo::addStat");

	answer->amount += newStat;

	if (newStat > 999 || newStat < -999)
	{
		callError(L"Stat outside of parameters.");
		answer->amount -= newStat;
		return -1;
	}

	answer->string = std::to_wstring(answer->amount);
	answer->updateStat();

	return 0;
}

void generalInfo::createBoxes(HWND hwnd)
{
	int x = xStart;
	int y = yStart;
	bunnyStat *name, *age, *points;
	name = statList->getStat(NM);
	age = statList->getStat(AG);
	points = statList->getStat(PT);
		
		
	name->x= x + TAB;
	name->y = y;
	age->x = x + TAB;
	age->y = name->y + BOXHEIGHT + YSPACING;
	points->x = x + TAB;
	points->y = age->y + BOXHEIGHT + YSPACING;

	createBoxText(hwnd, name, 2*TAB);
	createBox(hwnd, age, BOXLENGTH);
	createBox(hwnd, points, 1.5*BOXLENGTH);

	xSize = name->x + 2*TAB;
	ySize += points->y + BOXHEIGHT;
	ySize += YSPACING;

	statList->editStat(name);
	statList->editStat(points);
	statList->editStat(age);

	createButton(hwnd, xSize, max(max(points->y,age->y),name->y));
}

void generalInfo::createBoxText(HWND hwnd, bunnyStat* stat, int length)
{
	stat->hwnd = CreateWindow(TEXT("EDIT"), stat->string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER, stat->x, stat->y, length, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
}

void generalInfo::createBox(HWND hwnd, bunnyStat* stat, int length)
{
	stat->hwnd = CreateWindow(TEXT("EDIT"), stat->string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER, stat->x, stat->y, length, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
}

void generalInfo::createButton(HWND hwnd, int x, int y)
{
	disableButton = CreateWindow(TEXT("BUTTON"), TEXT("Lock"), WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON, x - 1.5*LONGBOXLENGTH, y - BOXHEIGHT, 1.5*LONGBOXLENGTH, 2*BOXHEIGHT, hwnd, (HMENU) ID_TOGGLE_GENERAL, NULL, NULL);
}

void generalInfo::callError(std::wstring function)
{
	MessageBox(NULL, function.c_str(), NULL, MB_OK);
}

void generalInfo::paintAll(HDC hdc)
{
	paintText(AG, hdc);
	paintText(PT, hdc);
	paintText(NM, hdc);
}

void generalInfo::paintText(statWord stat, HDC hdc)
{
	bunnyStat* answer = statList->getStat(stat);

	if (answer->identifier == EMPTY)
	{
		callError(L"Empty pointer at generalInfo::paintText");
		return;
	}
	SetTextAlign(hdc, TA_RIGHT);

	TextOut (hdc, answer->x - XSPACINGSHORT, answer->y, answer->label.c_str(), answer->label.length());

	SetTextAlign(hdc, TA_LEFT); //reset text align

}

int generalInfo::getXSize()
{
	return xSize;
}

int generalInfo::getYSize()
{
	return ySize;
}

void generalInfo::engineReceiver(WORD identifier, bool dependency)
{
	switch(identifier)
	{
	case ID_TOGGLE_GENERAL:
		if (dependency)
			toggleCommit();
		break;
	case ID_TOGGLE_BASE:
		toggleDisable(dependency);
		break;
	}
}

void generalInfo::toggleCommit()
{
	if (disabled) //insurance policy, should never happen.
	{
		callError(L"Dependent currently using thse functions.");
		return;
	}
	if (committed) //if the state is committed when the button was pressed, open the boxes up.
	{
		SendMessage(disableButton,WM_SETTEXT,FALSE,(LPARAM) L"Lock");
		SendMessage(statList->getStat(AG)->hwnd,EM_SETREADONLY,FALSE,NULL);
		SendMessage(statList->getStat(NM)->hwnd, EM_SETREADONLY,FALSE,NULL);
		SendMessage(statList->getStat(PT)->hwnd, EM_SETREADONLY,FALSE,NULL);
		committed = FALSE;
	}
	else //if the state was uncommitted when the button was pressed, close the boxes.
	{
		commitStat(NM);
		commitStat(AG);
		commitStat(PT);
		SendMessage(disableButton,WM_SETTEXT,FALSE,(LPARAM) L"Unlock");
		SendMessage(statList->getStat(AG)->hwnd,EM_SETREADONLY,TRUE,NULL);
		SendMessage(statList->getStat(NM)->hwnd, EM_SETREADONLY,TRUE,NULL);
		SendMessage(statList->getStat(PT)->hwnd, EM_SETREADONLY,TRUE,NULL);
		committed = TRUE;
	}
}

void generalInfo::commitStat(statWord stat)
{
	bunnyStat* answer = statList->getStat(stat);

	if (answer->identifier == EMPTY)
		callError(L"Empty pointer at generalInfo::commitStat");

	int length = GetWindowTextLengthW(answer->hwnd);
	std::wstring string = L"test";
	string.resize(length);
	GetWindowTextW(answer->hwnd, (LPWSTR)string.c_str(), length+1); //GetWindowText doesn't understand "Length," or GetWindowTextLength doesn't understand how to work.
	answer->string = string;
	answer->amount = _wtoi(answer->string.c_str()); //TODO: test this
}

bool generalInfo::isCommitted()
{
	return committed;
}

void generalInfo::toggleDisable(bool dependency)
{
	if (dependency)
	{
		Button_Enable(disableButton, TRUE);
		disabled = FALSE;
	}
	else
	{
		Button_Enable(disableButton, FALSE);
		disabled = TRUE;
	}
}

void generalInfo::setStart(int x, int y)
{
	xStart = x;
	yStart = y;
}

int generalInfo::getXStart()
{
	return xStart;
}

int generalInfo::getYStart()
{
	return yStart;
}