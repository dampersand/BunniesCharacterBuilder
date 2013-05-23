#include "Engine.h"


Engine::Engine() : general(data), derivedStats(data), bigFour(data), advantageStats(data)
{
	UIx = INITIALDISTANCE;
	UIy = INITIALDISTANCE;
	column1Width = INITIALDISTANCE;
	data.makeNewStat(EMPTY,0,0,0,L"EMPTY", L"0",0,0,0); //empty stat

	/*suppress those annoying error messages in the beginning, at least until skills get coded.*/
	data.makeNewStat(BF,0,0,0,L"EMPTY",L"0",0,0,0);
	data.makeNewStat(BW,0,0,0,L"EMPTY",L"0",0,0,0);
	data.makeNewStat(FAT,0,0,0,L"EMPTY",L"0",0,0,0);
	data.makeNewStat(RN,0,0,0,L"EMPTY",L"0",0,0,0);
	
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

		bigFour.engineReceiver(LOWORD(wParam), general.isCommitted()); //send the call to the base stats to spend the points
		derivedStats.recalculateAll();
		break;

	case ID_TOGGLE_GENERAL: //if the general toggle button made the call
		general.engineReceiver(LOWORD(wParam), !bigFour.isCommitted()); //send the call to the general stats along with dependency information
		bigFour.engineReceiver(LOWORD(wParam), general.isCommitted()); //notify main stats that a predecessor has locked in.
		break;


	case ID_TOGGLE_BASE: //if the base toggle button made the call
		bigFour.engineReceiver(LOWORD(wParam), general.isCommitted()); //send the call to the base stats along with dependency information
		general.engineReceiver(LOWORD(wParam), !bigFour.isCommitted()); //notify general stats that a dependent has locked in.

		if (bigFour.isCommitted()) //update messengerData and recalculate/display derived stats
		{
			derivedStats.recalculateAll();
		}
		break;

	case ID_ADV_ADD: //if the add advantage button made the call
	case ID_ADV_REM:
	case ID_DROPBOX: //if the dropbox made the call
		advantageStats.engineReceiver(LOWORD(wParam));
		break;

	default:
		MessageBox(NULL, L"Unrecognized word", NULL, MB_OK);
		return -1; //handle an error here
	}
	return 0;
}

int Engine::boxRouter(WPARAM wParam) //helper functions are for suckers.
{
	switch(LOWORD(wParam))
	{
	case ID_DROPBOX: //if the dropbox made the call
		advantageStats.engineReceiver(LOWORD(wParam));
		break;

	default:
		return -1; //handle an error here
	}
	return 0;
}

void Engine::initializeGeneral(HWND hwnd)
{
	general.setStart(UIx, UIy);
	general.createBoxes(hwnd);
}

void Engine::initializeBase(HWND hwnd)
{
	bigFour.setStart(UIx, general.getYSize() + 3*YSPACING);
	bigFour.createBoxes(hwnd);
	bigFour.createButtons(hwnd);
}

void Engine::initializeDerived(HWND hwnd)
{
	derivedStats.setStart(UIx, bigFour.getYSize() + 4*YSPACING);
	derivedStats.createBoxes(hwnd);
	derivedStats.recalculateAll();
}

void Engine::initializeAdvantages(HWND hwnd)
{
	advantageStats.setStart(column1Width + XSPACINGLONG, UIy);
	advantageStats.createBoxes(hwnd);
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

void Engine::paintAdvantages(HDC hdc)
{
	advantageStats.paintText(hdc);
}

void Engine::determineWidth()
{
	column1Width = max(general.getXSize(), max(bigFour.getXSize(), derivedStats.getXSize()));
}