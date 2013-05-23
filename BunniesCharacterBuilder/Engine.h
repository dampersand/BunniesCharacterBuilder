#ifndef __ENGINE_H
#define __ENGINE_H

#include "StatDefinitions.h"

#include "BaseStats.h"
#include "General.h"
#include "DerivedStats.h"
#include "Advantages.h"


class Engine
{
	messengerData data;
	generalInfo general;
	baseStats bigFour;
	derived derivedStats;
	advantages advantageStats;

	int UIx, UIy;
	int column1Width;

public:
	Engine();

	int buttonRouter(WPARAM wParam);
	int boxRouter(WPARAM wParam);

	void initializeGeneral(HWND hwnd);
	void initializeBase(HWND hwnd);
	void initializeDerived(HWND hwnd);
	void initializeAdvantages(HWND hwnd);
	void paintBase(HDC hdc);
	void paintGeneral(HDC hdc);
	void paintDerived(HDC hdc);
	void paintAdvantages(HDC hdc);

	void determineWidth();
};

#endif //__ENGINE_H guard word