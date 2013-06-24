#include "AdvCalc.h"


LRESULT calculateWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		return 0;


	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(m_hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

			RECT rectangle;
			POINT p;
			std::wstring text;
			SIZE textSize;

			GetClientRect(m_hwnd, &rectangle);
			switch (paintLogic) //TODO: Fill this out - 0 is nothing, 1 is standard window, 2 is simple window, 3 is attractiveness calculations...
			{
			case 0:
				break;
			case 1:
				text = current.label.c_str();
				GetTextExtentPoint32(hdc,text.c_str(), text.length(), &textSize);
				p.x = rectangle.right/2 - textSize.cx/2;
				p.y = rectangle.bottom/6;
				TextOut(hdc, p.x, p.y, text.c_str(), text.length());

				text = L"Level";
				GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
				p.x = rectangle.right/3 - textSize.cx - 2;
				p.y = rectangle.bottom/4;
				TextOut(hdc, p.x, p.y, text.c_str(), text.length());

				text = L"Cost";
				GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
				p.x = rectangle.right/3 - textSize.cx - 2;
				p.y += BOXHEIGHT + YSPACING;
				TextOut(hdc, p.x, p.y, text.c_str(), text.length());

				break;
				
			case 2:
				{
				text = L"Are you sure you want to spend ";
				text += std::to_wstring(current.amount);
				text += L" points on ";
				text += current.label;
				text += L"?";


				GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
				p.x = rectangle.right/2 - textSize.cx/2;
				p.y = rectangle.bottom/2;
				rectangle.top = rectangle.bottom/3;
				DrawTextEx(hdc, (LPWSTR)text.c_str(), text.length(), &rectangle, DT_CENTER | DT_WORDBREAK, NULL);
				}
				break;

			case 3:
				{
				text = current.label.c_str();
				GetTextExtentPoint32(hdc,text.c_str(), text.length(), &textSize);
				p.x = rectangle.right/2 - textSize.cx/2;
				p.y = rectangle.bottom/6;
				TextOut(hdc, p.x, p.y, text.c_str(), text.length());

				text = L"Potency";
				GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
				p.x = rectangle.right/3 - textSize.cx - 2;
				p.y = rectangle.bottom/4;
				TextOut(hdc, p.x, p.y, text.c_str(), text.length());

				text = L"Cost";
				GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
				p.x = rectangle.right/3 - textSize.cx - 2;
				p.y += BOXHEIGHT + YSPACING;
				TextOut(hdc, p.x, p.y, text.c_str(), text.length());
				}
				break;
			}
			EndPaint(m_hwnd, &ps);
		}
		return 0;

	case WM_CLOSE:
		{
			done = TRUE;
			return 0;
		}

		
	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
			switch(LOWORD(wParam))
			{
			case ID_CALC_OK:
				{
					if (current.x == 0)
						cancelStat();
					else
						commitStat();
					done = TRUE;
					return 0;
				}
			case ID_CALC_CANCEL:
				{
					cancelStat();
					done = TRUE;
					return 0;
				}
			case ID_CALC_PLUS:
				update(1);
				return 0;
			case ID_CALC_MINUS:
				update(-1);
				return 0;
			} //end switch LOWORD
		} // end switch HIWORD

	default:
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	}
	return TRUE;
}


bool calculateWindow::intro(bunnyStat* stat, messengerData* data, HWND mainWindow) //TODO: add skills at psionic ESP and beyond
{
	Create(L"Details", WS_POPUPWINDOW | WS_CAPTION, NULL, CALCWINDOWLOCX, CALCWINDOWLOCY, CALCWINDOWSIZEX, CALCWINDOWSIZEY, NULL, NULL);
	paintLogic = 0;
	statList = data;
	current = *stat;
	switch(current.identifier)
	{
	case ABT:
	case ABD:
	case AMB:
	case ANE:
	case CBR:
	case CNS:
	case CMP:
	case DGS:
	case DOJ:
	case ESS:
	case EMP:
	case HPT:
	case ITD:
	case INT:
	case LEP:
	case LON:
	case MAB:
	case NIV:
		{
			current.x = 1;
			current.y = current.amount;
			simpleBoxes();
			messageLoop(mainWindow);
		}
		break;

	case ATT:
		{
			appearanceBoxes();
			messageLoop(mainWindow);
		}
		break;
	
	case ACV:
	case ATS:
	case ACH:
	case ALT:
	case CHR:
	case CLI:
	case LGT:
	case LUK:
	case MTR:
		{
			standardBoxes();
			messageLoop(mainWindow);
		}
		break;

	default: //note that Enhanced Speed and Peripheral Vision fall here, as they are free.
		break;
	}
	EnableWindow(mainWindow, TRUE);
	DestroyWindow(m_hwnd);
	return commit;
}

