#ifndef _BASE_STATS_H
#define _BASE_STATS_H

#include <Windows.h>
#include <string>
#include "StatDefinitions.h"
#include <windowsx.h>

struct oneStat: public bunnyStat
{
	HWND addButton, subtractButton; //hwnd = handle to the buttons.
};

struct statList
	{
		oneStat Strength, Intelligence, Dexterity, Health;
	};



class baseStats
{
private:
	statList stats;
	int xSize, ySize;
	HWND disableButton;
	bool committed;
	bool disabled;

	void createOneButton(statWord stat, HWND hwnd, buttonID idUp, buttonID idDown); //cheap way to do this, but having trouble with my #defines
	void createToggleButton(HWND hwnd, int x, int y);
	void toggleCommit();
	void toggleDisable(bool dependency);
	void paintText(statWord stat, HDC hdc); //helper function to paint a label
	oneStat* translateStatWord(statWord stat); //gets a pointer to the stat asked for by statWord
	void callError(std::wstring function); //utility function to alert the user.  Or programmer.  Whoever.
	int getPointChange(int initialStat, int direction);

public:
	baseStats(); //constructor

	/*engine member functions*/
	int changeStat(statWord stat, int amount);
	int changeStat(statWord stat, int amount, int modifier);
	int getStatInt(statWord stat);
	LPCWSTR getStatString(statWord stat);
	void engineReceiver(WORD identifier, messengerData &data, bool dependency); //if dependency is false, user has not locked all predecessors first, or has a dependent open.
	int getXSize();
	int getYSize();
	bool isCommitted();

	/*UI member functions*/
	void createBoxes(HWND hwnd, int x, int y); //receive parent and starting point
	void createButtons(HWND hwnd);
	void paintAll(HDC hdc);


};

#endif // _BASE_STATS_H Guard word