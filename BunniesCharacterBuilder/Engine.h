#ifndef __ENGINE_H
#define __ENGINE_H

#include "BaseStats.h"
#include "General.h"

class Engine
{
	baseStats bigFour;
	generalInfo general;

	int generalXStart, generalYStart, bigFourXStart, bigFourYStart;

public:
	Engine();

	int buttonRouter(WPARAM wParam);
	void initializeGeneral(HWND hwnd);
	void initializeBase(HWND hwnd);
	void paintBase(HDC hdc);
	void paintGeneral(HDC hdc);
};

#endif //__ENGINE_H guard word