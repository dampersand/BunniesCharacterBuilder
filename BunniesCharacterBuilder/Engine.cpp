#include "Engine.h"


Engine::Engine()
{
	generalXStart = INITIALDISTANCE; //x & y locations of first general info
	generalYStart = INITIALDISTANCE;
	bigFourXStart = INITIALDISTANCE; //x & y locations of big four base stats
	bigFourYStart = INITIALDISTANCE;
	derivedStatsXStart = INITIALDISTANCE;
	derivedStatsYStart = INITIALDISTANCE;
}

int Engine::buttonRouter(WPARAM wParam) //helper functions are for suckers.
{
	switch(LOWORD(wParam))
	{
	case ID_ST_ADD:
	case ID_ST_SUB:
	case ID_IQ_ADD:
	case ID_IQ_SUB:
	case ID_DX_ADD:
	case ID_DX_SUB:
	case ID_HT_ADD:
	case ID_HT_SUB: //if a base-stat button increaser made the call

		bigFour.engineReceiver(LOWORD(wParam), data, general.isCommitted()); //send the call to the base stats to spend the points
		general.addStat(PT, data.points - general.getStat(PT)); //update general points number with the new one
		break;



	case ID_TOGGLE_GENERAL: //if the general toggle button made the call
		general.engineReceiver(LOWORD(wParam), !bigFour.isCommitted()); //send the call to the general stats along with dependency information
		bigFour.engineReceiver(LOWORD(wParam), data, general.isCommitted()); //notify main stats that a predecessor has locked in.

		if (general.isCommitted()) //update messengerData with new user input
			data.points = general.getStat(PT);
		break;


	case ID_TOGGLE_BASE: //if the base toggle button made the call
		bigFour.engineReceiver(LOWORD(wParam), data, general.isCommitted()); //send the call to the base stats along with dependency information
		general.engineReceiver(LOWORD(wParam), !bigFour.isCommitted()); //notify general stats that a dependent has locked in.

		if (bigFour.isCommitted()) //update messengerData and recalculate/display derived stats
		{
			data.strength = bigFour.getStatInt(ST);
			data.intelligence = bigFour.getStatInt(IQ);
			data.dexterity = bigFour.getStatInt(DX);
			data.health = bigFour.getStatInt(HT);
			derivedStats.recalculateAll(data);
		}
		break;


	default:
		MessageBox(NULL, L"shit, not a recognized word", NULL, NULL);
		return -1; //handle an error here
	}
	return 0;
}

void Engine::initializeGeneral(HWND hwnd)
{
	general.createBoxes(hwnd, generalXStart, generalYStart);
	bigFourYStart = general.getYSize();
}

void Engine::initializeBase(HWND hwnd)
{
	bigFourYStart = general.getYSize() + 3*YSPACING;
	bigFour.createBoxes(hwnd, bigFourXStart, bigFourYStart);
	bigFour.createButtons(hwnd);
}

void Engine::initializeDerived(HWND hwnd)
{
	derivedStatsYStart = bigFour.getYSize() + 4*YSPACING;
	derivedStats.createBoxes(hwnd, derivedStatsXStart, derivedStatsYStart);
	messengerData initCalc;
	initCalc.strength = bigFour.getStatInt(ST);
	initCalc.intelligence = bigFour.getStatInt(IQ);
	initCalc.dexterity = bigFour.getStatInt(DX);
	initCalc.health = bigFour.getStatInt(HT);
	initCalc.bunFu = 0;
	derivedStats.recalculateAll(initCalc);
}

void Engine::paintBase(HDC hdc)
{
	bigFour.paintAll(hdc);
}

void Engine::paintGeneral(HDC hdc)
{
	general.paintAll(hdc);
}

void Engine::paintDerived(HDC hdc)
{
	derivedStats.paintAll(hdc);
}