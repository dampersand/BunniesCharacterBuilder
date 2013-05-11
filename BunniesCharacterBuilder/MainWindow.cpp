//This cpp file is the file you should edit to change MainWindow.  It includes message logic.

#include "MainWindow.h"
#include "Engine.h"



LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
	case WM_CREATE:
		bigFour.createBoxes(10, 10, 20, m_hwnd);
		return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
		{
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
		bigFour.paintAll(hdc);
        EndPaint(m_hwnd, &ps);
		}
        return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}