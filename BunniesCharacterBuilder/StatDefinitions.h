#ifndef _STAT_DEFINITIONS_H
#define _STAT_DEFINITIONS_H
#include <string>
#include <Windows.h>

typedef unsigned int statWord;

struct bunnyStat
{
	int amount, x, y;
	std::wstring label, string;
	statWord identifier;
	HWND hwnd, addButton, subtractButton; //handle to display box and buttons.  Leave buttons uninitialized for non base stats.
	bunnyStat *next, *prev;
	void updateStat() { SendMessage(hwnd,WM_SETTEXT,FALSE,(LPARAM) string.c_str()); }
};

class messengerData //dynamic list of bunny stats.  Currently set by using a statbuilder program, in the future will be file-driven.
{
	bunnyStat* first;
	bunnyStat* last;

	/*helper functions*/
	void insertStatFront(bunnyStat statToAdd);
	void insertStatBack(bunnyStat statToAdd);
	void removeStatFront();
	void removeStatBack();
	bool isEmpty();

	bunnyStat* getNextStat(bunnyStat* currentCard);

public:
	messengerData(); //c-tor
	~messengerData(); //d-tor
	
	/*engine functions*/
	bunnyStat* getStat(statWord stat);
	bool changeStat(statWord stat, int addMe);
	bool changeStat(statWord stat, std::wstring newString);
	void changeStatLocation(statWord stat, int x, int y);
	void makeNewStat(statWord stat, int am, int X, int Y, std::wstring lab, std::wstring str, HWND hwnd, HWND add, HWND sub);
	void editStat(statWord stat, int am, int X, int Y, std::wstring str, std::wstring lab, HWND hwnd, HWND add, HWND sub);
	void editStat(bunnyStat* stat);

};

//Anything that has the potential to break DPI-awareness is placed here for troubleshooting convenience later.
#define YSPACING 10 //vertical spacing between boxes
#define XSPACINGLONG 15 //Horizontal spacing between boxes
#define XSPACINGSHORT 5
#define TAB 80 //horizontal spacing, one 'tab'
#define BOXHEIGHT 20
#define BOXLENGTH 20
#define LONGBOXLENGTH 40
#define INITIALDISTANCE 10

#define BUTTONLOGIC 100 //logical distance between items in a statWord category.  Looks like may not need this, consider trimming in future revisions



//note that all statWords are multiples of 100 away from their corresponding buttons.

//statWords - hopefully this will be file-driven in the future.
#define EMPTY 100 //an empty stat
#define ST 101
#define IQ 102
#define DX 103
#define HT 104
#define NM 105 //name
#define AG 106 //age
#define PT 107 //points
#define SPD 108 //speed
#define SZ 109 //size
#define WT 110 //weight
#define DG 111 //dodge
#define CW 112 //claw
#define BT 113 //bite
#define KK 114 //kick
#define MV 115 //move
#define CWC 116 //clawchance
#define BTC 117 //bitechance
#define KKC 118 //kickchance

//advantages and disadvantages
#define FAT 150 //Fat disadvantage

typedef unsigned int buttonID;
//button IDs
#define ID_ST_ADD 201
#define ID_IQ_ADD 202
#define ID_DX_ADD 203
#define ID_HT_ADD 204

#define ID_ST_SUB 301
#define ID_IQ_SUB 302
#define ID_DX_SUB 303
#define ID_HT_SUB 304

//utility button IDs
#define ID_TOGGLE_GENERAL 401
#define ID_TOGGLE_BASE 402

//skillwords
#define BF 501 //bunFu
#define BW 502 //brawl
#define RN 503 //running

#endif //_STAT_DEFINITIONS_H guard word