#ifndef _STAT_DEFINITIONS_H
#define _STAT_DEFINITIONS_H
#include <string>

struct messengerData //structure to be passed around for calculation's sake
{
	int strength, intelligence, dexterity, health, speed, der2, der3, points, bunFu, brawl, fat, run;
	messengerData()
	{
		strength = 0;
		intelligence = 0;
		dexterity = 0;
		health = 0;
		speed = 0;
		der2 = 0;
		der3 = 0;
		points = 0;
		bunFu = 0;
		brawl = 0;
		fat = 0;
		run = 0;
	}
};

struct bunnyStat
{
	int amount, x, y;
	std::wstring label, string;
	HWND hwnd; //handle to display box
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



typedef unsigned int statWord;

//note that all statWords are multiples of 100 away from their corresponding buttons.

//base statWords
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

#endif //_STAT_DEFINITIONS_H guard word