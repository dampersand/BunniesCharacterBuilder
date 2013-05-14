#ifndef _GENERAL_H
#define _GENERAL_H

#include <Windows.h>
#include <windowsx.h>
#include <string>
#include "StatDefinitions.h"


class generalInfo
{
private:
	bunnyStat age, points, name;
	int xSize, ySize;
	HWND disableButton;
	bool committed, disabled; //returns true if stats are committed, false otherwise

	void statInitiator(bunnyStat* stat, std::wstring lab, int X, int Y, int am); //helper function
	void createBox(HWND hwnd, bunnyStat* stat, int length); //helper function to make boxes
	void createBoxText(HWND hwnd, bunnyStat* stat, int length);
	void createButton(HWND hwnd, int x, int y); //x should be the xSize of the class, y should be the y of the lowest bunnyStat
	void callError(std::wstring function); //utility function, interior
	void paintText(bunnyStat* stat, HDC hdc); //helper function to paint text
	void toggleCommit();
	void toggleDisable(bool dependency);
	void commitStat(bunnyStat* stat);
	bunnyStat* translateStatWord(statWord stat);

public:
	generalInfo();// constructor

	/*UI Member Functions*/
	void createBoxes(HWND hwnd, int x, int y);
	void paintAll(HDC hdc);

	/*Engine Member Functions*/
	int getYSize();
	int getXSize();
	int getStat(statWord stat);
	int addStat(statWord stat, int newStat);
	void engineReceiver(WORD identifier, bool dependency); //if dependency is false, user has a dependent open.
	bool isCommitted();
};



#endif //_GENERAL_H guard word