void calculateWindow::messageLoop(HWND parent)
{
	done = FALSE;
	MSG msg;
	EnableWindow(parent, FALSE);
	ShowWindow(m_hwnd, SW_SHOW);

	for(done; done == FALSE; WaitMessage())
	{
		while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) //deals with POSTED quit messages only.
			{
				done = TRUE;
				PostMessage(NULL, WM_QUIT, NULL, NULL);
				break;
			}

			if (!IsDialogMessage(m_hwnd, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}//end While(PeekMessage)
	}//end for(done; done == FALSE)
}

void calculateWindow::simpleBoxes()
{
	paintLogic = 2;

	//temp variables
	RECT rectangle;
	POINT p;

	//setup OK button
	GetClientRect(m_hwnd, &rectangle);
	p.x = (rectangle.right/2 - XSPACINGLONG/2 - 3*BOXLENGTH);
	p.y = (3*rectangle.bottom/4);
	HWND ok = CreateWindow(TEXT("BUTTON"), TEXT("Yes"), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | BS_PUSHBUTTON | BS_DEFPUSHBUTTON, p.x, p.y, 2*BOXLENGTH, 1.5*BOXHEIGHT, m_hwnd, (HMENU) ID_CALC_OK, NULL, NULL);
	SetFocus(ok);

	//setup cancel button
	p.x = (rectangle.right/2 + XSPACINGLONG/2);
	CreateWindow(TEXT("BUTTON"), TEXT("No"), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | BS_PUSHBUTTON, p.x, p.y, 3*BOXLENGTH, 1.5*BOXHEIGHT, m_hwnd, (HMENU) ID_CALC_CANCEL, NULL, NULL);
}

void calculateWindow::standardBoxes()
{
	paintLogic = 1;

	//temp variables
	RECT rectangle;
	POINT p;

	//setup OK button
	GetClientRect(m_hwnd, &rectangle);
	p.x = (rectangle.right/2 - XSPACINGLONG/2 - 3*BOXLENGTH);
	p.y = (3*rectangle.bottom/4);
	HWND ok = CreateWindow(TEXT("BUTTON"), TEXT("OK"), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | BS_PUSHBUTTON | BS_DEFPUSHBUTTON, p.x, p.y, 2*BOXLENGTH, 1.5*BOXHEIGHT, m_hwnd, (HMENU) ID_CALC_OK, NULL, NULL);
	SetFocus(ok);

	//setup cancel button
	p.x = (rectangle.right/2 + XSPACINGLONG/2);
	CreateWindow(TEXT("BUTTON"), TEXT("Cancel"), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | BS_PUSHBUTTON, p.x, p.y, 3*BOXLENGTH, 1.5*BOXHEIGHT, m_hwnd, (HMENU) ID_CALC_CANCEL, NULL, NULL);

	//setup Level box
	p.x = rectangle.right/3;
	p.y = rectangle.bottom/4;
	levelBox = CreateWindow(TEXT("EDIT"), std::to_wstring(current.x).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE, p.x, p.y, BOXLENGTH, BOXHEIGHT, m_hwnd, (HMENU) 1, NULL, NULL);

	//setup +/- boxes
	p.x += BOXLENGTH + XSPACINGLONG;
	plusBox = CreateWindow(TEXT("BUTTON"), L"+", WS_CHILD | WS_VISIBLE | WS_BORDER, p.x, p.y, BOXLENGTH, BOXHEIGHT, m_hwnd, (HMENU) ID_CALC_PLUS, NULL, NULL);
	p.x += BOXLENGTH + XSPACINGSHORT;
	minusBox = CreateWindow(TEXT("BUTTON"), L"-", WS_CHILD | WS_VISIBLE | WS_BORDER, p.x, p.y, BOXLENGTH, BOXHEIGHT, m_hwnd, (HMENU) ID_CALC_MINUS, NULL, NULL);

	//setup Cost box
	p.x = rectangle.right/3;
	p.y += BOXHEIGHT + YSPACING;
	costBox = CreateWindow(TEXT("EDIT"), std::to_wstring(current.y).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE, p.x, p.y, BOXLENGTH, BOXHEIGHT, m_hwnd, (HMENU) 1, NULL, NULL);

	
	return;
};

