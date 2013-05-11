#ifndef _STAT_DEFINITIONS_H
#define _STAT_DEFINITIONS_H


//Anything that has the potential to break DPI-awareness is placed here for troubleshooting convenience later.
#define SPACING 10 //vertical spacing between boxes
#define TAB 80 //horizontal spacing, one 'tab'
#define BOXDISTANCE 4 //distance between stat labels and their boxes.



typedef unsigned int statWord;

//base statWords
#define ST 101
#define IQ 102
#define DX 103
#define HT 104

#endif //_STAT_DEFINITIONS_H guard word