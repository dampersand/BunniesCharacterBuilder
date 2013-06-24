#ifndef _ADVCALC_H
#define _ADVCALC_H

#include <Windows.h>
#include <windowsx.h>
#include "WindowWrapper.h"
#include "StatDefinitions.h"

class calculateWindow : public BaseWindow<calculateWindow>
{

public:

	//required by BaseWindow abstract class
    PCWSTR  ClassName() const { return L"Details"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool intro(bunnyStat* stat, messengerData* data, HWND mainWindow);

private:
	bunnyStat current;
	bool done, commit;
	HWND levelBox, costBox, plusBox, minusBox;
	int paintLogic; //integer to inform WM_PAINT what type of window we have. 0 is none, 1 is standard
	int cost;
	messengerData* statList;

	void messageLoop(HWND parent); //sets the program to be code-modal until the UI is done.
	void standardBoxes();
	void simpleBoxes();
	void appearanceBoxes();
	void update(int change);
	void commitStat();
	void cancelStat();
};

#endif _ADVCALC_H