void calculateWindow::update(int change)
{
	if (current.identifier == ATT)
	{
		if ((current.x == -3 && change < 0) || (current.x == 3 && change > 0))
			return;
		std::wstring text = L"";
		current.x += change; //x is requested level
		switch (current.x)
		{
		case -3:
			current.y = -20;
			text = L"Hideous";
			break;

		case -2:
			current.y = -10;
			text = L"Ugly";
			break;

		case -1:
			current.y = -5;
			text = L"Unattractive";
			break;
			
		case 0:
			current.y = 0;
			text = L"Average";
			break;

		case 1:
			current.y = 5;
			text = L"Attractive";
			break;

		case 2:
			current.y = 25;
			text = L"Handsome";
			break;
			
		case 3:
			current.y = 75;
			text = L"Very Handsome";
			break;
		}
		SendMessage(levelBox,WM_SETTEXT,FALSE,(LPARAM) text.c_str());
		SendMessage(costBox,WM_SETTEXT,FALSE,(LPARAM) std::to_wstring(current.y).c_str());
		return;
	}

	else if (current.identifier == CLI || current.identifier == LUK)
	{
		if (current.x == 0 && change < 0)
			return;
		else if (current.x == 2 && change > 0) //2 is max level
			return;
	}


	else if (current.x == 0 && change < 0)
		return;
	current.x += change; //x should be requested level
	current.y += change*current.amount; //y should be cost of this transaction
	SendMessage(levelBox,WM_SETTEXT,FALSE,(LPARAM) std::to_wstring(current.x).c_str());
	SendMessage(costBox,WM_SETTEXT,FALSE,(LPARAM) std::to_wstring(current.y).c_str());
}

void calculateWindow::commitStat()
{
	statList->editStat(&current);
	commit = TRUE;
}

void calculateWindow::cancelStat()
{
	current.x = 0;
	current.y = 0;
	commit = FALSE;
}

void calculateWindow::appearanceBoxes()
{
	paintLogic = 3;

	//temp variables
	RECT rectangle;
	POINT p;

	//setup OK button
	GetClientRect(m_hwnd, &rectangle);
	p.x = (rectangle.right/2 - XSPACINGLONG/2 - 3*BOXLENGTH);
	p.y = (3*rectangle.bottom/4);
	HWND ok = CreateWindow(TEXT("BUTTON"), TEXT("OK"), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | BS_PUSHBUTTON | BS_DEFPUSHBUTTON, p.x, p.y, 2*BOXLENGTH, 1.5*BOXHEIGHT, m_hwnd, (HMENU) ID_CALC_OK, NULL, NULL);
	SetFocus(ok);

	//setup cancel button
	p.x = (rectangle.right/2 + XSPACINGLONG/2);
	CreateWindow(TEXT("BUTTON"), TEXT("Cancel"), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | BS_PUSHBUTTON, p.x, p.y, 3*BOXLENGTH, 1.5*BOXHEIGHT, m_hwnd, (HMENU) ID_CALC_CANCEL, NULL, NULL);

	//setup Type box
	p.x = rectangle.right/3;
	p.y = rectangle.bottom/4;
	std::wstring text = L"Average";
	levelBox = CreateWindow(TEXT("EDIT"), text.c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE, p.x, p.y, 6*BOXLENGTH, BOXHEIGHT, m_hwnd, (HMENU) 1, NULL, NULL);

	//setup +/- boxes
	p.x += 6*BOXLENGTH + XSPACINGLONG;
	plusBox = CreateWindow(TEXT("BUTTON"), L"+", WS_CHILD | WS_VISIBLE | WS_BORDER, p.x, p.y, BOXLENGTH, BOXHEIGHT, m_hwnd, (HMENU) ID_CALC_PLUS, NULL, NULL);
	p.x += BOXLENGTH + XSPACINGSHORT;
	minusBox = CreateWindow(TEXT("BUTTON"), L"-", WS_CHILD | WS_VISIBLE | WS_BORDER, p.x, p.y, BOXLENGTH, BOXHEIGHT, m_hwnd, (HMENU) ID_CALC_MINUS, NULL, NULL);

	//setup Cost box
	p.x = rectangle.right/3;
	p.y += BOXHEIGHT + YSPACING;
	costBox = CreateWindow(TEXT("EDIT"), std::to_wstring(current.y).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE, p.x, p.y, 1.5*BOXLENGTH, BOXHEIGHT, m_hwnd, (HMENU) 1, NULL, NULL);

	
	return;
}