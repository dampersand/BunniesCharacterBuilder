#include "Engine.h"


Engine::Engine() : general(data), derivedStats(data), bigFour(data), advantageStats(data)
{
	UIx = INITIALDISTANCE;
	UIy = INITIALDISTANCE;
	column1Width = INITIALDISTANCE;
	points = 0;
	age = 0;
	data.makeNewStat(EMPTY,0,0,0,L"EMPTY", L"",0,0,0); //empty stat

	/*suppress those annoying error messages in the beginning, at least until skills get coded.*/
	data.makeNewStat(BF,0,0,0,L"EMPTY",L"0",0,0,0);
	data.makeNewStat(BW,0,0,0,L"EMPTY",L"0",0,0,0);
	data.makeNewStat(FAT,0,0,0,L"EMPTY",L"0",0,0,0);
	data.makeNewStat(RN,0,0,0,L"EMPTY",L"0",0,0,0);
	
}

int Engine::buttonRouter(WPARAM wParam)//helper functions are for suckers.
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

		if (general.isCommitted())
		{
			int result = MessageBox(NULL, L"Unlocking these stats MAY clear any work you've done (depending on the changes you make).  Continue?", L"Note", MB_YESNO | MB_TASKMODAL);
			if (result == 7)
				break;
			recordDependency();
		}

		general.engineReceiver(LOWORD(wParam)); //send the call to the general stats
		if (!checkDependency() && general.isCommitted()) //if the user changed the age or points
		{
			bigFour.resetStats();
			advantageStats.resetAdvantages();
		}
		bigFour.engineReceiver(LOWORD(wParam), general.isCommitted()); //notify main stats that a predecessor has locked in.
		advantageStats.engineReceiver(LOWORD(wParam), general.isCommitted()); //notify advantages that a predecessor has locked in
		break;


	case ID_ADV_ADD: //if the add advantage button made the call
	case ID_ADV_REM:
	case ID_DROPBOX: //if the dropbox made the call
		advantageStats.engineReceiver(LOWORD(wParam), general.isCommitted());
		break;

	default:
		MessageBox(NULL, L"Unrecognized word", NULL, MB_OK | MB_TASKMODAL);
		return -1; //handle an error here
	}
	return 0;
}

int Engine::boxRouter(WPARAM wParam) //helper functions are for suckers.
{
	switch(LOWORD(wParam))
	{
	case ID_DROPBOX: //if the dropbox made the call
		advantageStats.engineReceiver(LOWORD(wParam), general.isCommitted());
		break;

	case ID_DISPLAYBOX:
		advantageStats.engineReceiver(LOWORD(wParam), general.isCommitted());
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

void Engine::recordDependency()
{
	points = data.getStat(PT)->amount;
	age = data.getStat(AG)->amount;
}

bool Engine::checkDependency()
{
	if ((points == data.getStat(PT)->amount) && (age == data.getStat(AG)->amount))
		return TRUE;
	else
		return FALSE;
}