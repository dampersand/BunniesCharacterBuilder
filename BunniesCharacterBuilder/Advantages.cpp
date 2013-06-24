#include "Advantages.h"


advantages::advantages(messengerData &data)
{
	statList = &data;
	xSize = 0;
	ySize = 0;
	xStart = 0;
	yStart = 0;
	displayBox = 0;
	describeBox = 0;
	dropBox = 0;
	addButton = 0;
	removeButton = 0;
	costBox = 0;
	currentCostBox = 0;
	paintFlag = FALSE;
	createAdvantages();
}

void advantages::createBoxes(HWND hwnd)
{
	//temporary vars
	int x = xStart;
	int y = yStart + BOXHEIGHT; //factor in for size of paint
	int dropHeight, dropLength, displayHeight, displayLength, displayX, displayY, describeHeight, describeLength, describeX, describeY, costX, costY, currentCostX, currentCostY;
	dropLength = 4*TAB;
	dropHeight = 10*BOXHEIGHT;
	describeX = x;
	describeY = y + dropHeight + 3*YSPACING;
	describeLength = 4*TAB - BOXLENGTH - XSPACINGLONG;
	describeHeight = 4*BOXHEIGHT;
	displayX = x + dropLength + BOXLENGTH + 2*XSPACINGSHORT;
	displayY = y;
	displayLength = 3*TAB;
	displayHeight = 15*BOXHEIGHT;
	costX = describeX + describeLength + XSPACINGSHORT;
	costY = describeY;
	currentCostX = displayX + displayLength + XSPACINGSHORT;
	currentCostY = displayY;

	bunnyStat* dummyStat = NULL;
	//RECT mostRecentRectangle;

	//create initial list of advantages box
	dropBox = CreateWindow(TEXT("LISTBOX"), L"", WS_CHILD | WS_VISIBLE | LBS_NOTIFY  | WS_VSCROLL | WS_BORDER, x, y, dropLength, dropHeight, hwnd, (HMENU) ID_DROPBOX, NULL, NULL);

	//populate list of advantages
	SendMessage(dropBox, LB_ADDSTRING, NULL, (LPARAM) L"                  ------Advantages------"); //make this UI driven, dummy
	int counter = ADVANTAGES + 1;
	int pos;
	while (counter < DISADVANTAGES) 
	{
		dummyStat = statList->getStat(counter, TRUE);
		if (dummyStat->identifier != EMPTY)
		{
			pos = SendMessage(dropBox, LB_ADDSTRING, NULL, (LPARAM) dummyStat->label.c_str());
			SendMessage(dropBox, LB_SETITEMDATA, pos, (LPARAM) dummyStat->identifier);
		}
		counter++;
	}


	//Add costbox
	//GetWindowRect(dropBox, &mostRecentRectangle); //this is a way to make it UI driven, but I need to do a DPI conversion.
	costBox = CreateWindow(TEXT("EDIT"), L"0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, costX, costY, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);

	//add current cost box
	//TODO: Make this UI driven in the future
	currentCostBox = CreateWindow(TEXT("EDIT"), L"0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, currentCostX, currentCostY, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) 1, NULL, NULL);

	//Add description box
	//TODO: Make this UI driven in the future
	describeBox = CreateWindow(TEXT("EDIT"), L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE, describeX, describeY, describeLength, describeHeight, hwnd, (HMENU) 1, NULL, NULL);

	//Add selected advantages box
	//TODO: Make this UI driven in the future
	displayBox = CreateWindow(TEXT("LISTBOX"), L"", WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL | LBS_SORT, displayX, displayY, displayLength, displayHeight, hwnd, (HMENU) ID_DISPLAYBOX, NULL, NULL);

	//Add + & - buttons
	//TODO: Make this UI driven in the future
	addButton = CreateWindow(TEXT("BUTTON"), TEXT("->"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, x + 4*TAB + XSPACINGSHORT, y + 4*BOXHEIGHT, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) ID_ADV_ADD, NULL, NULL);
	removeButton = CreateWindow(TEXT("BUTTON"), TEXT("<-"), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, x + 4*TAB + XSPACINGSHORT, y + 5*BOXHEIGHT + YSPACING, BOXLENGTH, BOXHEIGHT, hwnd, (HMENU) ID_ADV_REM, NULL, NULL);
	Button_Enable(addButton, FALSE);
	Button_Enable(removeButton, FALSE);
}

void advantages::createAdvantages()
{
	statList->makeNewStat(ABD, 5, 0, 0, L"Absolute Direction", L"Can find north anywhere, gives a bonus to navigator skill", NULL, NULL, NULL);
	statList->makeNewStat(ABT, 5, 0, 0, L"Absolute Timing", L"Perfectly accurate internal clock", NULL, NULL, NULL);
	statList->makeNewStat(ACH, 2, 0, 0, L"Acute Hearing", L"+1 bonus to hearing roll per level", NULL, NULL, NULL);
	statList->makeNewStat(ATS, 5, 0, 0, L"Acute Taste and Smell", L"+1 bonus to taste and smell rolls per level. Affects many detection skills per level. Herbalism prereq", NULL, NULL, NULL);
	statList->makeNewStat(ACV, 2, 0, 0, L"Acute Vision", L"+1 bonus to vision roll per level", NULL, NULL, NULL);
	statList->makeNewStat(ALT, 8, 0, 0, L"Alertness", L"Adds bonus per level to any sense roll.  Affects many detection skills per level", NULL, NULL, NULL);
	statList->makeNewStat(AMB, 5, 0, 0, L"Ambidexterity", L"No penalty for offhand; can use both paws with equal skill", NULL, NULL, NULL);
	statList->makeNewStat(ANE, 15, 0, 0, L"Animal/Xenoempathy", L"Can roll to get a 'feel' for nonrabbits, bonus to zoology", NULL, NULL, NULL);
	statList->makeNewStat(ATT, 5, 0, 0, L"Attractiveness", L"Bonus to reaction rolls", NULL, NULL, NULL);
	statList->makeNewStat(CHR, 5, 0, 0, L"Charisma", L"+1 bonus to reaction roll per level; required to use enthrall ability", NULL, NULL, NULL);
	statList->makeNewStat(CLI, 5, 0, 0, L"Clerical Investment", L"Has a rank in an ordained religion, +1 to reactions from other religious types", NULL, NULL, NULL);
	statList->makeNewStat(CBR, 15, 0, 0, L"Combat Reflexes", L"+1 to Active Defense and Fast-Draw, +2 to Fright Check, immune to freeze, +6 to avoid surprise.  Party gets +1 to initiative (+2 if leading), ", NULL, NULL, NULL);
	statList->makeNewStat(CNS, 10, 0, 0, L"Common Sense", L"GM must roll against IQ to warn player if they start doing something stupid", NULL, NULL, NULL);
	statList->makeNewStat(CMP, 5, 0, 0, L"Composed", L"Hard to panic, calm.  +2 to any fright check (except phobia)", NULL, NULL, NULL);
	statList->makeNewStat(DGS, 15, 0, 0, L"Danger Sense", L"In ambush situations, GM secretly rolls against player's IQ and may alert player", NULL, NULL, NULL);
	statList->makeNewStat(DOJ, 5, 0, 0, L"Double Jointed", L"+3 to any rolls that involve climbing, wriggling out of something, or mechanic roll", NULL, NULL, NULL);
	statList->makeNewStat(ESS, 10, 0, 0, L"Eidectic Sense of Smell", L"Can take ten seconds to roll to attempt to memorize a particular smell, forcing GM to give information next time that smell occurs", NULL, NULL, NULL);
	statList->makeNewStat(EMP, 15, 0, 0, L"Empathy", L"Can roll to get a 'feel' for rabbits, psionic empathy prereq", NULL, NULL, NULL);
	statList->makeNewStat(EHS, 0, 0, 0, L"Enhanced Speed", L"Speed is doubled, as this character is a rabbit.  Free.", NULL, NULL, NULL);
	statList->makeNewStat(HPT, 10, 0, 0, L"High Pain Threshold", L"Immune from stun and DX penalty from combat wound (exception: crits, headblows). +3 to Will to ignore pain/resist torture", NULL, NULL, NULL);
	statList->makeNewStat(ITD, 10, 0, 0, L"Immunity to Disease", L"Immune to all infection (not parasites) - including forced.  MUST have HT 12 or better", NULL, NULL, NULL);
	statList->makeNewStat(INT, 15, 0, 0, L"Intuition", L"GM may roll (weighted by number of wrong choices) to steer player to a 'right' choice", NULL, NULL, NULL);
	statList->makeNewStat(LGT, 2, 0, 0, L"Language Talent", L"+2 bonus to learn a language per level, +1 bonus per level to linguistics", NULL, NULL, NULL);
	statList->makeNewStat(LEP, 5, 0, 0, L"Legal Enforcement Powers", L"Can enforce laws; has legal power. (Probably a member of Warren Watch)", NULL, NULL, NULL);
	statList->makeNewStat(LON, 5, 0, 0, L"Longevity", L"Long lifetime, only fail aging rolls on a 17 or 18. No points for 'age' disadvantage", NULL, NULL, NULL);
	statList->makeNewStat(LUK, 15, 0, 0, L"Luck", L"May ask GM to roll three times and return best result.  Can do this once per 60/level minutes", NULL, NULL, NULL);
	statList->makeNewStat(MAB, 10, 0, 0, L"Mathematical Ability", L"May count to eight (rabbits only count to four).  +2 to bartering", NULL, NULL, NULL);
	statList->makeNewStat(MTR, 5, 0, 0, L"Military Rank", L"1 rank per level of any military organization; 8 being highest", NULL, NULL, NULL);
	statList->makeNewStat(NIV, 10, 0, 0, L"Night Vision", L"Bunnies already see well in the dark, take this to see even better.  Ignore GM penalties due to darkness", NULL, NULL, NULL);
	statList->makeNewStat(PPV, 0, 0, 0, L"Peripheral Vision", L"Wider arc-of-vision.  If something happens behind you, GM rolls against your IQ to notice.  Active Defense only -2 from rear. Free", NULL, NULL, NULL);
	statList->makeNewStat(PES, 3, 0, 0, L"Psionic Power: ESP", L"Each level grants greater control over Psychometry, Precognition, and Seekersense", NULL, NULL, NULL);
	statList->makeNewStat(PTP, 5, 0, 0, L"Psionic Power: Telepathy", L"Each level grants greater control over telepathic skills", NULL, NULL, NULL);
	statList->makeNewStat(PEH, 5, 0, 0, L"Psionic Power: Empathic Healing", L"Each level grants greater control over healing skills", NULL, NULL, NULL);
	statList->makeNewStat(PRS, 2, 0, 0, L"Psionic Resistance", L"Each level reduces the potency of any psi abilities that include character in any capacity", NULL, NULL, NULL);
	statList->makeNewStat(RHL, 5, 0, 0, L"Rapid Healing", L"Add 5 to any HT rolls for healing.  Prerequisite: 10 or more HT", NULL, NULL, NULL);
	statList->makeNewStat(REP, 0, 0, 0, L"Reputation", L"Modifies reactions to character, includes special rules", NULL, NULL, NULL);
	statList->makeNewStat(STS, 5, 0, 0, L"Status", L"Each level of status increases character's social standing.  Most characters are 0, the king/queen is 3", NULL, NULL, NULL);
	statList->makeNewStat(STW, 4, 0, 0, L"Strong Will", L"Each level increases character's will roll by +1", NULL, NULL, NULL);
	statList->makeNewStat(TGH, 10, 0, 0, L"Toughness", L"Bestows a level of damage resistance to the character; take 1 less damage for each level up to 2.  Note: 10 points for first level, 25 points for second level", NULL, NULL, NULL);
	statList->makeNewStat(ULN, 10, 0, 0, L"Unaffected by Loud Noises", L"No fright checks or other adverse effects due to loud noises", NULL, NULL, NULL);
	statList->makeNewStat(UBK, 10, 0, 0, L"Unusual Background", L"Any other past-conferred advantage the player wishes the character to have.  Cost is variable", NULL, NULL, NULL);
	statList->makeNewStat(VCE, 10, 0, 0, L"Voice", L"Naturally clear, resonant voice. +2 to any singing/speaking skill; +2 reaction to anyone who can hear your voice", NULL, NULL, NULL);

}

void advantages::setAdvantage(statWord adv)
{
	//if the advantage has already been selected, return an error to the user
	int pos;
	int total = (int)SendMessage(displayBox, LB_GETCOUNT, NULL, NULL);
	std::wstring text = std::to_wstring(total);
	int result;
	for (pos = 0; pos <= total-1; pos++)
	{
		result = (int)SendMessage(displayBox,LB_GETITEMDATA, pos, NULL);
		if (result == adv)
		{
			MessageBox(NULL, TEXT("Character already possesses that advantage/disadvantage!"), NULL, MB_OK | MB_TASKMODAL);
			return;
		}
	}


	bunnyStat* dummyStat = statList->getStat(adv, 1);
	if (dummyStat->identifier == EMPTY)
		return;

	//do any extra calculations for the advantage in question, and make sure user presses "OK"
	if (!details.intro(dummyStat, statList, GetParent(dropBox)))
		return;

	//display the new advantage
	std::wstring type;
	type = dummyStat->label.c_str();
	if (dummyStat->identifier == ATT)
	{
		switch (dummyStat->x)
		{
		case -3:
			type += L": Hideous";
			break;

		case -2:
			type += L": Ugly";
			break;

		case -1:
			type += L": Unattractive";
			break;

		case 0:
			type += L": Average";
			break;

		case 1:
			type += L": Attractive";
			break;

		case 2:
			type += L": Handsome";
			break;

		case 3:
			type += L": Very handsome";
			break;
		}
		pos = SendMessage(displayBox, LB_ADDSTRING, NULL, (LPARAM) type.c_str());
	}
	else
		pos = SendMessage(displayBox, LB_ADDSTRING, NULL, (LPARAM) dummyStat->label.c_str());
	SendMessage(displayBox, LB_SETITEMDATA, pos, dummyStat->identifier);

	//change points based on cost of advantage
	statList->changeStat(PT, -(dummyStat->y));
}

void advantages::unsetAdvantage(int position)
{
	int result = (int)SendMessage(displayBox, LB_GETITEMDATA, position, NULL);
	bunnyStat* dummyStat = statList->getStat(result, TRUE);

	SendMessage(displayBox, LB_DELETESTRING, position, NULL);
	statList->changeStat(PT, dummyStat->y);
	
	//update removed stat
	dummyStat->y = 0;
	dummyStat->x = 0;
	statList->editStat(dummyStat);

}

void advantages::selectAdvantage(statWord adv)
{
	bunnyStat* dummyStat = statList->getStat(adv, TRUE);
	levelDependency(adv);

	SendMessage(describeBox, WM_SETTEXT, FALSE, (LPARAM) dummyStat->string.c_str());
	SendMessage(costBox, WM_SETTEXT, FALSE, (LPARAM) std::to_wstring(dummyStat->amount).c_str());
	SendMessage(currentCostBox, WM_SETTEXT, FALSE, (LPARAM) std::to_wstring(dummyStat->y).c_str());

}

void advantages::setStart(int x, int y)
{
	xStart = x;
	yStart = y;
}

int advantages::getXSize()
{
	return xSize;
}

int advantages::getYSize()
{
	return ySize;
}

void advantages::engineReceiver(WORD identifier, bool dependency)
{
	int pos;

	switch(identifier)
	{
	case ID_ADV_ADD: //adding an advantage
		pos = SendMessage(dropBox, LB_GETCURSEL, NULL, NULL);
		setAdvantage(SendMessage(dropBox, LB_GETITEMDATA, pos, NULL));
		break;

	case ID_ADV_REM:
		pos = SendMessage(displayBox, LB_GETCURSEL, NULL, NULL);
		unsetAdvantage(pos);
		break;

	case ID_DROPBOX:
		/*first deselect anything in displayBox*/
		SendMessage(displayBox, LB_SETCURSEL, -1, NULL);

		/*Now display what we're looking at in dropBox*/
		pos = SendMessage(dropBox, LB_GETCURSEL, NULL, NULL);
		selectAdvantage(SendMessage(dropBox, LB_GETITEMDATA, pos, NULL));
		break;

	case ID_DISPLAYBOX:
		/*first deselect anything in dropBox*/
		SendMessage(dropBox, LB_SETCURSEL, -1, NULL);

		/*Now display what we're looking at in displayBox*/
		pos = SendMessage(displayBox, LB_GETCURSEL, NULL, NULL);
		selectAdvantage(SendMessage(displayBox, LB_GETITEMDATA, pos, NULL));
		break;

	case ID_TOGGLE_GENERAL:
		toggleDisable(dependency);
		return;

	default:
		break;
	}
}

void advantages::paintText(HDC hdc)
{
	//temp variables
	RECT rectangle;
	std::wstring text;
	int x;
	SIZE textSize;
	POINT p; //make a point 0,0
	HWND hWndParent;

	//create title of dropbox
	//this is UI dependent!!
	GetClientRect(dropBox, &rectangle);
	text = L"Advantages and Disadvantages";
	GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
	x = (rectangle.right - rectangle.left - textSize.cx)/2;
	TextOut (hdc, xStart + x, yStart, text.c_str(), text.length());

	//create title of displaybox
	//this is UI dependent!!
	GetClientRect(displayBox, &rectangle);
	text = L"Active";
	GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
	hWndParent = GetParent(displayBox);
	p.x = 0;
	p.y = 0;
	MapWindowPoints(displayBox, hWndParent, &p, 1);
	x = (rectangle.right - rectangle.left - textSize.cx)/2;
	TextOut(hdc, p.x + x, yStart, text.c_str(), text.length());

	//create title of currentCostBox
	//this is UI dependent!!
	GetClientRect(currentCostBox, &rectangle);
	text = L"Current";
	GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
	hWndParent = GetParent(currentCostBox);
	p.x = 0;
	p.y = 0;
	MapWindowPoints(currentCostBox, hWndParent, &p, 1);
	x = (rectangle.right - rectangle.left - textSize.cx)/2;
	TextOut(hdc, p.x + x, p.y - 2*textSize.cy, text.c_str(), text.length());
	text = L"Cost";
	GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
	x = (rectangle.right - rectangle.left - textSize.cx)/2;
	TextOut(hdc, p.x + x, p.y - textSize.cy, text.c_str(), text.length());

	//create title of describebox
	//this is UI dependent!!
	GetClientRect(describeBox, &rectangle);
	text = L"Description";
	GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
	hWndParent = GetParent(describeBox);
	p.x = 0;
	p.y = 0;
	MapWindowPoints(describeBox, hWndParent, &p, 1);
	x = (rectangle.right - rectangle.left - textSize.cx)/2;
	TextOut(hdc, p.x + x, p.y - textSize.cy, text.c_str(), text.length());

	//create title of costBox
	//this is UI dependent!!
	GetClientRect(costBox, &rectangle);
	text = L"Cost";
	GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
	hWndParent = GetParent(costBox);
	p.x = 0;
	p.y = 0;
	MapWindowPoints(costBox, hWndParent, &p, 1);
	x = (rectangle.right - rectangle.left - textSize.cx)/2;
	TextOut(hdc, p.x + x, p.y - textSize.cy, text.c_str(), text.length());

	//create optional "per level" flag
	//this is UI dependent!!
	text = L"/lvl";
	hWndParent = GetParent(costBox); //get the handle to the costBox
	GetClientRect(costBox, &rectangle);
	p.x = 0;
	p.y = 0;
    MapWindowPoints(costBox, hWndParent, &p, 1); //map costBox's 0,0 in terms of costBox's parent 
	if (paintFlag)
		TextOut(hdc, p.x + rectangle.right - rectangle.left + 3, p.y, text.c_str(), text.length());

}

void advantages::levelDependency(statWord adv)
{
	bool original = paintFlag;
	switch(adv)
	{
	case ACH:
	case ATS:
	case ACV:
	case ALT:
	case CHR:
	case LGT:
	case MTR:
	case PES:
	case PTP:
	case PEH:
	case PRS:
	case STS:
	case STW:
	case TGH:
		paintFlag = TRUE;
		break;

	default:
		paintFlag = FALSE;
		break;
	}

	RECT rectangle;
	POINT p = {0};
	MapWindowPoints(costBox, GetParent(costBox), &p, 1);
	rectangle.top = p.y;
	rectangle.left = p.x;
	rectangle.right = p.x + BOXLENGTH + 35;
	rectangle.bottom = p.y + BOXHEIGHT;

	if (paintFlag != original)
		InvalidateRect(GetParent(describeBox), &rectangle, NULL);
}

void advantages::toggleDisable(bool dependency)
{
	if (dependency)
	{
		Button_Enable(addButton, TRUE);
		Button_Enable(removeButton, TRUE);
	}
	else
	{
		Button_Enable(addButton, FALSE);
		Button_Enable(removeButton, FALSE);
	}
}

void advantages::resetAdvantages()
{
	int total = (int)SendMessage(displayBox, LB_GETCOUNT, NULL, NULL);
	int pos;
	int result;
	bunnyStat* dummyStat;
	for (pos = 0; pos <= total-1; pos++)
	{
		result = (int)SendMessage(displayBox,LB_GETITEMDATA, pos, NULL);
		dummyStat = statList->getStat(result);
		dummyStat->x = 0;
		dummyStat->y = 0;
	}

	SendMessage(displayBox,LB_RESETCONTENT, NULL, NULL);
	setAdvantage(EHS);
	setAdvantage(PPV);
}
