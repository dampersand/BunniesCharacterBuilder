#include "StatDefinitions.h"

messengerData::messengerData()
{
	this->first = new bunnyStat;
	this->last = new bunnyStat;

	this->first = 0;
	this->last = 0;
}

//tested, working - pretty sure dummyStat and this->last already deleted; deleting them again throws error.
messengerData::~messengerData()
{
	bunnyStat* dummyStat = new bunnyStat;
    while (this->last != this->first)
    {
        dummyStat = this->last;
        this->last = this->last->prev;
        delete dummyStat;
    }
	delete this->first;
}

bool messengerData::isEmpty()
{
    if (this->first == 0 || this->last == 0)
        return true;
    return false;
}

//tested, working
void messengerData::insertStatFront(bunnyStat statToAdd)
{
	bunnyStat* oldFirst = this->first;
    this->first = new bunnyStat;
    this->first->addButton = statToAdd.addButton;
	this->first->subtractButton = statToAdd.subtractButton;
	this->first->amount = statToAdd.amount;
	this->first->hwnd = statToAdd.hwnd;
	this->first->identifier = statToAdd.identifier;
	this->first->label = statToAdd.label;
	this->first->string = statToAdd.string;
	this->first->x = statToAdd.x;
	this->first->y = statToAdd.y;
	this->first->next = oldFirst;
    this->first->prev = 0;
    if (isEmpty())
        this->last = this->first; //no longer empty, although last is now garbage data.
    else
        this->first->next->prev = this->first;
}

//tested, working
void messengerData::insertStatBack(bunnyStat statToAdd)
{
	bunnyStat* oldLast = this->last;
    this->last = new bunnyStat;
	this->last->addButton = statToAdd.addButton;
	this->last->subtractButton = statToAdd.subtractButton;
	this->last->amount = statToAdd.amount;
	this->last->hwnd = statToAdd.hwnd;
	this->last->identifier = statToAdd.identifier;
	this->last->label = statToAdd.label;
	this->last->string = statToAdd.string;
	this->last->x = statToAdd.x;
	this->last->y = statToAdd.y;
    this->last->prev = oldLast;
    this->last->next = 0;
    if (isEmpty())
        this->first = this->last; //no longer empty, although first is now garbage data.
    else
        this->last->prev->next = this->last;
}

void messengerData::removeStatFront()
{
	if (isEmpty())
		 return;
     bunnyStat* dummyStat;

    dummyStat = this->first;
    if (this->first != this->last)
    {
        this->first = this->first->next;
        this->first->prev = 0;
    }
    delete dummyStat;
}

void messengerData::removeStatBack()
{
	if (isEmpty())
		return;
	bunnyStat* dummyStat;

	dummyStat = this->first;
	if (this->first != this->last)
	{
		this->first = this->first->next;
		this->first->prev = 0;
	}
	delete dummyStat;
}

bunnyStat* messengerData::getStat(statWord stat)
{
	return getStat(stat, FALSE);
}

//tested, working
bunnyStat* messengerData::getStat(statWord stat, bool suppressErrorMessage)
{
	bunnyStat* dummyStat = 0;
	dummyStat = this->first;
	while(dummyStat->identifier != stat)
	{
		dummyStat = getNextStat(dummyStat);
		if (dummyStat == this->last && dummyStat->identifier != stat)
		{
			std::wstring errorMsg = L"messengerData could not find stat ";
			std::wstring appendMsg = std::to_wstring(stat);
			errorMsg.append(appendMsg);
			if (!suppressErrorMessage)
				MessageBox(NULL, errorMsg.c_str(), NULL, MB_OK | MB_TASKMODAL);
			return getStat(EMPTY);
		}
	}

	return dummyStat;
}

//tested, working
bunnyStat* messengerData::getNextStat(bunnyStat* currentCard)
{
	return currentCard->next;
}

//tested, working
bool messengerData::changeStat(statWord stat, int addMe)
{
	bunnyStat* dummyStat = getStat(stat);
	if (!dummyStat)
		return FALSE; //couldn't find stat to change
	dummyStat->amount += addMe;
	dummyStat->string = std::to_wstring(dummyStat->amount);
	dummyStat->updateStat();
	return TRUE;
}

//tested, working
bool messengerData::changeStat(statWord stat, std::wstring newString)
{
	bunnyStat* dummyStat = getStat(stat);
	if (!dummyStat)
		return FALSE; //couldn't find stat to change
	dummyStat->string = newString;
	SendMessage(dummyStat->hwnd,WM_SETTEXT,FALSE,(LPARAM) dummyStat->string.c_str());
	return TRUE;
}


//tested, working
void messengerData::changeStatLocation(statWord stat, int x, int y)
{
	bunnyStat* dummyStat = getStat(stat);
	dummyStat->x = x;
	dummyStat->y = y;
}

//tested, working
void messengerData::makeNewStat(statWord stat, int am, int X, int Y, std::wstring lab, std::wstring str, HWND hwnd, HWND add, HWND sub)
{
	bunnyStat dummyStat;
	dummyStat.amount = am;
	dummyStat.x = X;
	dummyStat.y = Y;
	dummyStat.label = lab;
	dummyStat.string = str;
	dummyStat.identifier = stat;
	dummyStat.hwnd = hwnd;
	dummyStat.addButton = add;
	dummyStat.subtractButton = sub;
	insertStatBack(dummyStat);
}

//tested, working
void messengerData::editStat(statWord stat, int am, int X, int Y, std::wstring lab, std::wstring str, HWND hwnd, HWND add, HWND sub)
{
	bunnyStat* dummyStat = getStat(stat);

	dummyStat->amount = am;
	dummyStat->x = X;
	dummyStat->y = Y;
	dummyStat->string = str;
	dummyStat->label = lab;
	dummyStat->hwnd = hwnd;
	dummyStat->addButton = add;
	dummyStat->subtractButton = sub;
	SendMessage(dummyStat->hwnd,WM_SETTEXT,FALSE,(LPARAM) dummyStat->string.c_str());
}

//tested, working
void messengerData::editStat(bunnyStat* stat)
{
	editStat(stat->identifier, stat->amount, stat->x, stat->y, stat->label, stat->string, stat->hwnd, stat->addButton, stat->subtractButton);
}
