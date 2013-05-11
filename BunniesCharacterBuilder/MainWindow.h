//Declares an object of type BaseWindow.  You may need to edit the class name in here.  Otherwise, edit MainWindow.cpp instead.

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H
#include <Windows.h>
#include "WindowWrapper.h"


class MainWindow : public BaseWindow<MainWindow>
{

public:

	//required by BaseWindow abstract class
    PCWSTR  ClassName() const { return L"Circle Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
#endif //_MAIN_WINDOW_H Guard Word