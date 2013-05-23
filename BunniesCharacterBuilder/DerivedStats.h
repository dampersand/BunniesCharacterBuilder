#ifndef _DERIVED_STATS_H
#define _DERIVED_STATS_H

#include <Windows.h>
#include "StatDefinitions.h"
#include <string>
#include <sstream>
#include <iostream>

class derived
{
private:
	messengerData* statList;
	int xSize, ySize;
	int xStart, yStart;

	void createBox(HWND hwnd, bunnyStat* stat, int length); //helper function to make boxes
	void callError(std::wstring function); //utility function, interior
	void paintText(statWord stat, HDC hdc); //helper function to paint text
	int thrustTable(int strength);
	int thrustTableMod(int strength);
	int swingTable(int strength);
	int swingTableMod(int strength);

public:
	derived(messengerData &data);

	/*Engine member functions*/
	void recalculateAll();
	void calculateStat(statWord stat); //includes logic for calculation
	void engineReceiver(WORD identifier, bool dependency);

	/*UI Member Functions*/
	void createBoxes(HWND hwnd);
	void paintAll(HDC hdc);
	void setStart(int x, int y);
	int getXStart();
	int getYStart();
	int getYSize();
	int getXSize();
};
		


#endif //_DERIVED_STATS_H guard word