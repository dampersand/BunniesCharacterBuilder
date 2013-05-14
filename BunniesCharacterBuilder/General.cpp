#include "General.h"
#include <stdlib.h>

generalInfo::generalInfo() //constructor
{
	statInitiator(&age, L"Age", 0, 0, 12);
	statInitiator(&points, L"Points", 0, 0, 100);
	statInitiator(&name, L"Name", 0, 0, 0);
	disableButton = NULL;
	committed = FALSE;
	disabled = FALSE;

	xSize = 0;
	ySize = 0;
}

void generalInfo::statInitiator(bunnyStat* stat, std::wstring lab, int X, int Y, int am)
{
	stat->amount = am;
	stat->label = lab;
	stat->x = X;
	stat->y = Y;
	stat->hwnd = 0;
	stat->string = std::to_wstring(stat->amount);
}

int generalInfo::getStat(statWord stat)
{
	bunnyStat* answer = translateStatWord(stat);
	if (stat == NM)
		return -1; //consider handling an error
	return answer->amount;
}

int generalInfo::addStat(statWord stat, int newStat)
{
	bunnyStat* answer = translateStatWord(stat);

	answer->amount += newStat;

	if (newStat > 999 || newStat < -999)
	{
		callError(L"Stat outside of parameters.");
		answer->amount -= newStat;
		return -1;
	}

	answer->string = std::to_wstring(answer->amount);
	SendMessage(answer->hwnd,WM_SETTEXT,FALSE,(LPARAM) answer->string.c_str());

	return 0;
}

void generalInfo::createBoxes(HWND hwnd, int x, int y)
{
	name.x = x + TAB;
	name.y = y;
	age.x = x + TAB;
	age.y = name.y + BOXHEIGHT + YSPACING;
	points.x = x + TAB;
	points.y = age.y + BOXHEIGHT + YSPACING;

	createBoxText(hwnd, &name, 2*TAB);
	createBox(hwnd, &age, BOXLENGTH);
	createBox(hwnd, &points, 1.5*BOXLENGTH);

	xSize = name.x + 2*TAB;
	ySize += points.y + BOXHEIGHT;
	ySize += YSPACING;

	createButton(hwnd, xSize, max(max(points.y,age.y),name.y));
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
	paintText(&age, hdc);
	paintText(&points, hdc);
	paintText(&name, hdc);
}

void generalInfo::paintText(bunnyStat* stat, HDC hdc)
{
	if (!stat)
	{
		callError(L"Empty pointer at generalInfo::paintText");
		return;
	}
	SetTextAlign(hdc, TA_RIGHT);

	TextOut (hdc, stat->x - XSPACINGSHORT, stat->y, stat->label.c_str(), stat->label.length());

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
		SendMessage(age.hwnd,EM_SETREADONLY,FALSE,NULL);
		SendMessage(name.hwnd, EM_SETREADONLY,FALSE,NULL);
		SendMessage(points.hwnd, EM_SETREADONLY,FALSE,NULL);
		committed = FALSE;
	}
	else //if the state was uncommitted when the button was pressed, close the boxes.
	{
		commitStat(&name);
		commitStat(&age);
		commitStat(&points);
		SendMessage(disableButton,WM_SETTEXT,FALSE,(LPARAM) L"Unlock");
		SendMessage(age.hwnd,EM_SETREADONLY,TRUE,NULL);
		SendMessage(name.hwnd, EM_SETREADONLY,TRUE,NULL);
		SendMessage(points.hwnd, EM_SETREADONLY,TRUE,NULL);
		committed = TRUE;
	}
}


void generalInfo::commitStat(bunnyStat* stat)
{
	int length = GetWindowTextLengthW(stat->hwnd);
	std::wstring string = L"test";
	string.resize(length);
	GetWindowTextW(stat->hwnd, (LPWSTR)string.c_str(), length+1); //GetWindowText doesn't understand "Length," or GetWindowTextLength doesn't understand how to work.
	stat->string = string;
	stat->amount = _wtoi(stat->string.c_str()); //TODO: test this
}

bunnyStat* generalInfo::translateStatWord(statWord stat)
{
	if (stat == NM)
		return &name;
	else if (stat == AG)
		return &age;
	else if (stat == PT)
		return &points;
	else
		return NULL;
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