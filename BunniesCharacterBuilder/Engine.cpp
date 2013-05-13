#include "Engine.h"


Engine::Engine()
{
	generalXStart = INITIALDISTANCE; //x & y locations of first general info
	generalYStart = INITIALDISTANCE;
	bigFourXStart = INITIALDISTANCE; //x & y locations of big four base stats
	bigFourYStart = INITIALDISTANCE; //note: y start should be 
}

int Engine::buttonRouter(WPARAM wParam)
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
	case ID_HT_SUB: //if a base-stat button made the call
		bigFour.engineReceiver(LOWORD(wParam));
		break;
	case ID_TOGGLE_COMMIT:
		general.engineReceiver(LOWORD(wParam));
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
	bigFour.createBoxes(hwnd, bigFourXStart, bigFourYStart);
	bigFour.createButtons(hwnd, bigFourXStart, bigFourYStart);
}

void Engine::paintBase(HDC hdc)
{
	bigFour.paintAll(hdc);
}

void Engine::paintGeneral(HDC hdc)
{
	general.paintAll(hdc);
}