#ifndef UNICODE
#define UNICODE
#endif 



#include <Windows.h>
#include "MainWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	BOOL WINAPI SetProcessDPIAware(void);
	//HRESULT hReturnValue = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE); //uncertain about that second DWORD
	//if(FAILED(hReturnValue))
	//	return 0; //way easier than including the entire program in 'succeeded' brackets.
	
	MainWindow win; //declare a window object.

	//create and register window "win"
	if (!win.Create(L"Bunnies and Burrows Character Creator", WS_OVERLAPPEDWINDOW)) //have the window create itself.  If it breaks, return.
		return 0;

	ShowWindow(win.Window(), nCmdShow); //show the window.

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg); //this will call windowProc of win, which in turn calls HandleMessage.  Boom.
    }

	//CoUninitialize();
    return 0;
}