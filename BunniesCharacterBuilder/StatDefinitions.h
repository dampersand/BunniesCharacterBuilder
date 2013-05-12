#ifndef _STAT_DEFINITIONS_H
#define _STAT_DEFINITIONS_H


//Anything that has the potential to break DPI-awareness is placed here for troubleshooting convenience later.
#define YSPACING 10 //vertical spacing between boxes
#define XSPACINGLONG 15 //Horizontal spacing between boxes
#define XSPACINGSHORT 5
#define TAB 80 //horizontal spacing, one 'tab'
#define BOXHEIGHT 20
#define BOXLENGTH 20
#define INITIALDISTANCE 10

#define BUTTONLOGIC 100 //logical distance between items in a statWord category.  Looks like may not need this, consider trimming in future revisions



typedef unsigned int statWord;

//note that all statWords are multiples of 100 away from their corresponding buttons.

//base statWords
#define ST 101
#define IQ 102
#define DX 103
#define HT 104

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

#endif //_STAT_DEFINITIONS_H guard word