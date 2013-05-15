#ifndef __ENGINE_H
#define __ENGINE_H

#include "StatDefinitions.h"

#include "BaseStats.h"
#include "General.h"
#include "DerivedStats.h"


class Engine
{
	messengerData data;
	generalInfo general;
	baseStats bigFour;
	derived derivedStats;

	int UIx, UIy;

public:
	Engine();

	int buttonRouter(WPARAM wParam);

	void initializeGeneral(HWND hwnd);
	void initializeBase(HWND hwnd);
	void initializeDerived(HWND hwnd);
	void paintBase(HDC hdc);
	void paintGeneral(HDC hdc);
	void paintDerived(HDC hdc);
};

#endif //__ENGINE_H guard word