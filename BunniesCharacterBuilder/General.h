#ifndef _GENERAL_H
#define _GENERAL_H

#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include "StatDefinitions.h"

struct genStat
{
	int amount; //number contained in the stat, if any
	std::wstring label; //label of stat
	std::wstring string;
	int x; //location of statbox
	int y;
	HWND hwnd; //hwnd = handle to the display box.
};

class generalInfo
{
private:
	genStat age, points, name;
	int xSize, ySize;
	HWND disableButton;
	bool committed; //returns true if stats are committed, false otherwise

	void statInitiator(genStat* stat, std::wstring lab, int X, int Y, int am); //helper function
	void createBox(HWND hwnd, genStat* stat, int length); //helper function to make boxes
	void createBoxText(HWND hwnd, genStat* stat, int length);
	void createButton(HWND hwnd, int x, int y); //x should be the xSize of the class, y should be the y of the lowest genStat
	void callError(std::wstring function); //utility function, interior
	void paintText(genStat* stat, HDC hdc); //helper function to paint text
	void toggleCommit();
	void commitStat(genStat* stat);

public:
	generalInfo();// constructor

	/*UI Member Functions*/
	void createBoxes(HWND hwnd, int x, int y);
	void paintAll(HDC hdc);

	/*Engine Member Functions*/
	int getYSize();
	int getXSize();
	int getAge();
	int changeAge(int newAge);
	void engineReceiver(WORD identifier);
};



#endif //_GENERAL_H guard word