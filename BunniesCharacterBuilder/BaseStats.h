#ifndef _BASE_STATS_H
#define _BASE_STATS_H

#include <Windows.h>
#include <string>
#include "StatDefinitions.h"

struct oneStat
{
	int amount; //number contained in the stat
	wchar_t string[3]; //number as a string, because programming, I guess.
	std::wstring label; //label of stat
	int x; //location of statbox
	int y;
	HWND hwnd, addButton, subtractButton; //hwnd = handle to the display box.
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

	void createOneButton(statWord stat, HWND hwnd, buttonID idUp, buttonID idDown); //cheap way to do this, but having trouble with my #defines
	void paintText(statWord stat, HDC hdc); //helper function to paint a label
	oneStat* translateStatWord(statWord stat); //gets a pointer to the stat asked for by statWord
	void callError(std::wstring function); //utility function to alert the user.  Or programmer.  Whoever.

public:
	baseStats(); //constructor

	/*engine member functions*/
	int changeStat(statWord stat, int amount);
	int changeStat(statWord stat, int amount, int modifier);
	int getStatInt(statWord stat);
	LPCWSTR getStatString(statWord stat);
	void engineReceiver(WORD identifier);
	int getXSize();
	int getYSize();

	/*UI member functions*/
	void createBoxes(HWND hwnd, int x, int y); //receive parent and starting point
	void createButtons(HWND hwnd, int x, int y);
	void paintAll(HDC hdc);


};

#endif // _BASE_STATS_H Guard word