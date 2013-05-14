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
	bunnyStat speed, size, weight, dodge, claw, bite, kick, move, clawChance, biteChance, kickChance;
	int xSize, ySize;

	void statInitiator(bunnyStat* stat, std::wstring lab, int X, int Y, int am);
	bunnyStat* translateStatWord(statWord stat);
	void createBox(HWND hwnd, bunnyStat* stat, int length); //helper function to make boxes
	void callError(std::wstring function); //utility function, interior
	void paintText(bunnyStat* stat, HDC hdc); //helper function to paint text
	int thrustTable(int strength);
	int thrustTableMod(int strength);
	int swingTable(int strength);
	int swingTableMod(int strength);

public:
	derived();

	/*Engine member functions*/
	int getYSize();
	int getXSize();
	int getStat(statWord stat);
	void recalculateAll(messengerData data);
	void calculateStat(statWord stat, messengerData data);

	/*UI Member Functions*/
	void createBoxes(HWND hwnd, int x, int y);
	void paintAll(HDC hdc);
};
		


#endif //_DERIVED_STATS_H guard word