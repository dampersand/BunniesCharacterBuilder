//This cpp file is the file you should edit to change MainWindow.  It includes message logic.

#include "MainWindow.h"



LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
	case WM_CREATE:
		engine.bigFour.createBoxes(m_hwnd);
		engine.bigFour.createButtons(m_hwnd);
		return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(m_hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
			engine.bigFour.paintAll(hdc);
			EndPaint(m_hwnd, &ps);
		}
        return 0;

	case WM_COMMAND:
		{
			switch(HIWORD(wParam))
			{
			case BN_CLICKED:
				engine.buttonRouter(wParam);
				break;

			default:
				//handle an error, unknown command
				break;
			}
		}
		return 0;


    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}