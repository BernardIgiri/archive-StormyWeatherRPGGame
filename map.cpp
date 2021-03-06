// H E A D E R S
#include <stdio.h>
#include <alloc.h>
#include <bios.h>
#include <graphics.h>
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <signal.h> // might not use
#include <palette.h> // may use

// A R R O W   K E Y S

#define Down 20704
#define Left 19424
#define Right 19936
#define Up 18656

// K E Y   P A D   K E Y S

#define KeyPad_Slash -8145
#define KeyPad_Star 14122
#define KeyPad_Minus 18989
#define KeyPad_Plus 20011
#define KeyPad_Enter -8179

#define KeyPad_Period 21294

#define KeyPad_0 21040
#define KeyPad_1 20273
#define KeyPad_2 20530
#define KeyPad_3 20787
#define KeyPad_4 19252
#define KeyPad_5 19509
#define KeyPad_6 19766
#define KeyPad_7 18231
#define KeyPad_8 18488
#define KeyPad_9 18745

#define KeyPad_Period_Num_Lock_Off 21248

#define KeyPad_0_Num_Lock_Off 20992
#define KeyPad_1_Num_Lock_Off 20224
#define KeyPad_2_Num_Lock_Off 20480
#define KeyPad_3_Num_Lock_Off 20736
#define KeyPad_4_Num_Lock_Off 19200

#define KeyPad_6_Num_Lock_Off 19712
#define KeyPad_7_Num_Lock_Off 18176
#define KeyPad_8_Num_Lock_Off 18432
#define KeyPad_9_Num_Lock_Off 18688

// C O N S O L E   K E Y S

#define Insert 21216
#define Delete 21472
#define Home 18400
#define End 18488
#define Page_Up 18912
#define Page_Down 20960

// F U N C T I O N   K E Y S

#define F1 15104
#define F2 15360
#define F3 15616
#define F4 15872
#define F5 16128
#define F6 16384
#define F7 16640
#define F8 16896
#define F9 17152
#define F10 17408

//   E  N  D     O  F     K  E  Y  B  O  A  R  D     S  C  A  N     C  O  D  E  S
//-------------------------------------------------------------------------------//
// R E G U L A R   D E F I N I T I O N S
#define conversion_factor 65536 // long int / conversionfactor = short int
#define PI 3.14159265359 // Pi
#define textcoordinate textvsize()
#define menumargin 512

typedef float stat;
typedef char critstat;
typedef unsigned int idstat;
// Global varibles
extern stat constitution, spirit, agility, speed, strength, mind, gold;
extern critstat mp, hitpoints, poison, mapnumber, mapx, mapy, story, mission, town, townx, towny,
	itemquantity[100], equiptmenthealth[100];
extern idstat itemname[100], equiptmentname[100], equipped[12];
// Functions
extern char string_transfere[64]; // used to store strings to be passed by a function
extern void soundfx(int sfxnumber, int modify);
extern int pop_up_window(char *message[], char *options[], int lines, int choices, int mode, int music);
extern void menu_selection(int music);
extern void help_display(char message[]);
extern void loadmusic(char filename[]);
extern void playmusic(float time_passed);
extern void pause(int mode);
extern int probability (float percentage);
extern void load_image(char filename[], int x, int y);

//**************************** E N D   O F   I N I T I A L   C O D E *****************************

void Map (critstat mnum)
{
	return;
}
