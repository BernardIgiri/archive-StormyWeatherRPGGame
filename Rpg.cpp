// nothing yet but anyway

// H E A D E R S
#include <stdio.h>
#include <malloc.h>
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
#include <dir.h>
#include <process.h>
#include<mem.h>

#include "c:\bc45\newbgi\svgautil.h"
#include "c:\bc45\newbgi\svga64k.h"

#define TRANS_PUT 5
#define YES 1
#define NO 0

// D E F I N I T I O N S
/*	My Key Board Map
------------------------------------------------------------
use with (bios.h)
	_bios_keybrd(_KEYBRD_READY)
		to check if a key is ready to be read
	_bios_keybrd(_NKEYBRD_READ)
		to read scan code
		as an unsigned
		(unsigned char) - to get ASCII
		(char) - Get ASCII charecter
------------------------------------------------------------
		  Key		code
------------------------------------------------------------
*/
//   K  E  Y  B  O  A  R  D     S  C  A  N     C  O  D  E  S

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
#define menumargin 672

typedef float stat;
typedef char critstat;
typedef unsigned int idstat;

struct WeaponData {
	float weight, penatration, durability, cost;
	int type, length, hands, handle;
	char description[64], name[32];
	};


// C H A R E C T E R   V A R I A B L E S
struct Save {
//stat life; // 32b, grows with and only with age - amount of HP REMOVED
stat constitution; // 32b, grows with survival of tough physical battles
	// (25%) and strenght(75%) - ability to resist physical damage
stat spirit; // 32b, grows with suvival of powerful magical battles (25%)
	// and mind(75%) - ability to resist magically inflicted damage
stat agility; // 32b, grows with greater use of evasive techniques (75%) and
	// speed(25%) - reaction time to dodge command
stat speed; // 32b, grows with greater use of light weapons and fast
	// attacks(90%) and forcefull attacks with light (resist free) weapons (10%)
	// - reaction time to attack command
stat strength; // 32b, grows with greater use of heavy equiptment (100%),
	// and heavy attacks on light targets - power in hits reduces affect of
	// weight on your speed
stat mind; // 32b, grows with greater use of magic - increases the effect of each
	// magic point (32767)
stat gold; // 32b, money possessed
critstat mp; // 8b, current magic points (percent total, battle only)
critstat hitpoints; // 8b, current hit point (used to store the percentage of total hitpoints)
critstat poison; // 8b, stores degree of poison (0 to 4x) [divide by 64]
// constitution, spirit, agility, speed, strength, mind (all =  16777216 * 5 at birth)
// gold, hitpoints, poison (50, 100, 0 respectively)
// mp (battle only) (100 always)
// 0-128 range for critstats (keep at 0-100)
// tally 280 bits or 35 Bytes 10 variables

// (x1max + 1)*x2 + x1 // intergration formula

// G A M E   V A R I A B L E S

critstat mapnumber; // 8b, current world map
critstat mapx; // 8b, x coordinate on world map
critstat mapy; // 8b, y coordinate on world map
critstat story; // 8b, point in story
critstat mission; // 8b, mission number

// in town/dungeon only
critstat townx; // 8b, x coordinate in town/dungeon
critstat towny; // 8b, y coordinate in town/dungeon
// tally 56 bits or 7 Bytes 7 variables

critstat map[64][35]; // used identifying space types for current map (world/town/dungeon)
//  17920 bytes or  2240 Bytes 1 array

// I T E M S   A R R A Y S

idstat itemname[100]; // stores the id number of all of your items
idstat equiptmentname[100]; // stores the id number of all of your equiptment
critstat itemquantity[100]; // stores quantity of each of you items
critstat equiptmenthealth[100]; // stores health of each peice of your equiptment

idstat equipped[12]; // {right_hand, left_hand, chest, arms, legs, head, item_1,
						//item_2, item_3, item_4, item_5, item_6} stores user settings
idstat history[100];
};

typedef struct Save PlayerData;
PlayerData Player;

critstat town;

// data on each item/equiptment peice is stored with in the program
// tally  4,800 bits or 600 Bytes

// G E N E R A L   U S E   S T O R A G E

char string_transfere[64]; // used to store strings to be passed by a function
//////// game palette
int palset[16][3] = {
	{0,0,0},			// 0 black
	{0,0,63},		// 1 blue
	{0,31,0},		// 2 green
	{0,31,31},		// 3 cyan
	{63,0,0},		// 4 red
	{31,0,31},		// 5 magenta
	 {48,21,0},		// 6 brown
	// {32,15,0}, // real brown
	// {63,31,0}, // orange
	{48,48,48},		// 7 light gray
	{32,32,32},		// 8 dark gray
	{15,15,63},		// 9 light blue
	{0,63,0},		// 10 light green
	{0,63,63},		// light cyan
	{63,15,15},		// light red
	{63,0,63},		// light magenta
	{63,63,0},		// yellow
	{63,63,63}};	// white
//*/


int menu_pick=0; // user menu selection

// S A V E   F I L E   A R R A Y

char save_file[9];

// M U S I C   V A R I B L E S   A N D   A R R A Y S

float totaltime=0, oldtime=0, tempo=.1;
int position=0, octave=4;
char song[400]; // Music Buffer

// game variable tally 336 bits or 42 Bytes
// game array tally  24876 bits or 3112 Bytes or 3 Kilo Bytes
// game memory requirments in initialization phase 25222 bits or 3154 Bytes or  3  Kilo Bytes
// total of 17 variables and 5 array

// Varibles saved in save file in their order
// 32 bits/varible for varibles 1 - 7, 8 bits/varible for varibles 8 - 14, 800 bit/array for arrays at 15-18
// life, constitution, spirit, agility, speed, strength, mind, hitpoints, poison, mapnumber
// mapx, mapy, story, mission, itemname[], equiptmentname[], itemquantity[], equiptmenthealth[]
// 3480 bits or 435 Bytes

//char *value = "    "; //stores converted data

/* code for getting and putting images /

void far *graphbuffer[2] // graphics buffer

getimage(x1, y1, x2, y2, graphbuffer[block]); // get image
putimage(x, y, graphbuffer[block], TRANS_PUT); // put image
farfree(graphbuffer[block]); // use to fre ram after usage

if ((buf[block] = farmalloc(imagesize_8b(x1, y1, x2, y2))) == NULL)
{
	closegraph();
	printf("Error: not enough heap space in save_screen().\n");
	exit(1);
}
*/
// F U N C T I O N   D E F I N I T I O N S
// gets the float value of a stored number
float convert_string_to_integer(char string[], int length);
// loads charecter data from file
int load_data(void);
// initializes graphics (640x350, 16 colors, 2 pages)
void initialize_graphics(void);
// looks up equiptment data
float get_equipped(idstat equipnumber, int request);
// pauses with nothing (0) or music (1);
void pause(int mode);
// Gets the current text height
int textvsize(void);
// for centering text graphically
void center(int beginx, int endx, int y, char enteredtext[]);
// for vertically centering text graphically
int findycenter(int lines);
// for converting floats to strings
void number_to_string(float thenumber, int digits, int format);
// for converting the char value to an int value
int charecter_to_integer(char charecter);
// Plays a musical note
void playnote(char note);
// Performs the next command in a Music string
void check_for_octave(void);
// Loads the music into the music buffer
void loadmusic(char filename[]);
// Plays Loaded music
void playmusic(float time_passed);
// Saves Player data
int save_player_data(void);
// plays a sound effect
void soundfx(int sfxnumber, int modify);
// displays a pop up window with text and options and it returns the users selection
int pop_up_window(char *message[], char *options[], int lines, int choices, int mode, int music);
// displays the players menu
int menu_display(int viewing, int music);
// allows the user to select the desired menu
void menu_selection(int music);
// displays a message in the help box
void help_display(char message[]);
// determins via percentage wheather or not something happens
int probability (float percentage);
// copy from (page) to the other page
void copy_page(int page);
// switches from page to page
void switch_page(int page);
// updates music playing
void music_check (int music, time_t start);
// fades to black
void fadeout(void);
// takes a substring from one string and places it in the result string
void isolate_string(int start_byte, int end_byte, const char *string, char *result);
// equips your equipment or permits user selection
int equip_them(int music, int told_to);
// loads user selected file
void load_new(void);
// allows the user to select an item, returns number of item on list
int select_item(int music);
// the menu for item commands
int item_menu(int music);
// uses non-battle items
void use_item(idstat id);
// gets on data item
void get_item(idstat id, char *name, char *description);
// prompts user to replace an item or peice of equiptment
void replace (int equiptment, idstat id);
// finds where item or equiptment is in the list
int find (int equiptment, idstat id);
// gives equipment or item to player
void give_player (int equiptment, idstat id);
// takes equipment or item from location
void take (int equiptment, int location);
// The end game sequence
void end_game(void);
// shows a pop up image at x y coodinates
void image_pop_up (int x, int y, int width, int height, int music, char image[], char message[]);

// E - X - T - E - R - N - A - L -  - F - U - N - C - T - I - O - N - S
extern void Map (critstat mnum);
// T O W N S
extern void town1(void);

// NEW GRAPHICS RELATED FUNCTIONS
void clear_buffer (void far *buffer,size_t size)
{
	char far *buf=(char far *)buffer;
	for (register size_t i=0; i<size; i++)
	{
		*(buf+i)=1;
		*(buf+i)=0;
	}
}

char palette[792];

int Find_White_Pixel ()
{
	int color=0;
	for (register int i = 0; i<256; i++)
	{
		if (palette[i*3]+palette[i*3+1]+palette[i*3+2]>palette[color*3]+palette[color*3+1]+palette[color*3+2])
			color=i;
			//return i;
	}
	return color;//-1;
}

unsigned long imagesize_8b(int x1, int y1, int x2, int y2)
{
	int width=x2-x1;
	int height=y2-y1;
	return 4+((long)width)*height;
}

int huge DetectVGA64k()
{
  return 4;
}

