#ifndef _BASE_STATS_H
#define _BASE_STATS_H

#include <Windows.h>
#include <string>
#include "StatDefinitions.h"
#include <windowsx.h>


class baseStats
{
private:
	messengerData* statList;
	int xSize, ySize;
	int xStart, yStart;
	HWND disableButton;
	bool committed, disabled;

	void createOneButton(statWord stat, HWND hwnd, buttonID idUp, buttonID idDown); //cheap way to do this, but having trouble with my #defines
	void createToggleButton(HWND hwnd, int x, int y);
	void toggleCommit();
	void toggleDisable(bool dependency);
	void paintText(statWord stat, HDC hdc); //helper function to paint a label
	void callError(std::wstring function); //utility function to alert the user.  Or programmer.  Whoever.
	int getPointChange(int initialStat, int direction);
	void setStatButtonState(statWord stat, bool state);

public:
	baseStats(messengerData &data); //constructor

	/*engine member functions*/
	int changeStat(statWord stat, int amount);
	int changeStat(statWord stat, int amount, int modifier);
	void engineReceiver(WORD identifier, bool dependency); //if dependency is false, user has not locked all predecessors first, or has a dependent open.
	bool isCommitted();

	/*UI member functions*/
	void createBoxes(HWND hwnd); //receive parent and starting point
	void createButtons(HWND hwnd);
	void paintAll(HDC hdc);
	int getYSize();
	int getXSize();
	void setStart(int x, int y);
	int getXStart();
	int getYStart();


};

#endif // _BASE_STATS_H Guard word