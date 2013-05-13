#include "General.h"
#include <stdlib.h>

generalInfo::generalInfo() //constructor
{
	statInitiator(&age, L"Age", 0, 0, 18);
	statInitiator(&points, L"Points", 0, 0, 0);
	statInitiator(&name, L"Name", 0, 0, 0);
	disableButton = NULL;
	committed = FALSE;

	xSize = 0;
	ySize = 0;
}

void generalInfo::statInitiator(genStat* stat, std::wstring lab, int X, int Y, int am)
{
	stat->amount = am;
	stat->label = lab;
	stat->x = X;
	stat->y = Y;
	stat->hwnd = 0;
	stat->string = std::to_wstring(stat->amount);
}

int generalInfo::getAge()
{
	return age.amount;
}

int generalInfo::changeAge(int newAge)
{
	if (newAge > 99 || newAge < 1)
	{
		//callError(L"Age outside of parameters.");
		return -1;
	}

	age.amount = newAge;
	age.string = std::to_wstring(age.amount);
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
	createBox(hwnd, &points, BOXLENGTH);

	xSize = name.x + 2*TAB;
	ySize += points.y + BOXHEIGHT;
	ySize += YSPACING;

	createButton(hwnd, xSize, max(max(points.y,age.y),name.y));
}

void generalInfo::createBoxText(HWND hwnd, genStat* stat, int length)
{
		stat->hwnd = CreateWindow(TEXT("EDIT"), stat->string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER, stat->x, stat->y, length, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
}

void generalInfo::createBox(HWND hwnd, genStat* stat, int length)
{
	stat->hwnd = CreateWindow(TEXT("EDIT"), stat->string.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER, stat->x, stat->y, length, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);
}

void generalInfo::createButton(HWND hwnd, int x, int y)
{
	disableButton = CreateWindow(TEXT("BUTTON"), TEXT("Commit"), WS_VISIBLE | WS_CHILD | WS_BORDER | BS_PUSHBUTTON, x - TAB - 2*BOXLENGTH, y - BOXHEIGHT, TAB + 2*BOXLENGTH, 1.5*BOXHEIGHT, hwnd, (HMENU) ID_TOGGLE_COMMIT, NULL, NULL);
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

void generalInfo::paintText(genStat* stat, HDC hdc)
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

void generalInfo::engineReceiver(WORD identifier)
{
	switch(identifier)
	{
	case ID_TOGGLE_COMMIT:
		toggleCommit();
		break;
	}
}

void generalInfo::toggleCommit()
{
	if (committed) //if the state is committed when the button was pressed, open the boxes up.
	{
		SendMessage(disableButton,WM_SETTEXT,FALSE,(LPARAM) L"Commit");
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
		SendMessage(disableButton,WM_SETTEXT,FALSE,(LPARAM) L"Uncommit");
		SendMessage(age.hwnd,EM_SETREADONLY,TRUE,NULL);
		SendMessage(name.hwnd, EM_SETREADONLY,TRUE,NULL);
		SendMessage(points.hwnd, EM_SETREADONLY,TRUE,NULL);
		committed = TRUE;
	}
}

void generalInfo::commitStat(genStat* stat)
{
	int length = GetWindowTextLengthW(stat->hwnd);
	LPWSTR string = NULL;
	GetWindowTextW(stat->hwnd, string, length);
	stat->string = string;
	stat->amount = _wtoi(stat->string.c_str());
}