//************************************************************************************************
void ReadPCX( char *filename, int offsetx, int offsety, int load_pal )
{
	FILE *fptr;
  char header[128];
	register unsigned char c, runlen;
	int i, r;
  int x, y, w, h;
  register int tx, ty;
  int maxx, maxy;
  int drawing;


/* TEXT STEP 1 */

  /* Open the file */
  fptr = fopen(filename,"rb");
	if (fptr==NULL) {
	  printf( "\nError - Couldn't open PCX file %s!\n", filename );
	  return;
	  }

  /* Read the header information */
  r = fread( &header, 1, 128, fptr );
  if ( r != 128 ) {
	  printf( "\nError - Failed to read header!\n" );
	  fclose(fptr);
	  return;
	  }

  /* Check if this is a 256 colour file */
  if ( header[1] != 5 || header[3] != 8 ) {
	  printf( "Error - This doesn't appear to be a 256 colour file\n" );
	  fclose(fptr);
	  return;
	  }

  /* C readers - this is a quick way to get an 'int' from two chars */
  // but a struct would've been even easier and faster

  /* Get the top left x and y */
  memcpy( &x, &header[4], 2 );
  memcpy( &y, &header[6], 2 );

  /* Get the width and height */
  memcpy( &w, &header[8], 2 );
  memcpy( &h, &header[10], 2 );


/* TEXT STEP 2 */

  /* Get the check marker for 256-colour files */
	fseek(fptr,-769L,SEEK_END);
  c = fgetc(fptr);
	if (c!=0x0C) {
	 printf( "\nError - File is not a 256 colour VGA PCX file!\n" );
		fclose(fptr);
		return;
		}

/* TEXT STEP 3 */

  /* Read in the 256 colour palette */
	{
		for (register int clear_it=0;clear_it<792;clear_it++)
			palette[clear_it]=0;
	}
	r = fread( &palette, 1, 768, fptr );
	if (r != 768) {
	 printf( "\nError - Failed to read palette\n" );
		fclose(fptr);
		return;
		}


/* TEXT STEP 4 */

  /* Divide all the values by 4 */
  //	for (i=0;i<768;i++)	palette[i] /= 4; // not needed for an SVGA driver


/* TEXT STEP 5 */
	if (load_pal)
	{
		int ii=0;
		for (int col = 0; col<256; col++)
		{
			int red = palette[ii++],
				 green = palette[ii++],
				 blue = palette[ii++];
			setrgbpalette(col,red,green,blue);
		}
	}


/* TEXT STEP 6 */

	fseek(fptr,128L,SEEK_SET);


/* TEXT STEP 7 */

  tx = x;
  ty = y;
  maxx = x+w;
  maxy = y+h;

  /* as long as the variable "drawing" is set to "YES" (1), we are still
	  drawing the image */

  // added in for speed

  void far *image_cache; // buffer for putimage
  int ystep=5;// the larger the step size the faster the load but the the more ram it takes
					// also all loaded images must have a y dimension divisible by this factor
  size_t cache_size = imagesize_8b(x,y,maxx,ystep); // needed later
  if ((image_cache = farmalloc(cache_size+maxx+ystep*10)) == NULL) { // alocate extra space to...\/
	closegraph();
	printf("Error: Ran out of memory.\n");
	exit(1);
  }
  clear_buffer(image_cache,cache_size+maxx+ystep*10);// to prevent garbage from mess ing up the image
  {
	int x_res=maxx,
		 y_res=ystep;
	memcpy(image_cache,&x_res,2);
	memcpy((char *)image_cache+2,&y_res,2); // resolution to be passed to put image
  }/* */
  char far *draw_to_buffer = (char *)image_cache+4;//pointer for use to draw to the buffer
  int position=0; // position in the buffer
  int ry=0;

  // added for speed /\


  drawing = YES;
	while (drawing) {

/* TEXT STEP 8 */

	 c=fgetc(fptr);

	 /* Check for a run */
		if ((c & 0xc0) == 0xc0) {


  /* TEXT STEP 9 */

		/* Strip off the high bits to get the run length */
			runlen = c & 0x3f;


  /* TEXT STEP 10 */

			/* get the colour of the run */
			c=fgetc(fptr);
			}
		else
		/* if it's not a run, just set the run length to 1 */
		runlen = 1;

	 do {

  /* TEXT STEP 11 */

		//putpixel( tx, ty, c );
		*(draw_to_buffer+position++)=c;
		//position=(tx+ty*maxx)%(maxx*4);
		//*(draw_to_buffer+position)=c;
		if ((position-1)==cache_size) // if the cache is filled then draw the image
		{
			putimage(x+offsetx,ry+offsety,image_cache, ystep); // trans put the image on screen
			/*farfree(image_cache);// free the allocated memory
			  if ((image_cache = farmalloc(cache_size+maxx+ystep*10)) == NULL) { // alocate extra space to...\/
				closegraph();
				printf("Error: Ran out of memory.\n");
				exit(1);
			  }*/
			position=0; // reset position
			ry+=ystep;
		}


  /* TEXT STEP 12 */

		if ( ++tx > maxx ) {
			tx = x;
			ty++;
			}

		if ( ty > maxy ) drawing = NO;


  /* TEXT STEP 13 */

		} while ( drawing && --runlen > 0 );

	 }


  /* TEXT STEP 14 */
  farfree(image_cache);// free the allocated memory

  fclose(fptr);

}



//*********************************M U S I C   F U N C T I O N S**********************************
void loadmusic(char filename[]) // loads music
{
	// reset music varibles
	totaltime=0;
	oldtime=0;
	tempo=.1;
	position=0;
	octave=4;
	{
		for (int i = 0; i < 400; i++)
			song[i]=0;
	}
	// end of reset
	nosound();
	FILE *fptr;
	if ((fptr = fopen(filename, "r")) == NULL){
		printf("Cannot open %s.\n", filename);
		exit(1);
	}
	int c;
	int i=1;
	// ---------
	char all_the_chars[256], i2;
	all_the_chars[0] = 0;
	for (i = 1, i2 = 1; i2 != 0; i++, i2++)
		all_the_chars[i] = i2;
	all_the_chars[0] = 0;
	// ---------
	i = 0;
	while ((c=fgetc(fptr)) != EOF){
		song[i]=all_the_chars[c];
		song[i+1]=0;
		i++;
	}
	fclose(fptr);
	return;
}

void playmusic(float time_passed) // plays loaded music
{
	totaltime+=time_passed;
	if ((totaltime-oldtime)>(tempo))
	{
		oldtime=totaltime;
		while (song[position]=='o' || song[position]=='-' || song[position]=='t')
			check_for_octave();
		playnote(song[position]);
	}
	return;
}

void check_for_octave(void) // performs next command in music string
{
	switch (song[position])
	{
		case 'o':
			switch (song[position+1])
			{
				case '0':
					octave = 0; break;
				case '1':
					octave = 1; break;
				case '2':
					octave = 2; break;
				case '3':
					octave = 3; break;
				case '4':
					octave = 4; break;
				case '5':
					octave = 5; break;
				case '6':
					octave = 6; break;
				case '7':
					octave = 7; break;
				case '8':
					octave = 8; break;
				default:
					octave = 4; break;
			}
			position+=2; break;
		case '-':
			nosound();
			position++; break;
		case 't':
			switch (song[position+1])
			{
				case '0':
					tempo = 1; break;
				case '1':
					tempo = .9; break;
				case '2':
					tempo = .8; break;
				case '3':
					tempo = .7; break;
				case '4':
					tempo = .6; break;
				case '5':
					tempo = .5; break;
				case '6':
					tempo = .4; break;
				case '7':
					tempo = .3; break;
				case '8':
					tempo = .2; break;
				case '9':
					tempo = .1; break;
				default:
					tempo = .1; break;
			}
			position+=2; break;
		default:
			break;
	}
	return;
}

void playnote(char note) // plays a musical note
{
		float freq;
		switch (note)
		{
			case 'c':
				freq = (float)262 * pow(2,(float)octave - 2);
				sound(freq); break;
			case 'd':
				freq = (float)294 * pow(2,(float)octave - 2);
				sound(freq); break;
			case 'e':
				freq = (float)330 * pow(2,(float)octave - 2);
				sound(freq); break;
			case 'f':
				freq = (float)350 * pow(2,(float)octave - 2);
				sound(freq); break;
			case 'g':
				freq = (float)392 * pow(2,(float)octave - 2);
				sound(freq); break;
			case 'a':
				freq = (float)440 * pow(2,(float)octave - 2);
				sound(freq); break;
			case 'b':
				freq = (float)494 * pow(2,(float)octave - 2);
				sound(freq); break;
			case ' ':
				nosound(); break;
			case '.':
				position = -1;
				totaltime=0;
				oldtime=0;
			default:
				position = -1;
				totaltime=0;
				oldtime=0;
				break;
		}
		position++;
		return;
}
//*************************E N D   O F   M U S I C   F U N C T I O N S****************************

//**************************************************************************************************
//*********************************** I M A G E   L O A D I N G ************************************
void draw_image(char command[], int x, int y);
void Draw(char command[], int pos, int type, int x, int y);
void setuppallete(void);

void load_image(char filename[], int x, int y)
{
	FILE *fptr;
	if ((fptr = fopen(filename, "r")) == NULL){
		printf("Cannot open %s.\n", filename);
		exit(1);
	}
	int c = 0;
	int i=1;
	char command[200]=""; // to receive data
	// ---------
	char all_the_chars[256], i2;
	all_the_chars[0] = 0;
	for (i = 1, i2 = 1; i2 != 0; i++, i2++)
		all_the_chars[i] = i2;
	// ---------
	i = 0;
	for (int ioi=0; ioi<200; ioi++)
		command[ioi]=0;
	while ((c=fgetc(fptr)) != EOF){
		command[i]=(all_the_chars[c]);
		i++;
		if (c == ';') // end of command marked by a ";"
		{
			command[i]=0; // append the null charecter
			draw_image(command,x,y); // figures what to draw
			i = 0;
			strcpy(command,""); // clear, partially
			for (int ioi=0; ioi<200; ioi++)
				command[ioi]=0;
		}
	}
	fclose(fptr);
	return;
}

void draw_image(char command[], int x, int y)
{
	//int varibles[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	//int number_of_varibles = 0;
	for (int i = 0; i < strlen(command); i++)
	{
		//printf("\n%s",command);
		switch (command[i])
		{
			case '`': // comment
				i+= strlen(command);
			break;
			case 'l': // line
				Draw(command, i, 0,x,y);
			break;
			case 'c': // circle
				Draw(command, i, 1,x,y);
			break;
			case 'a': // arc
				Draw(command, i, 2,x,y);
			break;
			case 'e': // ellipse
				//printf("hit");
				Draw(command, i, 3,x,y);
			break;
			case 'p': // pollygon
				Draw(command, i, 4,x,y);
			break;
			case 'f': // filled ellipse
				Draw(command, i, 5,x,y);
			break;
			case 's': // pie slice
				Draw(command, i, 6,x,y);
			break;
			case 'g': // filled pollygon
				Draw(command, i, 7,x,y);
			break;
			case 'd': // flood fill
				Draw(command, i, 8,x,y);
			break;
		  //	case 'o': // load pallete
		  //	  pallete_command(command);
		  //	break;
			default:
				break;
		}
	} // end of for loop
	return;
}

void Draw(char command[], int pos, int type, int x, int y)
{
	//                 x1 y1 x2 y2 color style {solid=0, dotted=1, centered=2, dashed=3}
	//int varibles[6] = {0, 0, 0, 0, 0,     0}
	setlinestyle(0,0,1);
	int varibles[33] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	int v=0, var_num=0;
	char var[4] = "   ";
	for (int i = pos; i < strlen(command); i++)
	{
		var[4] = 0;
		if (v==4)
		{
			v=0;
			var[0] = ' ';
			var[1] = ' ';
			var[2] = ' ';
			var[3] = ' ';
			var_num++;
		}
		switch (command[i])
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': // if its a digit
			var[v]=command[i];
			v++;
			break;
			default:
			if (v>0)
			{
				varibles[var_num] = atoi(var);
				if(atoi(var)&&var_num); // do not know why but it makes it work
				var_num++;
				v=0;
				var[0] = ' ';
				var[1] = ' ';
				var[2] = ' ';
				var[3] = ' ';
			}
			break;
		}
	}
	int poly[30]; // ={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int inc=0;
	int av=0;
	for (int iv = 0; iv < 30; iv++)
	{
		if (((av/2)*2)==av) // if even
			inc=x;
		else
			inc=y;/**/

		poly[iv] = varibles[iv+3]+inc; // transfere
		av++;
	}
	if (type!=4 && type!=7)
	{
		varibles[0]+=x;
		varibles[1]+=y;
	}
	switch (type)
	{
		case 0: // line x1 y1 x2 y2 color style {solid=0, dotted=1, centered=2, dashed=3}
			setcolor(varibles[4]);
			setlinestyle(varibles[5],0,1);
			line(varibles[0],varibles[1],varibles[2]+x,varibles[3]+y);
		break;
		case 1: // circle x y radius color
			setcolor(varibles[3]);
			circle(varibles[0],varibles[1],varibles[2]);
		break;
		case 2: // arc x y startangle endangle radius color
			setcolor(varibles[5]);
			arc(varibles[0],varibles[1],varibles[2],varibles[3],varibles[4]);
		break;
		case 3: // elipse x y startangle endangle radius_x radius_y color
			setcolor(varibles[6]);
			ellipse(varibles[0],varibles[1],varibles[2],varibles[3],varibles[4],varibles[5]);
		break;
		case 4: // polygon color style number_of_points x1 y1 x2 y2 x3 y3 x...
			setcolor(varibles[0]);
			setlinestyle(varibles[1],0,1);
			// use the transfere
			drawpoly(varibles[2],poly);
		break;
		// filled objects
		case 5: // filled elipse x y radius_x radius_y color pattern
			setcolor(varibles[4]);
			setfillstyle(varibles[5],varibles[4]);
			// W I L L   N O T   W O R K
			fillellipse(varibles[0],varibles[1],varibles[2], varibles[3]);
		break;
		case 6: // pie slice x y startangle endangle radius color pattern
			setcolor(varibles[5]);
			setfillstyle(varibles[6],varibles[5]);
			pieslice(varibles[0],varibles[1],varibles[2],varibles[3],varibles[4]);
		break;
		case 7: // filledpolygon color style number_of_points x1 y1 x2 y2 x3 y3 x...
			setcolor(varibles[0]);
			setfillstyle(varibles[1],varibles[0]);
			// use transfere
			fillpoly(varibles[2],poly);
		break;
		case 8: // flood fill x y border_color fill_color style
			setfillstyle(varibles[4],varibles[3]);
			floodfill(varibles[0],varibles[1],varibles[2]);
		break;
	}
	return;
}

