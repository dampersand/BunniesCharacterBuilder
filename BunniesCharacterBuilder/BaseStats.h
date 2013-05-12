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

public:
	baseStats(); //constructor

	/*member functions*/
	int changeStat(statWord stat, int amount);
	int changeStat(statWord stat, int amount, int modifier);

	int getStatInt(statWord stat);
	LPCWSTR getStatString(statWord stat);

	void createBoxes(HWND hwnd);
	void createButtons(HWND hwnd);
	void createOneButton(statWord stat, HWND hwnd, buttonID idUp, buttonID idDown); //cheap way to do this, but having trouble with my #defines

	void baseStats::paintAll(HDC hdc);
	void paintText(statWord stat, HDC hdc);

	oneStat* translateStatWord(statWord stat); //gets a pointer to the stat asked for by statWord

	void callError(std::wstring function);

	void engineReceiver(WORD identifier);

};

#endif // _BASE_STATS_H Guard word