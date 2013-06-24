#ifndef _STAT_DEFINITIONS_H
#define _STAT_DEFINITIONS_H
#include <string>
#include <Windows.h>

typedef unsigned int statWord;

struct bunnyStat
{
	int amount, x, y; //amount of a stat and the stat's x and y location for regular stats OR cost of a stat and extra variables for advantages
	std::wstring label, string; //stat's label, descriptive string, and type of 
	statWord identifier;
	HWND hwnd, addButton, subtractButton; //handle to display box and buttons.  Leave buttons uninitialized for non base stats.
	bunnyStat *next, *prev;
	void updateStat() {SendMessage(hwnd,WM_SETTEXT,FALSE,(LPARAM) string.c_str());} //sends a message to the stat's box to display properly
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
	bunnyStat* getStat(statWord stat, bool suppressErrorMessage);
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

#define CALCWINDOWSIZEX 400
#define CALCWINDOWSIZEY 300
#define CALCWINDOWLOCX 600
#define CALCWINDOWLOCY 120

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
//for these stats, "amount" will be point cost.
#define ADVANTAGES 200 //start of advantages list
#define ABD 201 //absolute direction
#define ABT 202 //absolute timing
#define ACH 203 //acute hearing
#define ATS 204 //acute taste and smell
#define ACV 205 //acute vision
#define ALT 206 //alertness
#define AMB 207 //ambidexterity
#define ANE 208 //animal empathy
#define ATT 209 //attractiveness
#define CHR 210 //charisma
#define CLI 211 //clerical investment lolwut bunnies
#define CBR 212 //combat reflexes
#define CNS 213 //common sense
#define CMP 214 //composed
#define DGS 215 //danger sense
#define DOJ 216 //double jointed
//eidectic memory not allowed in bunnies games
#define ESS 217 //eidectic sense of smell
#define EMP 218 //empathy
#define EHS 219 //enhanced speed
#define HPT 220 //high pain threshold
#define ITD 221 //immunity to disease
#define INT 222 //intuition (not intelligence)
#define LGT 223 //language talent
#define LEP 224 //legal enforcement powers
//lightning calculator not allowed in bunnies games
//literacy not allowed in bunnies games
#define LON 225 //longevity
#define LUK 226 //luck
//magical aptitude not allowed in bunnies games
//magical resistance not allowed in bunnies games
#define MAB 227 //mathematical ability
#define MTR 228 //military rank
//musical ability not allowed in bunnies games
#define NIV 229 //night vision
#define PPV 230 //peripheral vision
#define PES 231 //psionic ESP
#define PTP 232 //psionic telepathy
#define PEH 233 //psionic empathic healing
#define PRS 234 //psionic resistance
#define RHL 235 //rapid healing
#define REP 236 //reputation
#define STS 237 //status
#define STW 238 //strong will
#define TGH 239 //toughness
#define ULN 240 //unaffected by loud noises
#define UBK 241 //unusual background
#define VCE 242 //voice
//wealth not allowed in bunnies games

#define DISADVANTAGES 250 //start of disadvantages list
#define FAT 251 //Fat disadvantage

typedef unsigned int buttonID;
//button IDs
#define ID_ST_ADD 301
#define ID_IQ_ADD 302
#define ID_DX_ADD 303
#define ID_HT_ADD 304

#define ID_ST_SUB 305
#define ID_IQ_SUB 306
#define ID_DX_SUB 307
#define ID_HT_SUB 308

#define ID_ADV_ADD 309
#define ID_ADV_REM 310

//utility button IDs
#define ID_TOGGLE_GENERAL 401
#define ID_CALC_OK 402
#define ID_CALC_CANCEL 403
#define ID_CALC_PLUS 404
#define ID_CALC_MINUS 405

//skillwords
#define BF 501 //bunFu
#define BW 502 //brawl
#define RN 503 //running

typedef unsigned int boxID;
//box IDs
#define ID_DROPBOX 601
#define ID_DISPLAYBOX 602

#endif //_STAT_DEFINITIONS_H guard word