void setuppallete(void) // sets the pallete according to whats in the pallete array
{
	/*
	{
		for (int ii = 0; ii < 16; ii++)
			printf("%d\t%d\t%d\n",palset[ii][0],palset[ii][1],palset[ii][2]);
	} /**/
	//delay(5000);
	/*


	struct palettetype pal;
	getpalette(&pal);

	for (int ii=0; ii < 16; ii++)
	{
		//palset[ii][0],palset[ii][1],palset[ii][2]
		int red = palset[ii][0],
			green = palset[ii][1],
			blue = palset[ii][2];
		setrgbpalette(pal.colors[ii],red*4,green*4,blue*4);
	}//*/
}
//*****************************E N D   O F   I M A G E   L O A D I N G ***************************
//************************************************************************************************

int textvsize(void) // Gets the current text height
{
	return (textheight("|") + 1);
}

void center(int beginx, int endx, int y, char enteredtext[]) // for centering text graphically
{
	int x = ((endx-beginx)/2) - (textwidth(enteredtext)/2) + beginx;
	outtextxy(x,y,enteredtext);
	return;
}

int findycenter(int lines) // for vertically centering text graphically
{
	return (getmaxy()/2)-((textcoordinate*lines)/2);
}

void number_to_string(float thenumber, int digits, int format) // for converting floats to strings
{
	int i=0, ii=0;
	strset(string_transfere, 0);
	strcpy(string_transfere,"");
	char string1[10];
	strset(string1, 0);
	strcpy(string1,"");
	ltoa((long)thenumber, string1, 10);
	if (format==1) {
		if (strlen(string1)<digits) {
			for (i=0; (strlen(string1)+strlen(string_transfere)) < digits; i++)
			{
				string_transfere[i]=48;
				string_transfere[i+1]=0;
			}
		}
	} /**/
	strcat(string_transfere,string1);
	return;
}

int charecter_to_integer(char charecter) // for converting the char value to an int value
{
	char c=0;
	int result=0;
	for (c=0; c < charecter; c++, result++);
	return result;
}

void initialize_graphics(void) // initializes graphics (640x350, 16 colors, 2 pages)
{
	clrscr ();
	{
		for (register int clear_it=0;clear_it<792;clear_it++)
			palette[clear_it]=0;
	}

  //chdir("c:\\bc45\\bgi\\");
  int  Gd = DETECT, Gm;
  int  Drv=3;
  char GrErr;

  double coLors=0;
  installuserdriver("Svga",DetectVGA64k);
  coLors=255;
/*  If driver is linked with file, remove comments */
/*          registerfarbgidriver(Svga64k_fdriver);  */
  initgraph(&Gd,&Gm,"");

  /* Test if mode was initialized successfully */
  GrErr = graphresult();
  if (GrErr != grOk) {
		printf("Graphics error: %s\n",grapherrormsg(GrErr));
		exit(1);
  }
	//getch();
	setuppallete();
	return;
}

int load_data(void) // loads charecter data from file
{
	int result = 1;

	// Get Filename
	char filename[20] = {'s','a','v','e','s',92,0};
	strcat(filename,save_file);
	strcat(filename,".sav");
	//laod data

	FILE *fptr;

	if ((fptr = fopen(filename, "rb")) == NULL){
		//printf("Cannot open %s.\n", filename);
		result = 0;
	}
	else {
		fread(&Player,sizeof(PlayerData),1,fptr);
	}
	fclose(fptr);
	return result;
}

float get_equipped(idstat equip_number, int request) // looks up equiptment data
{
	float result = 0, swing_time;
	char filename[20] = {'d','a','t','a',92,0};
	struct WeaponData Weapon;
	enum {Sword, Axe, Mace, Flail, Hammer, Sheild, Helmet, Guantlete, ChestArmor,
		Leggings}; // for type varible

	// Get Filename
	number_to_string((float) equip_number, 5, 1);
	strcat(string_transfere,".dat");
	strcat(filename,string_transfere);
	strcpy(string_transfere,"");
	// L O A D   E q u i p t m e n t   D a t a
	FILE *fptr;
	if ((fptr = fopen(filename, "rb")) == NULL){ // O p e n
		printf("Cannot load weapon data.\n%s",filename);
		exit(1);
	}
	fread(&Weapon,sizeof(struct WeaponData),1,fptr);
	fclose(fptr); // C l o s e
	//              D  E  L  I  V  E  R      R  E  Q  U  E  S  T
	// get swing time
	if (Weapon.type!=Sword)
		swing_time = ((Weapon.weight *(float)Weapon.length)/(Player.strength/conversion_factor))/5; // s 10 5lbs 12 inch 1 sec
	else if (Weapon.type==Sheild)
		swing_time = (Weapon.weight/(Player.strength/conversion_factor))/5;
	else
		swing_time = ((Weapon.weight *(float)Weapon.length)/(Player.strength/conversion_factor))/12;
	// now for the request
	switch (request)
	{
		case 0: // name
			strcpy(string_transfere,Weapon.name); break;
		case 1: // description
			strcpy(string_transfere,Weapon.description); break;
		case 2: // cost (.5x for resale 1-2x for sale)
			result = Weapon.cost; break;
		case 3: // weight
			result = Weapon.weight; break;
		case 4: // lenght
			result = (float)Weapon.length; break;
		case 5: // handle
			result = (float)Weapon.handle; break;
		case 6: // durability
			result = Weapon.durability; break;
		case 7: // penatration
			result = Weapon.penatration; break;
		case 8: // type
			result = (float)Weapon.type; break;
		case 9: // hands
			result = (float)Weapon.hands; break;
			// FROM HERE IT IS MORE THAN MERE ROUTINE RETREVAL
		case 10: // swing time
			result = swing_time; break;
		case 11: // impact damage
			// max speed formula ((255/2.9)*(speed/16777216)+3)
			if (swing_time < ((255/-2.9)*(Player.speed/conversion_factor)+3))
				result = (((1/((255/2.9)*(Player.speed/conversion_factor)+3))*Weapon.weight)/10)*Weapon.penatration;
			else
				result = (((1/swing_time)*Weapon.weight)*10)*Weapon.penatration;
			break;
		default:
			result = 0;
			break;
	}
	return result;
}
		//string_transfere[64]
// 80 Dragon strength, 150 High Level Dragon strength, 110 - 180 Golem strength
// 120 - 200 Special Dragon, 300 Diamond Golem, 270 Titan, 220 Earth Elemental
// 5lbs per strength point

// Varibles saved in save file in their order
// 32 bits/varible for varibles 1 - 7, 8 bits/varible for varibles 8 - 14, 800 bit/array for arrays at 15-18
// life, constitution, spirit, agility, speed, strength, mind, hitpoints, poison, mapnumber
// mapx, mapy, story, mission, itemname[], equiptmentname[], itemquantity[], equiptmenthealth[]
// 3480 bits or 435 Bytes



void pause(int mode) // pause PAUSE;
{
	nosound();
	time_t beginT=time(NULL), endT=time(NULL);
	/*while ((time(NULL)-beginT) < (time_t).01)
		if (bioskey(1) !=0)
				char inkey = bioskey(0);*/
	//char a = getch();
	//while (getch()==a);

	if (mode==1)
	{
		//unsigned char a = getch();
		while ( ((time_t)endT-beginT) < (float).25)
		{
			beginT=time(NULL);
			while (bioskey(1) == 0)
			{
				delay(10);
				playmusic(.01+.001); // .01 can be set to anything
			}
			if (bioskey(1) !=0)
				char inkey = bioskey(0);
			endT=time(NULL);
		}
		nosound();
	}
	else if (mode==-1)
	{
		while (bioskey(1) == 0)
		{
			delay(10);
			playmusic(.01+.001); // .01 can be set to anything
		}
		nosound();
	}
	else if (mode==0)
		while (bioskey(1) == 0);
	return;
}

int save_player_data(void)
{
	int result = 1;
	// Get Filename
	char filename[20] = {'s','a','v','e','s',92,0};
	strcat(filename,save_file);
	strcat(filename,".sav");
	//save data
	FILE *fptr;
	if ((fptr = fopen(filename, "wb")) != NULL)
	{
		printf("Creating player save.\n'%s'\n...\n",filename);
		fwrite(&Player,sizeof(PlayerData),1,fptr);
		printf("Done!\n");
	}
		else
		result = 0;
	fclose(fptr);
	return result;
}

void soundfx(int sfxnumber, int modify) // plays a sound effect
{
	switch (sfxnumber)
	{
		case 0: // miss (modify speed 200-650)
		{
			float limit = 270 -.15*(float)modify;
			for (int times=0; times < (int)limit; times++)
			{
				sound( modify+200*(sin((double)times*PI/180)) );
				delay(1);
			}
			break;
		}
		case 1: // select (modify quickness, 300 select, 180 pick) //
		{
			float limit = 180 -.3*(float)modify;
			for (int times=0; times < (int)limit; times++)
			{
				sound( modify+300*(tan((double)times*PI/180)) );
				delay(1);
			}
			break;
		}
		case 2: // hit (modify strength 20-30)
		{
			float limit = .215*(float)modify, frequency, start = (float)modify/100;
			for (float times=start; times < limit; times+=(float)modify/600)
			{
				if (times<modify)
					frequency += times*times;
				else
					frequency -= times*times*times;
				sound(frequency);
				delay(1);
			}
			break;
		}
	}
	nosound();
}

