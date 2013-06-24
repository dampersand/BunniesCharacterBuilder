#ifndef _ADVANTAGES_H
#define _ADVANTAGES_H

#include <Windows.h>
#include <windowsx.h>
#include "StatDefinitions.h"
#include "AdvCalc.h"


class advantages
{
private:
	messengerData* statList; //note that 'amount' for these stats will be point cost!
	int xSize, ySize;
	int xStart, yStart;
	bool paintFlag; //flag to determine whether or not to paint "per level"
	HWND displayBox, describeBox, dropBox, addButton, removeButton, costBox, currentCostBox; //boxes to display, describe, select, add and remove advantages, and show cost.
	calculateWindow details;

	/*Engine Functions*/
	void createAdvantages(); //make this file driven
	void createDisadvantages(); //make this file driven
	void selectAdvantage(statWord adv); //populates displaybox and costbox on select
	void setAdvantage (statWord adv); //sets advantages and disadvantages.  BE SURE TO CREATE A HELPER FUNCTION FOR SCALING ADVANTAGES.
	void unsetAdvantage (int position);
	void levelDependency(statWord adv);
	void toggleDisable(bool dependency);

public:
	advantages(messengerData &data); //constructor

	/*UI Functions*/
	int getXSize();
	int getYSize();
	void setStart(int x, int y);
	void createBoxes(HWND hwnd);
	void paintText(HDC hdc);

	/*engine functions*/
	void engineReceiver(WORD buttonIdentifier, bool dependency);
	void resetAdvantages();

};


#endif