void copy_page(int page) // copy from (page) to the other page
{
	int maxx = getmaxx(), maxy = getmaxy();
	void far *buf;
	int page1 = page;
	int page2 = !page;
	setactivepage(page1);
// save screen
	unsigned size;
	int ystart=0, yend, yincr, block;
	yincr = 1;//(maxy+1) / 120;
	yend = yincr;
	/* get byte size of image */
	size = imagesize_8b(0, ystart, maxx, yend);
		if ((buf = farmalloc(size)) == NULL) {
			closegraph();
			printf("Error: not enough heap space in save_screen2().\n%d",size);
			exit(1);
		}
	for (block=0; block<600; block++) {
	  //;
	  setactivepage(page1);
	  getimage(0, ystart, maxx, yend, buf);
	  setactivepage(page2);
	  putimage(0,ystart,buf,TRANS_PUT);
	  ystart = yend + 1;
	  yend += yincr + 1;
	}
	farfree(buf);// free the allocated memory
	setactivepage(page);
}

void switch_page(int page) // switches from page to page
{
	setactivepage(page);
	setvisualpage(!page);
}

void music_check (int music, time_t start) // updates music playing
{
	if (music)
	{
		double difference = (time(NULL) - start)+.0001;
		playmusic(difference);
	}
}

int pop_up_window(char *message[], char *options[], int lines, int choices, int mode, int music)
{
	sound(300);
	delay(50);
	nosound();
	int result = 0,
	oldcolor = Find_White_Pixel(),
	// define useful varibles xymin to xymax and width
	x1 = ((menumargin-1)/2)-(textwidth(message[0])/2),
	y1 = findycenter(lines) - 10,
	x2 = (((menumargin-1)/2) - (textwidth(message[0])/2)+textwidth(message[0])),
	y2 = (findycenter(lines)+textcoordinate*lines)-10,
	width = x2-x1;

	void far *graphbuffer; // graphics buffer

	unsigned size = imagesize_8b(x1-2, y1-2, x2+2, y2+2)+(x1-2)-(x2+2)+200;
	if ((graphbuffer = farmalloc(size)) == NULL) {
		closegraph();
		printf("Error: not enough heap space in save_screen().\n");
		exit(1);
	}
	clear_buffer(graphbuffer,size);

	//getimage(x1-2, y1-2, x2+2, y2+2, graphbuffer); // store background
	{
		int left=x1-2,top=y1-2,right=x2+2,bottom=y2+2;
		getimage(left,top,right,bottom,graphbuffer);
	}
	//getch();

	setcolor(8);
	// window
	{
	if (mode < 4)
		{
			int poly[10]={x1-2, y1-2, x2+2, y1-2, x2+2, y2+2, x1-2, y2+2, x1-2, y1-2};
			setfillstyle(1, 7);
			if (choices==-1)
			{
				setfillstyle(1, 8);
				setcolor(7);
			}
			fillpoly(5, poly);
		}
	}
	// window
	setcolor(0);

	switch (mode)
	{
		case 0: // regular text display
		{
			for (int i=0; i < lines; i++)
				center(0, menumargin-1,y1+textcoordinate*i, message[i]);
			pause(music);
			break;
		}
		case 1: // side by side options
		{
			float endx = (float)width/(float)choices;

			for (int iy=0; iy < (lines-1); iy++) // show text
				center(0, menumargin-1,y1+textcoordinate*iy, message[iy]);
			for (int cnum=0; cnum < choices; cnum++) // show choices
				center(endx*(float)cnum+x1,endx*(float)(cnum+1)+x1, y1+textcoordinate*(lines-1), options[cnum]);

			// selection and return of result
			int over = 0;
			time_t start, end = time(NULL);
			while (!over)
			{
			start = time(NULL);
				if (_bios_keybrd(_KEYBRD_READY) != 0)
				{
					unsigned inkey = _bios_keybrd(_NKEYBRD_READ);
					if ((unsigned char)inkey==13)
					{
						over=1;
						soundfx(1,100);
						soundfx(1,20);
					}
					switch (inkey)
					{
						case Up:
						result = 0;
						soundfx(1,300);
						break;
						case Down:
						result = choices-1;
						soundfx(1,300);
						break;
						case Left:
						if (result > 0)
							result--;
						soundfx(1,300);
						break;
						case Right:
						if (result < (choices-1))
							result++;
						soundfx(1,300);
						break;
					}
					// reshow choices
					setcolor(0);
					{
						float endx2 = (float)width/(float)choices;

						for (int iy=0; iy < (lines-1); iy++) // show text
							center(0, menumargin-1,y1+textcoordinate*iy, message[iy]);
						for (int cnum=0; cnum < choices; cnum++) // show choices
							center(endx2*(float)cnum+x1,endx2*(float)(cnum+1)+x1, y1+textcoordinate*(lines-1), options[cnum]);

						setcolor(14);
						center(endx2*(float)result+x1,endx2*(float)(result+1)+x1, y1+textcoordinate*(lines-1), options[result]);
					}
					// highlight selection

				};
				end = time(NULL);
				if (((.01-(difftime(end,start)))*1000)>0)
					delay( ( .01-( difftime(end,start) ) ) *1000 );
				if (music)
					playmusic(.01+.001);
			} // end of while loop
			while (_bios_keybrd(_KEYBRD_READY) != 0);
			break;
		}
		case 2: // verticle options
		{
			float endx = (float)width/(float)choices;

			for (int iy=0; iy < (lines-choices); iy++) // show text
				center(0, menumargin-1,y1+textcoordinate*iy, message[iy]);
			for (int cnum=0; cnum < choices; cnum++) // show choices
				center(0, menumargin-1,y1+textcoordinate*(iy+cnum), options[cnum]);

			// selection and return of result
			int over = 0;
			time_t start, end = time(NULL);
			while (!over)
			{
			start = time(NULL);
				if (_bios_keybrd(_KEYBRD_READY) != 0)
				{
					unsigned inkey = _bios_keybrd(_NKEYBRD_READ);
					if ((unsigned char)inkey==13)
					{
						over=1;
						soundfx(1,100);
						soundfx(1,20);
					}
					switch (inkey)
					{
						case Up:
						if (result > 0)
							result--;
						soundfx(1,300);
						break;
						case Down:
						if (result < (choices-1))
							result++;
						soundfx(1,300);
						break;
						case Left:
						result = 0;
						soundfx(1,300);
						break;
						case Right:
						result = choices-1;
						soundfx(1,300);
						break;
					}
					// reshow choices
					setcolor(0);
					{
						float endx2 = (float)width/(float)choices;

						for (int iy=0; iy < (lines-choices); iy++) // show text
							center(0, menumargin-1,y1+textcoordinate*iy, message[iy]);
						for (int cnum=0; cnum < choices; cnum++) // show choices
							center(0, menumargin-1,y1+textcoordinate*(iy+cnum), options[cnum]);

						setcolor(14);
						//center(endx2*(float)result+x1,endx2*(float)(result+1)+x1, y1+textcoordinate*(lines-1), options[result]);
						center(0, menumargin-1,y1+textcoordinate*(result)+textcoordinate*(lines-choices), options[result]);
					}
					// highlight selection

				};
				end = time(NULL);
				if (((.01-(difftime(end,start)))*1000)>0)
					delay( ( .01-( difftime(end,start) ) ) *1000 );
				if (music)
					playmusic(.01+.001);
			} // end of while loop
			while (_bios_keybrd(_KEYBRD_READY) != 0);
			break;
		}
		case 3: // Prompt
		{
			float endx = (float)width/(float)choices;

			for (int iy=0; iy < (lines-1); iy++) // show text
				center(0, menumargin-1,y1+textcoordinate*iy, message[iy]);
			center(0, menumargin-1,y1+textcoordinate*(lines-1), options[0]); // show default
			strcpy(string_transfere,"");
			for (int cn=0; cn <=choices; cn++)
				string_transfere[cn]=0; // clear string_transfere
			strcpy(string_transfere,options[0]); // update it
			// Read User input
			int over = 0;
			time_t start, end = time(NULL);
			while (!over)
			{
			start = time(NULL);
				if (_bios_keybrd(_KEYBRD_READY) != 0)
				{
					if (result>=choices)
						result=choices-1;
					unsigned inkey = _bios_keybrd(_NKEYBRD_READ);
					if((unsigned char)inkey==13)
					{
						over=1;
						soundfx(1,100);
						soundfx(1,20);
					}
					else if ((unsigned char)inkey==224);
					else if ((unsigned char)inkey==8) // backspace
					{
						result++;
						if (result>0)
						{
							result--;
							char append[64];
							for (int cn2=0; cn2 < 64; cn2++)
								append[cn2]=0;
							char buff[64]="";
							for (cn2=0; cn2 < 64; cn2++)
								buff[cn2]=0;
							strrev(string_transfere); // reverse string
							strncpy(append,string_transfere,strlen(string_transfere)-result); // copy after deletion point
							strrev(string_transfere); // bring them back to foward
							strrev(append); // ditto...

							if (result>1)
							{
								strncpy(buff,string_transfere,result-1); // copy before deletion point
								strcpy(string_transfere,buff); // copy beginning
								strcat(string_transfere,append); // add end
							}
							else
								strcpy(string_transfere,append); // copy remains
						}
						else
						{
							char buff[64];
							strcpy(buff,string_transfere);
							strrev(buff);
							strncpy(string_transfere,buff,strlen(buff)-1);
							strrev(string_transfere);
						};
						result--;
					}
					else if ((unsigned char)inkey==27)
					{
						for (int cn=0; cn < 64; cn++)
							string_transfere[cn]=0; // clear string_transfere
						result=0;
					}

					else if ((unsigned char)inkey<127 && (unsigned char)inkey>31)
					{
						string_transfere[result]=(unsigned char)inkey;
						result++;
					}

					switch (inkey)
					{
						case Up:
						result = 0;
						soundfx(1,300);
						break;
						case Down:
						result = strlen(string_transfere);
						soundfx(1,300);
						break;
						case Left:
						if (result > 0)
							result--;
						soundfx(1,300);
						break;
						case Right:
						if (result < (choices-1))
							result++;
						soundfx(1,300);
						break;
						case Delete:
						result++;
						if (strlen(string_transfere)<2)
						{
							for (int cn=0; cn < 64; cn++)
								string_transfere[cn]=0; // clear string_transfere
							result=0;
						}
						else if (result<(strlen(string_transfere)+1)) // delete // if not at end
						{
							char append[64];
							for (int cn2=0; cn2 < 64; cn2++)
								append[cn2]=0;
							char buff[64]="";
							for (cn2=0; cn2 < 64; cn2++)
								buff[cn2]=0;
							strrev(string_transfere); // reverse string
							strncpy(append,string_transfere,strlen(string_transfere)-result); // copy after deletion point
							strrev(string_transfere); // bring them back to foward
							strrev(append); // ditto...

							if (result>1)
							{
								strncpy(buff,string_transfere,result-1); // copy before deletion point
								strcpy(string_transfere,buff); // copy beginning
								strcat(string_transfere,append); // add end
							}
							else
								strcpy(string_transfere,append); // copy remains
						}
						else
						{
							char buff[64];
							strcpy(buff,string_transfere);
							strncpy(string_transfere,buff,strlen(buff)-1);
						};
						result--;
						break;
					}
					// check up on result status
					if (result<0)
						result=0;
					else if (result>=choices)
						result=choices-1;
					else if (result>strlen(string_transfere))
						result=strlen(string_transfere);
					// reshow choices
					setcolor(7);
					// window  clear text
					{
						int poly[10]={x1, y1+textcoordinate*(lines-1), x2, y1+textcoordinate*(lines-1), x2, y2, x1, y2, x1, y1+textcoordinate*(lines-1)};
						setfillstyle(1, 7);
						fillpoly(5, poly);
					}
					// window
					setcolor(0);
					{
						float endx2 = (float)width/(float)choices;

						for (int iy=0; iy < (lines-1); iy++) // show text
							center(0, menumargin-1,y1+textcoordinate*iy, message[iy]);
						center(0, menumargin-1,y1+textcoordinate*(lines-1), string_transfere); // show text
						{ // underline
							char underline[64];
							for (int cn2=0; cn2 < 64; cn2++)
								underline[cn2]=0;
							//strnset(underline,32,strlen(string_transfere)),
							for (cn2=0; cn2 < strlen(string_transfere); cn2++)
								underline[cn2]=32;
							underline[result] = '_';
							setcolor(14);
							center(0, menumargin-1,y1+textcoordinate*(lines-1)+1, underline); // underline
						}
					}

				};
				end = time(NULL);
				if (((.01-(difftime(end,start)))*1000)>0)
					delay( ( .01-( difftime(end,start) ) ) *1000 );
				if (music)
					playmusic(.01+.001);
			} // end of while loop
			while (_bios_keybrd(_KEYBRD_READY) != 0);
			break;
		}
		case 4: // SCROLLING TEXT DISPLAY
		{
			setcolor(oldcolor);
			int scroll = 0;
			time_t start, end = time(NULL), st;
			int page = 1;
			copy_page(1); // make copy
			setvisualpage(0); // display cpoy
			while ((scroll + lines - 1)<=choices) // while not at total number of lines
			{
				for (int climb = 0; climb < textcoordinate; climb++) // y raise
				{
					start = time(NULL);// write it
					st = time(NULL);
					for (int i=0; i < (lines-1); i++)
					{
						center(0, menumargin-1,y1+textcoordinate*(i+1)-climb, message[i+scroll]); // show visible
						music_check (music,st);
						st = time(NULL);
					}
					//setvisualpage(1); // show whats written
					//copy_page(1); // copy it
					music_check (music,st);
					st = time(NULL);
					//setvisualpage(0); // display copy...
					switch_page(page); // display change
					page = !page;
					putimage(x1-2, y1-2, graphbuffer, TRANS_PUT); // as orignal is cleared to background image

					end = time(NULL);
					if ((((float).2-(difftime(end,start)))*1000)>0)
						delay( ( (float).2-( difftime(end,start) ) ) *1000 );

					music_check (music,st);

					if (bioskey(1))
					{
						char inkey = bioskey(0); // absorb key
						if (inkey == 27)
							scroll = choices - lines + 1;
						break;
					}

				}
				scroll++; // scroll to next line
			}
			break;
		}
	} // end of case
	nosound();
	setactivepage(1);
	putimage(x1-2, y1-2, graphbuffer, TRANS_PUT); // put image
	setvisualpage(1);
	farfree(graphbuffer); // use to fre ram after usage

	return result;
}
/////////////////// MENU STUFF

void help_display(char message[]) // displays message in help box
{
	setcolor(0);
	{ // clear area
		int poly[10]={0, 590,
						  799, 590,
						  799, 599,
						  0, 599,
						  0, 590};
		setfillstyle(1, 0);
		fillpoly(5, poly);
	}
	setcolor(15);
	outtextxy(0,591,message);
}

int menu_display(int viewing, int music) // displays selected menu
{
	int result = 0;
	// window
	for (int i = 0; i < 64; i ++)
		string_transfere[i]=0;
	setcolor(0);
	{
		int poly[10]={menumargin+1, 10,
									  799, 10,
									  799, 588,
						  menumargin+1, 588,
						  menumargin+1, 10};
		setfillstyle(1, 0);
		fillpoly(5, poly);
	}
	{
		int poly[10]={menumargin+1, 0,
									  799, 0,
									  799, 8,
						  menumargin+1, 8,
						  menumargin+1, 0};
		setfillstyle(1, 0);
		fillpoly(5, poly);
	}
	// window
	char display[14] = "";
	switch(viewing)
	{
		case 0: // draw boarders
			cleardevice();
			setcolor(15);
			line(0,589,799,559); // help border
			line(menumargin,0,menumargin,559); // menu border
			line(menumargin,9,799,9); // menu line
			// help space (0,339,639,339) 80 charecters
		break;
		case 1:
			help_display("See the player stats and profile.");
			center(menumargin+1, 799, 0, "S T A T S");

			{ // sequence to load an image
			char filename[20] = "images\\";
			strcat(filename,"unarmed.drw");
			load_image(filename,menumargin+1,textcoordinate+7);
			}

			// Draw what is equipped

			if (Player.equipped[0])
			{
				{
				int y_add = 85 - ((float)5/3*get_equipped(Player.equipped[0],4)); // hand altitude - pixel length
				if (y_add<0)
					y_add==0;
				char filename[20] = "data\\images\\";
				// Get Filename show right hand
				number_to_string((float) Player.equipped[0], 5, 1);
				strcat(string_transfere,".drw");
				strcat(filename,string_transfere);
				load_image(filename,menumargin+6,textcoordinate+7+y_add);
				}
			}
			if (Player.equipped[1])
			{
				if(get_equipped(Player.equipped[1], 8)>=5) //Sheild
				{
				int y_add = 85 - ((float)5/3*get_equipped(Player.equipped[1],4));
				if (y_add<0)
					y_add==0;
				char filename[20] = "data\\images\\";
				// Get Filename show left hand
				number_to_string((float) Player.equipped[1], 5, 1);
				strcat(string_transfere,".drw");
				strcat(filename,string_transfere);
				load_image(filename,menumargin+71,textcoordinate+7+y_add);
				}
			}

			setcolor(15);
			number_to_string(Player.spirit/conversion_factor, 3, 1);
			strcpy(display,"  Spirit: ");
			strcat(display, string_transfere);
			center(menumargin+1, 799, 1*textcoordinate+142, display);

			number_to_string(Player.constitution/conversion_factor, 3, 1);
			strcpy(display,"Vitality: ");
			strcat(display, string_transfere);
			center(menumargin+1, 799, 2*textcoordinate+142, display);

			number_to_string(Player.agility/conversion_factor, 3, 1);
			strcpy(display," Agility: ");
			strcat(display, string_transfere);
			center(menumargin+1, 799, 3*textcoordinate+142, display);

			number_to_string(Player.speed/conversion_factor, 3, 1);
			strcpy(display,"   Speed: ");
			strcat(display, string_transfere);
			center(menumargin+1, 799, 4*textcoordinate+142, display);

			number_to_string(Player.strength/conversion_factor, 3, 1);
			strcpy(display,"Strength: ");
			strcat(display, string_transfere);
			center(menumargin+1, 799, 5*textcoordinate+142, display);

			number_to_string(Player.mind/conversion_factor, 3, 1);
			strcpy(display,"    Mind: ");
			strcat(display, string_transfere);
			center(menumargin+1, 799, 6*textcoordinate+142, display);

			center(menumargin+1, 799, 8*textcoordinate+142, "Gold");
			number_to_string(Player.gold, 10, 0);
			strcat(string_transfere," GP");
			center(menumargin+1, 799, 9*textcoordinate+142, string_transfere);

			number_to_string((float)charecter_to_integer(Player.hitpoints), 3, 1);
			strcpy(display,"Health: ");
			strcat(display, string_transfere);
			strcat(display, "%");
			center(menumargin+1, 799, 11*textcoordinate+142, display);
		break;
		case 2:
		{
			help_display("Set and view equiptment for battle.");
			center(menumargin+1, 799, 0, "E Q U I P");
			//center(menumargin+1, 639, 1*textcoordinate+2, "This is where");
			center(menumargin+1,799, 1*textcoordinate+2, "Set Up");
			//char part[]="               ";
			for (int e = 0; e < 6; e++)
			{
				setcolor(8);
				switch (e)
				{
					case 0:
					//strcpy(part,"Right Hand");
					center(menumargin+1,799, (3+e*3)*textcoordinate+2, "Right Hand");
					break;
					case 1:
					//strcpy(part,"Left Hand");
					center(menumargin+1,799, (3+e*3)*textcoordinate+2, "Left Hand");
					break;
					case 2:
					//strcpy(part,"Chest");
					center(menumargin+1,799, (3+e*3)*textcoordinate+2, "Chest");
					break;
					case 3:
					//strcpy(part,"Arms");
					center(menumargin+1,799, (3+e*3)*textcoordinate+2, "Arms");
					break;
					case 4:
					//strcpy(part,"Legs");
					center(menumargin+1,799, (3+e*3)*textcoordinate+2, "Legs");
					break;
					case 5:
					//strcpy(part,"Head");
					center(menumargin+1,799, (3+e*3)*textcoordinate+2, "Head");
					break;
				}
				//setcolor(8);
				//center(menumargin+1,799, (3+e*3)*textcoordinate+2, part);
				setcolor(15);
				if (Player.equipped[e])
					if (get_equipped(Player.equipped[e], 0)==0)
					{
						string_transfere[15]=0;
						center(menumargin+1, 799, (4+e*3)*textcoordinate+2, string_transfere); // print name of weapon
					}
				if (!Player.equipped[e])
					center(menumargin+1, 799, (4+e*3)*textcoordinate+2, "(nothing)");
			}
			center(menumargin+1, 799, 22*textcoordinate+2, "Press any");
			center(menumargin+1, 799, 23*textcoordinate+2, "key");
			center(menumargin+1, 799, 24*textcoordinate+2, "to change");
			center(menumargin+1, 799, 25*textcoordinate+2, "set up");
			pause(music);
			{
				setcolor(0);
				{
					int poly[10]={menumargin+1, 10,
												  799, 10,
												  799, 558,
									  menumargin+1, 558,
									  menumargin+1, 10};
					setfillstyle(1, 0);
					fillpoly(5, poly);
				}
				setcolor(15);
				result = equip_them(music,1);
			}
		}
		break;
		case 3:
		{
			for (int e = 0; e < 6; e++)
			{
				setcolor(8);
				switch (e)
				{
					case 0:
					center(menumargin+1,799, (5+e*3)*textcoordinate+2, "Pocket 1");
					break;
					case 1:
					center(menumargin+1,799, (5+e*3)*textcoordinate+2, "Pocket 2");
					break;
					case 2:
					center(menumargin+1,799, (5+e*3)*textcoordinate+2, "Pocket 3");
					break;
					case 3:
					center(menumargin+1,799, (5+e*3)*textcoordinate+2, "Pocket 4");
					break;
					case 4:
					center(menumargin+1,799, (5+e*3)*textcoordinate+2, "Pocket 5");
					break;
					case 5:
					center(menumargin+1,799, (5+e*3)*textcoordinate+2, "Pocket 6");
					break;
				}
				setcolor(15);
				if (Player.equipped[e+6])
				{
					char name[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
					char description[81];
					get_item(Player.equipped[e+6],name,description);
					center(menumargin+1, 799, (6+e*3)*textcoordinate+2, name); // print name of item
				}
				if (!Player.equipped[e+6])
					center(menumargin+1, 799, (6+e*3)*textcoordinate+2, "(nothing)");
			}
			result=item_menu(music);
		}
		break;
		case 4:
		{
			help_display("Save, load, and exit the game from here.");
			center(menumargin+1, 799, 0, "S Y S T E M");
			center(menumargin+1, 799, 1*textcoordinate+2, "Save");
			center(menumargin+1, 799, 2*textcoordinate+2, "Load");
			center(menumargin+1, 799, 3*textcoordinate+2, "Exit");
			int finished = 0, selecting=0;
			time_t start, end;
			while (!finished) {
				start = time(NULL);
				if (_bios_keybrd(_KEYBRD_READY) != 0) {
					unsigned inkey = _bios_keybrd(_NKEYBRD_READ);
					if (inkey) // reshow choices
					{
						setcolor(15);
						center(menumargin+1, 799, 1*textcoordinate+2, "Save");
						center(menumargin+1, 799, 2*textcoordinate+2, "Load");
						center(menumargin+1, 799, 3*textcoordinate+2, "Exit");
					}
					switch (inkey)
					{
						case Up:
						soundfx(1,300);
						selecting--;
						break;
						case Down:
						soundfx(1,300);
						selecting++;
						break;
						case Left:
						result = 1;
						finished=1;
						break;
						case Right:
						result = 2;
						finished=1;
						break;
					}
					if (selecting > 2)
						selecting = 0;
					if (selecting < 0)
						selecting = 2;
					//highlight selection
					setcolor(14);
					switch (selecting)
					{
					case 0:
					center(menumargin+1, 799, 1*textcoordinate+2, "Save");
					setcolor(15);
					help_display("Save your data over your last save.");
					break;
					case 1:
					center(menumargin+1, 799, 2*textcoordinate+2, "Load");
					setcolor(15);
					help_display("Load new data.");
					break;
					case 2:
					center(menumargin+1, 799, 3*textcoordinate+2, "Exit");
					setcolor(15);
					help_display("Exit the game!?");
					break;
					}
					setcolor(15);

					switch ((unsigned char)inkey)
					{
						case 13: // enter
							switch (selecting)
							{
								case 0: // Save
								{
									if (town==0) // if not in town
									{
										char *message[1] = {"Overwrite your last save?"};
										char *options[2] = {"Yes","No"};
										if (!pop_up_window(message, options, 2, 2, 1, music))
											if(save_player_data())
												{
													char *message[1] = {"Your file has been saved"};
													char *options[1] = {""};
													pop_up_window(message, options, 1, 1, 0, music);
												}
									}
									else
									{
										char *message1[1] = {"You cannot save while inside a town!"};
										char *options1[1] = {""};
										pop_up_window(message1, options1, 1, 1, 0, music);
									}
									setcolor(15);
								}
								break;
								case 1: // load
									load_new();
								break;
								case 2: // Exit
								{
									char *message[1] = {"Are you sure you want to exit the game?"};
									char *options[2] = {"Yes","No"};
									if (!pop_up_window(message, options, 2, 2, 1, music))
										end_game();
								}
								break;
							}
						break; // go to menu
						case 9: // tab
						result = 3;
						finished=1;
						break;
					}
				};
				end = time(NULL);
				if (((.01-(difftime(end,start)))*1000)>0)
					delay( ( .01-( difftime(end,start) ) ) *1000 );
				if (music)
					playmusic(.01+.001);
			};
			setcolor(15);
		}
		break;
	}
	return result;
}

void menu_selection(int music) // allows the user to select the desired menu
{
	setcolor(14);
	line(0,559,799,559); // help border
	line(menumargin,0,menumargin,559); // menu border
	line(menumargin,9,799,9); // menu line
	soundfx(1,20);
	int over = 0, last_menu=menu_pick, menu=menu_pick,menu_pass=0;
	time_t start, end;
	while (!over)
	{
		start = time(NULL);
		if (menu_pass==1)
			menu--;
		if (menu_pass==2)
			menu++;
		if (_bios_keybrd(_KEYBRD_READY) != 0)
		{
			unsigned inkey = _bios_keybrd(_NKEYBRD_READ);
			if ((unsigned char)inkey==9)
			{
				over=1;
				soundfx(1,20);
			}
			last_menu=menu;
			switch (inkey)
			{
			case Right:
			menu ++;
			break;
			case Left:
			menu --;
			break;
			} // 0 - 3
			if (menu < 0) // set limits
				menu = 3;
			if (menu > 3) // upper limit
				menu = 0;
			menu_pass=0;
		} // end of if key
		if (last_menu!=menu) // if changed menu display changed menu
		{
			if (menu < 0) // set limits
				menu = 3;
			if (menu > 3) // upper limit
				menu = 0;
			last_menu=menu;
			menu_pick=menu;
			menu_pass = menu_display(menu_pick+1, music);
			if (menu_pass == 3) // exit if told to
			{
				over=1;
				soundfx(1,20);
			}
		}
		end = time(NULL);
		if (((.01-(difftime(end,start)))*1000)>0)
			delay( ( .01-( difftime(end,start) ) ) *1000 );
		if (music)
			playmusic(.01+.001);
	} // end of while loop

	setcolor(15);
	line(0,559,799,559); // help border
	line(menumargin,0,menumargin,559); // menu border
	line(menumargin,9,799,9); // menu line
	return;
}

int probability (float percentage) // determins via percentage wheather or not something happens
{
	randomize();
	return (random(1000) <= (int)(percentage * 10));
}

void fadeout(void) // fades to black
{
	/*struct palettetype pal;
	getpalette(&pal);

	int red, green, blue, eval = 0;
	for (int f=0; f < 64; f++)
	{
		for (int ii=0; ii < 16; ii++)
		{
			red = 0; green = 0; blue = 0;
			eval = palset[ii][0];
			if ((eval-f)>0)
				red = eval - f;
				eval = palset[ii][1];
			if ((eval-f)>0)
				green = eval - f;
				eval = palset[ii][2];
			if ((eval-f)>0)
				blue = eval - f;

			setrgbpalette(pal.colors[ii],red,green,blue);

		}
		if (bioskey(1))
			break;
		delay(1);
	} */
	return;
}

void isolate_string(int start_byte, int end_byte, const char *string, char *result)
{
	for (int i = start_byte, d=0; i <= end_byte; i++, d++)
		*(result+d)=*(string+i);
	*(result+(d+1))=0; // append null
}

int equip_them(int music, int told_to)
{
	char desc_list[6300];
	int list = 0, selected = 0, escaped = 0; // list marks top of list
	//for (int max = 0; max <=99 && equiptmentname[max] != 0; max++); // find how many
	int max = find(1,0)-1;//find how many
	while (!escaped)
	{
		if (selected==1) // next?
			list += 20;
		if (selected>1 && !told_to) // picked something but not told to change equiptment?
		{
			escaped = selected; // return selected equiptment
			break; // break out of here
		}
		if (selected>1 && told_to) // picked something?
		{ //           E           Q        U         I        P        P   R  O C E S S
			enum {Sword, Axe, Mace, Flail, Hammer, Sheild, Helmet, Guantlete, ChestArmor,
				Leggings}; // for type varible

			switch ((int)get_equipped(Player.equiptmentname[selected-2], 8))
			{
				case Sword: case Axe: case Mace: case Flail: case Hammer:
				{
					int hands = get_equipped(Player.equiptmentname[selected-2], 9);
					if (hands == 1) // hand and a half
					{
						char *message[1] = {"Do want to hold the weapon with both hands?"};
						char *options[2] = {"Yes","No"};
						hands = 0;
						if (!pop_up_window(message, options, 2, 2, 1, 1))
							hands = 2; // if yes treat as a two hander
					}
						if (hands == 2) // two handed
					{
						Player.equipped[0]=Player.equiptmentname[selected-2];
						Player.equipped[1]=Player.equiptmentname[selected-2];
					}
					if (hands == 0) // one handed
						Player.equipped[0]=Player.equiptmentname[selected-2];
				}
				break;
				case Sheild:
					if (Player.equipped[1]!=0)
					{
						if(get_equipped(Player.equipped[1], 8)<Sheild)
						{ // if weapon is there, de-equip it
							Player.equipped[0]=0;
							Player.equipped[1]=0;
						}
					}
					Player.equipped[1]=Player.equiptmentname[selected-2];
				break;
				case ChestArmor:
				Player.equipped[2]=Player.equiptmentname[selected-2];
				break;
				case Guantlete:
				Player.equipped[3]=Player.equiptmentname[selected-2];
				break;
				case Leggings:
				Player.equipped[4]=Player.equiptmentname[selected-2];
				break;
				case Helmet:
				Player.equipped[5]=Player.equiptmentname[selected-2];
				break;
			}
		}
		if (selected==0) // previous
			list -= 20;

		if (list < 0)
			list = 0;
		if (list > max)
			list=max;
		/*if (list > (max-19) && (max-20) >=0) // if to far down go to 20 above bottom
			list = max-20;*/


	setcolor(0);
	{
		int poly[10]={menumargin+1, 10,
									  799, 10,
									  799, 558,
						  menumargin+1, 558,
						  menumargin+1, 10};
		setfillstyle(1, 0);
		fillpoly(5, poly);
	}
	setcolor(15);

		center(menumargin+1, 799, 1*textcoordinate+2, "Previous");
		{
			for (int i = 0; i < 6300; i++)
				desc_list[i]=0;
		}
		for (int nv = list, n = 0; nv<(list+20) && nv <= max; nv++, n++) // make lists read data
		{
			if (nv>99) // if too far
			{
				break;
			}
			if (Player.equiptmentname[nv]==0) // if at list end
			{
				break;
			}
			if (get_equipped(Player.equiptmentname[nv], 0)==0)
			{
				string_transfere[15]=0;
				center(menumargin+1, 799, (n+2)*textcoordinate+2, string_transfere); // print name of weapon
			}
			if (get_equipped(Player.equiptmentname[nv], 1)==0)
				strcat(desc_list,string_transfere); // add description to list
		}
		time_t start, end = time(NULL);
		int selecting, finished;
		finished=0;
		selecting=1;
		center(menumargin+1, 799, (n+2)*textcoordinate+2, "Next");
		setlinestyle(0,0,1);
		while (!finished) {
			start = time(NULL);
			if (_bios_keybrd(_KEYBRD_READY) != 0) {
				unsigned inkey = _bios_keybrd(_NKEYBRD_READ);
				if (inkey) // clear marker
				{
					setcolor(0);
					rectangle(menumargin+1, selecting*textcoordinate+1,
							799, (selecting+1)*textcoordinate+1);
					setcolor(15);
				}
				switch (inkey)
				{
					case Up:
					soundfx(1,300);
					selecting--;
					break;
					case Down:
					soundfx(1,300);
					selecting++;
					break;
					case Left:
					escaped = 1;
					finished=1;
					break;
					case Right:
					escaped = 2;
					finished=1;
					break;
				}
				if (selecting > (n+2))
					selecting = 1;
				if (selecting < 1)
					selecting = (n+2);

				// determine what was selected
				if (selecting>=(n+2)) // next
					selected=1;
				if (selecting==1) // previous
				selected=0;
				if (!(selecting>=(n+2)) && selecting!=1) // if pick something
					selected=(list+selecting);
				//show marker
					setcolor(14);
					rectangle(menumargin+1, selecting*textcoordinate+1,
							799, (selecting+1)*textcoordinate+1);
					setcolor(15);

				//show description
				if (selected>=2)
				{
					char info[64];
					char temp[6300];
					strcpy(temp,desc_list);
					for (register int i = 0; i < 64; i++)
						info[i]=0;
					isolate_string(63*(selected-2), (63*(1+(selected-2))-1), desc_list, info);
					help_display(info);
					strcpy(desc_list,temp);
				}
				if (selected==0) // previous
					help_display("Show previous 20");
				if (selected==1) // next
					help_display("Show next 20");

				switch ((unsigned char)inkey)
				{
					case 13:
					finished=1; // break free its over
					break; // go to menu
					case 9: // tab
					escaped = 3;
					finished=1;
					break;
				}
			};
			end = time(NULL);
			if (((.01-(difftime(end,start)))*1000)>0)
				delay( ( .01-( difftime(end,start) ) ) *1000 );
			if (music)
				playmusic(.01+.001);
		};
		setcolor(0);
		rectangle(menumargin+1, selecting*textcoordinate+1,
			799, (selecting+1)*textcoordinate+1);
		setcolor(15);
	}
	return escaped;
}

void load_new(void) // loads user selected file
{
		char file_to_load[12] = "";
		{
			char *message[6] = {"Select a file to load",
									 ""
									 };
			//-F-I-L-E---L-I-S-T---C-O-D-E-----------------------------------------------
			char file1[13]="",file2[13]="",file3[13]="",file4[13]="",file5[13]="",file6[13]="",
				file7[13]="",file8[13]="",file9[13]="",file10[13]=""; // arrays for filenames
			char *file_list[11] = {file1,file2,file3,file4,file5,file6,file7,file8,file9,
				file10, "(Cancel)"}; // pointer to file list
			int file_number=0;
			{ // code for getting a list of filenames
				struct ffblk ffblk;
				int done;
				char current_file[13];
				done = findfirst("saves\\*.sav",&ffblk,0);
				while (!done && file_number < 10)
				{
					strcpy(current_file,ffblk.ff_name);
					done = findnext(&ffblk);
					strncpy(file_list[file_number],current_file, strlen(current_file)-4);
					file_number++;
				}
			}
			//-E-N-D---O-F---F-I-L-E---L-I-S-T---C-O-D-E---------------------------------


			/*char *options[3] = {"Nothing.",
									 "Yet.",
									 "Problem?"
									 };*/

			strcpy(file_list[file_number], "(Cancel)");
			int result = pop_up_window(message, file_list, file_number+3, file_number+1, 2, 1);
			setcolor(15);
			{
				for (int i = 0; i < 9; i++)
					save_file[i]=0;
			}
			if (result != file_number) // laoding something
			{ // complete transfere of control from this program to the next
				//char save_file[9]={0,0,0,0,0,0,0,0,0};
				strcpy(save_file,file_list[result]);
				char *arg0 = save_file;
				cleardevice();
				closegraph();
				spawnlp(P_OVERLAY, "rpg", "rpg.exe",arg0, NULL);
			}
		}
}

//              I     T         E          M             S      T     U      F       F

int select_item(int music) // allows the user to select an item, returns number of item on list
{
	char desc_list[8001];
	int list = 0, selected = 0, escaped=0; // list marks top of list
	int max = find(0,0)-1;//find how many
	while (!escaped)
	{
		if (selected==1) // next?
			list += 20;
		if (selected>1) // picked something?
		{
			escaped = selected; // return selected equiptment
			setcolor(0);
			{
				int poly[10]={menumargin+1, 10,
											  799, 10,
											  799, 558,
								  menumargin+1, 558,
								  menumargin+1, 10};
				setfillstyle(1, 0);
				fillpoly(5, poly);
			}
			setcolor(15);
			return escaped-2;
			//break; // break out of here
		}
		if (selected==0) // previous
			list -= 20;

		if (list < 0)
			list = 0;
		if (list > max)
			list=max;
		/*if (list > (max-19) && (max-20) >=0) // if to far down go to 20 above bottom
			list = max-20;*/


	setcolor(0);
	{
		int poly[10]={menumargin+1, 10,
									  799, 10,
									  799, 558,
						  menumargin+1, 558,
						  menumargin+1, 10};
		setfillstyle(1, 0);
		fillpoly(5, poly);
	}
	setcolor(15);

		center(menumargin+1, 799, 1*textcoordinate+2, "Previous");
		setcolor(8);
		center(menumargin+1, 799, findycenter(2)+0*textcoordinate, "Press Esc");
		center(menumargin+1, 799, findycenter(2)+1*textcoordinate, "To Cancel");
		setcolor(15);
		{
			for (register int i = 0; i < 8001; i++)
				desc_list[i]=0;
		}
		for (int nv = list, n = 0; nv<(list+20) && nv <= max; nv++, n++) // make lists read data
		{
			if (nv>99) // if too far
			{
				break;
			}
			if (Player.itemname[nv]==0) // if at list end
			{
				break;
			}
			{
				char name[15];
				char description[81];
				//char quantity[5];
				for (register int ih = 0; ih<81; ih++)
					description[ih]=0;
				for (ih = 0; ih<15; ih++)
					name[ih]=0;
				get_item(Player.itemname[nv],name,description);
				number_to_string(Player.itemquantity[nv],2,1);
				strcat(name," ");
				strcat(name,string_transfere);
				center(menumargin+1, 799, (n+2)*textcoordinate+2, name); // print name of weapon
				strcat(desc_list,description); // add description to list
			}
		}
		time_t start, end = time(NULL);
		int selecting, finished;
		finished=0;
		selecting=1;
		center(menumargin+1, 799, (n+2)*textcoordinate+2, "Next");
		setlinestyle(0,0,1);
		while (!finished) {
			start = time(NULL);
			if (_bios_keybrd(_KEYBRD_READY) != 0) {
				unsigned inkey = _bios_keybrd(_NKEYBRD_READ);
				if (inkey) // clear marker
				{
					setcolor(0);
					rectangle(menumargin+1, selecting*textcoordinate+1,
							799, (selecting+1)*textcoordinate+1);
					setcolor(15);
				}
				switch (inkey)
				{
					case Up:
					soundfx(1,300);
					selecting--;
					break;
					case Down:
					soundfx(1,300);
					selecting++;
					break;
				}
				if (selecting > (n+2))
					selecting = 1;
				if (selecting < 1)
					selecting = (n+2);

				// determine what was selected
				if (selecting>=(n+2)) // next
					selected=1;
				if (selecting==1) // previous
				selected=0;
				if (!(selecting>=(n+2)) && selecting!=1) // if pick something
					selected=(list+selecting);
				//show marker
					setcolor(14);
					rectangle(menumargin+1, selecting*textcoordinate+1,
							799, (selecting+1)*textcoordinate+1);
					setcolor(15);

				//show description
				if (selected>=2)
				{
					char info[81];
					char temp[8001];
					strcpy(temp,desc_list);
					for (register int i = 0; i < 81; i++)
						info[i]=0;
					isolate_string(80*(selected-2), (80*(1+(selected-2))-1), desc_list, info);
					help_display(info);
					strcpy(desc_list,temp);
				}
				if (selected==0) // previous
					help_display("Show previous 20");
				if (selected==1) // next
					help_display("Show next 20");

				switch ((unsigned char)inkey)
				{
					case 13:
					finished=1; // break free its over
					break; // go to menu
					case 27: // esc
					escaped = -10;
					finished=1;
					break;
				}
			};
			end = time(NULL);
			if (((.01-(difftime(end,start)))*1000)>0)
				delay( ( .01-( difftime(end,start) ) ) *1000 );
			if (music)
				playmusic(.01+.001);
		};
		setcolor(0);
		rectangle(menumargin+1, selecting*textcoordinate+1,
			799, (selecting+1)*textcoordinate+1);
		setcolor(15);
	}
	setcolor(0);
	{
		int poly[10]={menumargin+1, 10,
									  799, 10,
									  799, 558,
						  menumargin+1, 558,
						  menumargin+1, 10};
		setfillstyle(1, 0);
		fillpoly(5, poly);
	}
	setcolor(15);
	return escaped-2;
}

int item_menu(int music) // the menu for item commands
{
	int result=0;
	help_display("Set up battle items and use items.");
	center(menumargin+1, 799, 0, "I T E M S");
	setcolor(14);
	center(menumargin+1, 799, 1*textcoordinate+2, "Use Item");
	setcolor(15);
	center(menumargin+1, 799, 2*textcoordinate+2, "Set For Battle");
	center(menumargin+1, 799, 3*textcoordinate+2, "Throw Away");
	int finished = 0, selecting=0;
	time_t start, end;
	while (!finished) {
		start = time(NULL);
		if (_bios_keybrd(_KEYBRD_READY) != 0) {
			unsigned inkey = _bios_keybrd(_NKEYBRD_READ);
			if (inkey) // reshow choices
			{
				setcolor(15);
				center(menumargin+1, 799, 1*textcoordinate+2, "Use Item");
				center(menumargin+1, 799, 2*textcoordinate+2, "Set For Battle");
				center(menumargin+1, 799, 3*textcoordinate+2, "Throw Away");
			}
			switch (inkey)
			{
				case Up:
				soundfx(1,300);
				selecting--;
				break;
				case Down:
				soundfx(1,300);
				selecting++;
				break;
				case Left:
				result = 1;
				finished=1;
				break;
				case Right:
				result = 2;
				finished=1;
				break;
			}
			if (selecting > 2)
				selecting = 0;
			if (selecting < 0)
				selecting = 2;
			//highlight selection
			setcolor(14);
			switch (selecting)
			{
			case 0:
			center(menumargin+1, 799, 1*textcoordinate+2, "Use Item");
			setcolor(15);
			help_display("Use an Item");
			break;
			case 1:
			center(menumargin+1, 799, 2*textcoordinate+2, "Set For Battle");
			setcolor(15);
			help_display("Place items on your belt so that you can use them in battle.");
			break;
			case 2:
			center(menumargin+1, 799, 3*textcoordinate+2, "Throw Away");
			setcolor(15);
			help_display("Throw away an item so that you can have more free space.");
			break;
			}
			setcolor(15);
			switch ((unsigned char)inkey)
			{
				case 13: // enter
					{
					int poly[10]={menumargin+1, 0,
												  799, 0,
												  799, 8,
									  menumargin+1, 8,
									  menumargin+1, 0};
					setfillstyle(1, 0);
					setcolor(0);
					fillpoly(5, poly);
					setcolor(15);
					}
					switch (selecting)
					{
						case 0: // Use
						{
							center(menumargin+1, 799, 0, "USE ITEM");
							setcolor(15);
							help_display("Press enter on the one you whish to use, press Esc to cancel.");
							int item = select_item(music);
							if (item>=0)
								use_item(Player.itemname[item]);
						}
						break;
						case 1: // set for battle
						{
							center(menumargin+1, 799, 0, "SET FOR BATTLE");
							setcolor(15);
							help_display("Press enter on the one you whish to set up for battle, press Esc to cancel.");
							int item = select_item(music);
							if (item>=0)
							{
								//int already_there=0;
								int amount = 0, where=0;
								for (int i=6; i<12; i++)
									if (Player.equipped[i]==Player.itemname[item])
									{
										where=i;
										amount++;
									}
								if (amount >= Player.itemquantity[find(0,item)-1]) // if you have all equipped then de-equip
									Player.equipped[where]=0;
										//equipped[i]=0; // if equipped then de equip it
								{
									char *message[3] = {"Where to do want to place it?",
															  "Pocket # (Key)",
															  ""};
									char *options[6] = {"Pocket 1 (Ins) ",
															  "Pocket 2 (Home)",
															  " Pocket 3 (Pg Up)",
															  "Pocket 4 (Del) ",
															  "Pocket 5 (End) ",
															  "   Pocket 6 (Pg Down)"};
									Player.equipped[pop_up_window(message, options, 9, 6, 2, music)+6]=Player.itemname[item];
								}
							}
						}
						break;
						case 2: // throw away item
						{
							center(menumargin+1, 799, 0, "THROW AWAY");
							help_display("Press enter on the one you whish to THROW AWAY, press Esc to cancel.");
							int item = select_item(music);
							if (item>=0)
							{
								char *message[1] = {"Are you sure you to throw that away?"};
								char *options[2] = {"Yes","No"};
								if (!pop_up_window(message, options, 2, 2, 1, music))
									take(0,item);
							}
						}
						break;
					}
				break; // go to menu
				case 9: // tab
				result = 3;
				finished=1;
				break;
			}
		};
		end = time(NULL);
		if (((.01-(difftime(end,start)))*1000)>0)
			delay( ( .01-( difftime(end,start) ) ) *1000 );
		if (music)
			playmusic(.01+.001);
	};
	setcolor(15);
	return result;
}

void use_item(idstat id) // uses non-battle items
{
	int location = find(0,id)-1;

	switch (id)
	{
		case 1: // heal
			if (Player.hitpoints<100)
			{
				Player.hitpoints+=10;
				Player.itemquantity[location]--; // reduce quantity by 1
				if (Player.itemquantity[location]<1) // if out then remove from list
					take(0,location);
			}
		break;
		case 2: // m heal
			if (Player.hitpoints<100)
			{
				Player.hitpoints+=25;
				Player.itemquantity[location]--; // reduce quantity by 1
				if (Player.itemquantity[location]<1) // if out then remove from list
					take(0,location);
			}
		break;
		case 3: // full heal
			if (Player.hitpoints<100)
			{
				Player.hitpoints=100;
				Player.itemquantity[location]--; // reduce quantity by 1
				if (Player.itemquantity[location]<1) // if out then remove from list
					take(0,location);
			}
		break;
		case 4: // cure
			if (Player.poison>0)
			{
				Player.poison/=5;
				Player.itemquantity[location]--; // reduce quantity by 1
				if (Player.itemquantity[location]<1) // if out then remove from list
					take(0,location);
			}
		break;
		case 5: // m cure
			if (Player.poison>0)
			{
				Player.poison/=10;
				Player.itemquantity[location]--; // reduce quantity by 1
				if (Player.itemquantity[location]<1) // if out then remove from list
					take(0,location);
			}
		break;
		case 6: // full cure
			if (Player.poison>0)
			{
				Player.poison=0;
				Player.itemquantity[location]--; // reduce quantity by 1
				if (Player.itemquantity[location]<1) // if out then remove from list
					take(0,location);
			}
		break;
		case 1001: // tellers message
		{
			char *message[17] = {"                                                        ",
									  "Fortune Teller's Message",
									  "",
									  "She said that she saw a man and he was writing a note",
									  "that said...",
									  "To defeat the cloud you must find the one it looks for.",
									  "From thy neck shall hang thy guide to lead you up the",
									  "mountain. With the sealing roars the sound but only",
									  "once the way is found. The skies return what is",
									  "disguised, discern; as you find the righteous cause for",
									  "that which you yearn.",
									  "She then said it looks like he's trying to write more",
									  "but she could not read it. Then she said that she sees",
									  "a mountain and several caves. She said that she is",
									  "getting a strong feeling about this and thinks it is",
									  "very important that I find out what all of this means.",
									  ""
									 };
			char *options[1] = {""};
			pop_up_window(message, options, 17, -1, 0, 0);
		}
		default:
		{
			char *message[1] = {"Cannot use that here!"};
			char *options[1] = {""};
			pop_up_window(message, options, 1, 0, 0, 1);
		}
		break;
	}
	if (Player.hitpoints>100)
		Player.hitpoints=100;
}

void get_item(idstat id, char *name, char *description) // gets on data item
{
	strcpy(description,"                                                                                "); // 63 max
	switch (id)
	{
		case 1:
			strcpy(name,"Heal Potion");
			strcpy(description,"Heals 10% of your health."); // 63 max
			break;
		case 2:
			strcpy(name,"Major Heal");
			strcpy(description,"Heals 25% of your health."); // 63 max
			break;
		case 3:
			strcpy(name,"Revival");
			strcpy(description,"Heals 10% you completely."); // 63 max
			break;
		case 4:
			strcpy(name,"Cure");
			strcpy(description,"A weak cure for poison."); // 63 max
			break;
		case 5:
			strcpy(name,"Major Cure");
			strcpy(description,"A stong cure for poison");
			break;
		case 6:
			strcpy(name,"Full Cure");
			strcpy(description,"A complete cure for all poison."); // 63 max
			break;
		case 10:
			strcpy(name,"Fairy Dust");
			strcpy(description,"Fully replenishes magic in battle"); // 63 max
			break;
		case 20:
			strcpy(name,"Block Stone");
			strcpy(description,"Cast \"Block Spell\" in battle to interrupt opponents casting.");
			break;
		case 30:
			strcpy(name,"Shock Stone");
			strcpy(description,"Cast \"Spark\" in battle resulting in a 25% power, electrical attack.");
			break;
		case 40:
			strcpy(name,"Fire Stone");
			strcpy(description,"Cast \"Fire Ball\" in battle resulting in a 45% power, fire attack.");
			break;
		case 1000:
			strcpy(name,"Ring Amulet");
			strcpy(description,"Its hollow and doesn't seem to be worth much.");
			break;
		case 1001:
			//          "123456789012"
			strcpy(name,"Teller's Msg");
			strcpy(description,"Your copy of what the fortune teller told you.");
			break;
		case 1002:
			strcpy(name,"T. Hall Pass");
			strcpy(description,"A pass to freely enter the training hall.");
			break;
	}
	if (strlen(description)<80)
		description[strlen(description)]=' ';
}

// ITEM AND EQUIPTMENT SUBSYSTEMS

void replace (int equiptment, idstat id) // prompts user to replace an item or peice of equiptment
{
	char *message[4] = {"You have too many items or pieces of",
							  "equiptment to recieve another one.",
							  "Do you want to replace something",
							  "with what you are receiving?"};
	char *options[2] = {"Yes","No"};
	if (!pop_up_window(message, options, 5, 2, 1, 1)) // if told to replace
	{
		{
		int poly[10]={menumargin+1, 0,
									  799, 0,
									  799, 8,
						  menumargin+1, 8,
						  menumargin+1, 0};
		setfillstyle(1, 0);
		fillpoly(5, poly);
		}
		center(menumargin+1, 799, 0, "R E P L A C E");
		char *message2[1] = {"Select which one you want to replace."};
		char *options2[1] = {""};
		pop_up_window(message2, options2, 1, 0, 0, 1);

		if (equiptment) // replace equiptment procedure
		{
			int location=equip_them(0,0)-2;
			Player.equiptmentname[location]=id;
			Player.equiptmenthealth[location]=100;
		}
		if (!equiptment)
		{
			int location = select_item(0);
			Player.itemname[location]=id;
			Player.itemquantity[location]=1;
		}
	}
}

int find (int equiptment, idstat id) // finds where item or equiptment is in the list
{
	int result = 0;
	for (int i=0; i<100; i++)
	{
		if (equiptment)
			if (Player.equiptmentname[i]==id)
				result=i+1;
		if (!equiptment)
			if (Player.itemname[i]==id)
				result=i+1;
		if (result)
			break;
	}
	return result;
}

void give_player (int equiptment, idstat id) // gives equipt or item to player
{
	int end = find(equiptment,0) - 1;
	int found = find(equiptment,id);
	if (end>98)// if you have 100 already then replace one
	{
		if (!equiptment && !found)
				replace(equiptment, id);
		if (equiptment)
			replace(equiptment, id);
	}
	else
	{
		if (equiptment)
		{
			Player.equiptmentname[end]=id;
			Player.equiptmenthealth[end]=100;
		}
		if (!equiptment)
		{
			if (!found)
			{
				Player.itemname[end]=id;
				Player.itemquantity[end]=1;
			}
			if (found) // if found just increase the quantity
			{
				Player.itemquantity[found-1]++;
				if (Player.itemquantity[found-1]>99) // limit set to 99
					Player.itemquantity[found-1]=99;
			}
		}
	}
}

void take (int equiptment, int location) // takes equipt or item from location
{
	for (int i=location; i<99; i++)
	{
		if (equiptment)
		{
			Player.equiptmentname[i]=Player.equiptmentname[i+1];
			Player.equiptmenthealth[i]=Player.equiptmenthealth[i+1];
		}
		else
		{
			Player.itemname[i]=Player.itemname[i+1];
			Player.itemquantity[i]=Player.itemquantity[i+1];
		}
	}
	if (equiptment)
		Player.equiptmentname[99]=0;
	else
		Player.itemname[99]=0;
}

// END OF ITEM AND EQUIPTMENT SUBSYSTEMS

void image_pop_up (int x, int y, int width, int height, int music, char image[], char message[]) // shows a pop up image at x y coodinates
{
	void far *graphbuffer; // graphics buffer

	unsigned size = imagesize_8b(x, y, x+width-1, y+height-1);
	if ((graphbuffer = farmalloc(size)) == NULL) {
		closegraph();
		printf("Error: not enough heap space in save_screen().\n");
		exit(1);
	}

	getimage(x, y, x+width-1, y+height-1, graphbuffer); // store background

	load_image(image,x,y); // draw image
	setcolor(15);
	center(x, x+width-1,y+height-1-textcoordinate,message); // shows caption
	pause(music); // pause

	putimage(x, y, graphbuffer, TRANS_PUT); // puts back original image
	farfree(graphbuffer); // use to free ram after usage
}

int main(int argc, char *argv[])
{
	strcpy(save_file,"");
	///////////CODE FOR LOADING COMMAND LINE FILE
	{
		char filename[9];
		for (int i=0; i < 9; i++)
			filename[i]=0;
		filename[9]=0;

		if (argc > 1) {
			strcpy(filename, argv[1]);
		}
		else
		{
			printf("Missing Parameters.");
			exit(1);
		}
		//load_data(filename);
		strcpy(save_file,filename);
	}
	if (!load_data())
	{
		printf("Could not load file.");
		exit(1);
	}
	/////////END OF ARGUMENT CODE main(int argc, char *argv[])
	clrscr();
	initialize_graphics();
	setactivepage(1);
	setvisualpage(1);
	menu_display(0,0);
	menu_display(1,0);
	// BEGINING OF WHILE LOOP             G A M E    S T A R T S    H E R E
	town=0;
	if (Player.story == 1 && Player.mission == 1) // if first time, then in first town
		town=1;
	while (1)
	{
		if (town) // = 0 if not in a town, so if in a town
		{
			switch (town) // = 0 if not in a town
			{
				case 1:
					town1();
					town=0; // outside of town
					Player.mapnumber=1; // on map 1
				break;
			}
		} // end of if town
		else
			Map(Player.mapnumber);
	} // end of while loop
	//menu_selection(1);
	closegraph();
	return 0;
}

float convert_string_to_integer(char string[], int length)
{
	int i;
	int place;
	float result;
	//length--;
	for(i=0; i<length; i++)
	{
		place = length-i;
		switch (string[i])
		{
			case '0': break;
			case '1': result = result + pow(10, (double)(place)); break;
			case '2': result = result + 2 * pow(10, (double)(place)); break;
			case '3': result = result + 3 * pow(10, (double)(place)); break;
			case '4': result = result + 4 * pow(10, (double)(place)); break;
			case '5': result = result + 5 * pow(10, (double)(place)); break;
			case '6': result = result + 6 * pow(10, (double)(place)); break;
			case '7': result = result + 7 * pow(10, (double)(place)); break;
			case '8': result = result + 8 * pow(10, (double)(place)); break;
			case '9': result = result + 9 * pow(10, (double)(length-i)); break;
			default: result = -1; break;
		}
		if (result == -1)
		{
			printf("Data reading error\n");
			printf("Press any key to exit.");
			getch();
			exit(1);
		}
	}
	return result/10;
}

void end_game(void)
{
	closegraph();
	exit(0);
}

