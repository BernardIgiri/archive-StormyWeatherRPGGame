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
#include "pldata.h"

#define TRANS_PUT 5
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

/*typedef float stat;
typedef char critstat;
typedef unsigned int idstat;*/
// Global varibles
extern PlayerData Player;
extern critstat town;
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
extern void fadeout(void);
extern void setuppallete(void);
extern int find (int equiptment, idstat id);
extern void give_player (int equiptment, idstat id);
extern void take (int equiptment, int location);
extern void image_pop_up (int x, int y, int width, int height, int music, char image[], char message[]);
extern int menu_display(int viewing, int music);
extern int menu_display(int viewing, int music);
extern void get_item(idstat id, char *name, char *description);
extern void number_to_string(float thenumber, int digits, int format);
extern int textvsize(void);
extern void center(int beginx, int endx, int y, char enteredtext[]);
extern unsigned long imagesize_8b(int x1, int y1, int x2, int y2);
extern void ReadPCX( char *filename, int offsetx, int offsety, int load_pal );

void get_visual (idstat item_id, int *x, int *y, int *width, int *height, char *image);

void dialog_display (char speaker[], char text[], int music); // formats and displays dialog
// L o c a t i o n s  F o r  P l a y e r  t o  b e  a t
int Tims_Home (void);
int Market (void);
void Store1 (idstat on_sale[], int prices[], char *comments[], int choices, int music);

int town_data[5];
//**************************** E N D   O F   I N I T I A L   C O D E *****************************
void user_cursor(int music, int *selected_x, int *selected_y)
{
	help_display("Move the cursor with the arrow keys, and press tab to access the menu");
	int over=0, x=*selected_x, y=*selected_y, tried_to_leave=0;
	time_t start, end = time(NULL);
	void far *graphbuffer[1]; // graphics buffer

	unsigned size = imagesize_8b(x*10, y*10, x*10+10, y*10+10);
	if ((graphbuffer[0] = farmalloc(size)) == NULL) {
		closegraph();
		printf("Error: not enough heap space to store graphics!\n");
		exit(1);
	}
	getimage(x*10, y*10, x*10+10, y*10+10, graphbuffer[0]); // store background
	setcolor(0);
	setfillstyle(1,14);
	{
		int poly[]={x*10+5, y*10+5,
						x*10+7, y*10+7,
						x*10+5, y*10+7,
						x*10+5, y*10+10,
						x*10+5, y*10+7,
						x*10+3, y*10+7,
						x*10+5, y*10+5};
		fillpoly(7,poly); // draw cursor
	}
	while (!over) {
		start = time(NULL);
		if (_bios_keybrd(_KEYBRD_READY) != 0) {
			unsigned inkey = _bios_keybrd(_NKEYBRD_READ);
			if (inkey==Up || inkey==Down || inkey==Left || inkey==Right) // if move
				putimage(x*10, y*10, graphbuffer[0], TRANS_PUT); // put image
			switch (inkey)
			{
				case Up:
				y--;
				break;
				case Down:
				y++;
				break;
				case Left:
				x--;
				break;
				case Right:
				x++;
				break;
			}
			if (y<0)
				y=0;
			if (y>33)
				y=33;
			if (x<0)
				x=0;
			if (x>50)
				x=50;
			if (inkey==Up || inkey==Down || inkey==Left || inkey==Right) // if there was a move
			{
				getimage(x*10, y*10, x*10+10, y*10+10, graphbuffer[0]); // store background
				int poly[]={x*10+5, y*10+5,
								x*10+7, y*10+7,
								x*10+5, y*10+7,
								x*10+5, y*10+10,
								x*10+5, y*10+7,
								x*10+3, y*10+7,
								x*10+5, y*10+5};
				fillpoly(7,poly); // draw cursor
			}
			switch ((unsigned char)inkey)
			{
				case 13:
				over=1;
				soundfx(1,100);
				break; // end this
				case 9:
				menu_selection(1);
				help_display("Move the cursor with the arrow keys, and press tab to access the menu");
				setcolor(0);
				setfillstyle(1,14);
				break; // go to menu
			}
		};

		if (!tried_to_leave && (x==0 || y==0 || x==50 || y==33)) // if trying to exit
		{
			setcolor(15);
			help_display("Leave here?");
			tried_to_leave=1;
			setcolor(0);
			setfillstyle(1,14);
		}
		if (tried_to_leave && !(x==0 || y==0 || x==50 || y==33))
		{
			setcolor(15);
			help_display("Move the cursor with the arrow keys, and press tab to access the menu");
			tried_to_leave=0;
			setcolor(0);
			setfillstyle(1,14);
		}

		end = time(NULL);
		if (((.01-(difftime(end,start)))*1000)>0)
			delay( ( .01-( difftime(end,start) ) ) *1000 );
		if (music)
			playmusic(.01+.001);
	};
	setcolor(15); // reset color
	putimage(x*10, y*10, graphbuffer[0], TRANS_PUT); // put image back
	farfree(graphbuffer[0]); // use to fre ram after usage
	*selected_x = x;
	*selected_y = y;
}


//***************************** S t o r y   U p d a t e s ****************************************

void Story1(void)
{
	{ // sequence to load a new music file
	char filename[20] = "music\\";
	strcat(filename,"story.m");
	loadmusic(filename);
	} // end of sequence to load new music
	setactivepage(1);
	setvisualpage(0);
	{ // sequence to load an image
	char filename[20] = "images\\";
	strcat(filename,"aseoili.pcx");
	ReadPCX(filename,0,0,1);
	} // end of sequence to load image

	setcolor(0);
	{

	char *message[140]={"                                                    ",
							 "",
							 "",
							 "",
							 "Under a once great empire lies the land of Aseoili",
							 "",
							 "King Syrius IV the decendant of King Syrius I, II,",
							 "and III rules over the land of Aseoili, he is not",
							 "all that great of a king, but he keeps the tax",
							 "money reeling in. Anyway, one day his sooth sayer",
							 "gave him a message from the skies...",
							 "Sooth Sayer: \"A day of darkness threatens to fall",
							 "upon us.\"                                        ",
							 "King Syrius: \"...ok.\"                            ",
							 "Sooth Sayer: \"Our only chance for survival is to ",
							 "find the light that faded amongst the shadows.\"  ",
							 "King Syrius: \"So does that mean we will collect a",
							 "lot of taxes today?\"                             ",
							 "Sooth Sayer: \"...this is more important my Lord.\"",
							 "King Syrius: \"Excuse me?\"                        ",
							 "Sooth Sayer: \"Your taxes will do fine my Lord... ",
							 "Remember my message.\"                            ",
							 "",
							 "Needless to say King Syrius pays no attention to",
							 "the cryptic message and goes about his day. The",
							 "next day is a day of celebration, all enter the",
							 "castle for the festival to celebrate the",
							 "aniversiary of the day when King Syrius IV became",
							 "king. The king decides to walk through the crowds",
							 "and see how well the merchants are doing",
							 "collecting his future tax money. Then out of the",
							 "crowds runs a boy chasing a rat. The boy runs",
							 "through the guards and into the king. The guards",
							 "immediately draw there blades to the boys neck,",
							 "but the boy rolls out of the way and runs off. The",
							 "guards try to go chase him, but the king interupts",
							 "and tells the guards to \"let him go, he is only a",
							 "boy.\" Five years later the sky turns from daylight",
							 "to pitch black darkness and a storm arises.",
							 "Thunder, lightening, rain like never before, the",
							 "sky punishes the land. The boy from the festival,",
							 "now a teenager looks out of his window. He sees",
							 "the people who took him in as their son out in the",
							 "feilds trying to save their cattle as the",
							 "lightening strikes the ground around them. The",
							 "boy's adoptive parents are then struck directly",
							 "and they burn up at once. Then in the clouds form",
							 "the faint image of the head of a man, and its",
							 "looking down on the running people with a smile on",
							 "its face. The boy then remebers a message an old",
							 "fortune teller once told him.",
							 "",
							 "\"To defeat the cloud you must find the one it",
							 "looks for.\"",
							 "",
							 "The boy then notices that the face in the clouds",
							 "faces towards the castle and he immediately starts",
							 "running to the castle. The guards warn the king",
							 "that...",
							 "",
							 "Guard: \"...a boy just swam across the moat,       ",
							 "squeesed by the draw bridge and is now demanding  ",
							 "to see you.\"                                      ",
							 "King Syrius: \"Your joking right?\"                 ",
							 "Guard: \"No. And the strangest thing is, no rain is",
							 "falling on the boy.\"                              ",
							 "King Syrius: \"What? Let him in, bring him to me.\" ",
							 "Two guards walk in driping wet with the boy who is",
							 "completely dry.",
							 "",
							 "King Syrius: \"Why did you come here?\"",
							 "Boy: \"He killed my parents.\"         ",
							 "King Syrius: \"Who?\"                  ",
							 "",
							 "The boy points out the window to the head which",
							 "has now formed into a much clearer more visible",
							 "image. It then yells out the kings name and begins",
							 "to speak in a booming voice.",
							 "",
							 "Head: \"Exaclty 100 years ago your grandfather",
							 "sealed my father into a tomb on the island of",
							 "Yaknor. What was to be an exile changed to a death",
							 "sentence and your grandfather was the executioner.",
							 "He was the one to seal the tomb and weld it shut.",
							 "He was the one to order it buried 100 feet below",
							 "the earth. But my father survived, he survived and",
							 "broke free of your feeble attempt to murder him.",
							 "He lived 20 years in the ground until he found how",
							 "to break out. He suffered because your grandfather",
							 "was too weak to accept my fathers gifts. Now that",
							 "my time has come I will get revenge for my",
							 "father's suffering. I will make you and all of",
							 "your blood, all under your rule, all that you care",
							 "about, suffer like my father suffered.",
							 "Die!!!!!!!!!!!\"",
							 "",
							 "With that the storm gets worse and places start",
							 "catching a fire with each strike of lightening,",
							 "the sky roars louder with blast of thunder. The",
							 "king then laughs.",
							 "",
							 "King Syrius: \"Ha, he plans to kill me with a",
							 "storm?\"                                     ",
							 "The king looks towards the guards...",
							 "King Syrius: \"Do you see this? This little",
							 "pipsqueak plans to kill me with clouds.\"  ",
							 "They all then begin laughing vigorusly as the",
							 "storm worstens.",
							 "",
							 "Boy: \"As I was saying... he killed my parents and",
							 "now he will die.\"                                ",
							 "King Syrius: \"What? Now this boy too is sounding ",
							 "crazy.\"                                          ",
							 "The guards and the king begin laughing even",
							 "harder. Trying to control his laughter the king",
							 "clears his throat and says...",
							 "King Syrius: \"Here take one of these an go kill",
							 "that cloud for me.\"                            ",
							 "The king offers the boy a choice an old Bronze Axe",
							 "and a Cheap Foil that are rusting in the corner.",
							 "The guards laugh even louder.",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 "",
							 ""
							 };
	char *options[1] = {""}; // 117 (155 total)
	//                 visible_lines total_lines 4 music
	pop_up_window(message, options, 5, 140, 4, 1);

	// E N D   OF  I N I T I A L  STORY
	//free(message);
	}
	{
		char *message1[1] = {"The boy takes the..."};
		char *options1[2] = {"Axe?","Foil?"};

		Player.equiptmenthealth[0]=100;

		if (pop_up_window(message1, options1, 2, 2, 1, 1))
			Player.equiptmentname[0]=10; // foil
		else
			Player.equiptmentname[0]=12; // axe
	}

	// C O N C L U S I O N   O F   S T O R Y
	setcolor(0);
	{
		char *message2[51]={"                                                    ",
								  "",
								  "",
								  "",
								  "",
								  "",
								  "",
								  "",
								  "",
								  "Then he turns around to leave, but stops as he",
								  "remembers more of the message he was given, and",
								  "he turns back and says...",
								  "Boy: \"That amulete of yours, that could help.\"",
								  "The king onlys laughs even more and says...",
								  "King Syrius: \"Here take this peice of junk.\"",
								  "As he tosses the amulete to the boy.",
								  "King Syrius: \"...oh and by the way what is your",
								  "name so that I can spread the joke around?\"    ",
								  "Boy: \"Timothy.\"                               ",
								  "",
								  "The guards and the king fall on the floor laughing",
								  "as the boy leaves. The boy makes it out of the",
								  "castle and as soon as he does a giant fireball",
								  "streaks directly into the castle destroying it and",
								  "certaintly most of the royal army. The talking",
								  "head then speaks again.",
								  "",
								  "Head: \"I have just killed your king, your army,",
								  "and your navy. You have no ships in your seas, you",
								  "have no cattle in you farms, you have no fruits in",
								  "your orchards. You are destitute and I have not",
								  "finished my doings. My minoins will soon envelop",
								  "your lands. Already they roam your forest, your",
								  "mountains, you feilds, your beaches, and your",
								  "caves. They will only multiply and envelope your",
								  "land. I will not stop until everyone of you is",
								  "dead, may this rainbow be a symbol of that",
								  "promise.\"",
								  "",
								  "The cloud then fades from the sky and all of the",
								  "other clouds fade too as the storm calms, the",
								  "light returns, and a rainbow appears in the sky.",
								  "",
								  "",
								  "",
								  "",
								  "",
								  "",
								  "",
								  "",
								  "",
								 };
		char *options2[1] = {""}; // 117 (155 total)
		//                 visible_lines total_lines 4 music
		pop_up_window(message2, options2, 3, 51, 4, 1);
		give_player(0,1000); // amulet
	}
	Player.story=2; // next place in story
}

//**************************************** T o w n s *********************************************
void town1(void) // ----------------- T O W N  # 1 -----------------------------------------------
{
	town_data[0]=0; // beggar
	town_data[1]=0; // day
	town_data[2]=0; // talk with knight
	town_data[3]=0;
	town_data[4]=0;
	int location;
	town = 1;
	if (Player.story==1)
	{
		Story1();
		fadeout(); // fade out all colors to black
		getch();
		exit(0);
		//
		setactivepage(0); // go to alternate page
		cleardevice(); // clear it
		setvisualpage(0); // view blank screen
		setuppallete(); // restore color
		setactivepage(1); // go to main page

		{ // sequence to load an image // draw picture
		char filename[20] = "images\\";
		strcat(filename,"house.pcx");
		ReadPCX(filename,0,0,1);
		}
		setcolor(15);
		line(0,559,799,559); // help border

		setvisualpage(1); // view the image


		{ // sequence to load a new music file
		char filename[20] = "music\\";
		strcat(filename,"build.m");
		loadmusic(filename);
		} // end of sequence to load new music

		{
		char *message[9] = {"You are Timothy, and you have decided to continue",
								  "on your way to defeat the cloud but first you",
								  "decide to stop by your parents house so that you",
								  "can gather some tools for your mission. You",
								  "remember that you wrote down a copy of the full",
								  "message that the fortune teller gave you. It has",
								  "some more on it that you do not remember,",
								  "fortunately you hid it under your bed just in",
								  "case ever needed it again."
								 };
		char *options[1] = {""};
		pop_up_window(message, options, 9, 0, 0, 1);
		}
		location=0; //set location
	}
	else
	{
		{ // sequence to load an image // draw picture
		char filename[20] = "images\\";
		strcat(filename,"house.drw");
		load_image(filename,0,0);
		}
		setcolor(15);
		line(0,559,799,559); // help border

		{ // sequence to load a new music file
		char filename[20] = "music\\";
		strcat(filename,"tv.m");
		loadmusic(filename);
		} // end of sequence to load new music
		location=0; // set location

	}

	// E N D  O F   I N I T I A L  S T O R Y  P O I N T   S E T  U P
	int gone=0;
	while (!gone)
	{
		switch(location)
		{
		case 0: // parents house
		location = Tims_Home();
		break;
		case 2: // market
		location = Market();
		break;
		}
	}
}

int Tims_Home (void) // first location
{
	if (find(1,15))
	{ // sequence to load an image // draw picture
		char filename[20] = "images\\";
		strcat(filename,"house2.drw");
		load_image(filename,0,0);
	}
	else
	{ // sequence to load an image // draw picture
		char filename[20] = "images\\";
		strcat(filename,"house.drw");
		load_image(filename,0,0);
	}
	int location = 0;
	int x=25,y=17;
	int *xa=&x,*ya=&y;
	while (location==0)
	{
		user_cursor(1,xa,ya);
		// box format
		//if (x>(left bound) && x<(right bound) && y>(upper bound) && y<(lower bound))
		if (x>31 && x<43 && y>22 && y<26) // if with in flower box
		{
			int found = find(1,15); //find hammer
			//printf("%d ", found);
			if (!found) // if you don't have the hammer
			{
				{
					char *message[5] = {"You notice that something is in the dirt other",
											  "than flowers, you tug at it and finaly yank it",
											  "out. It appears that you have found a rusty",
											  "old hammer. It would make a nice weapon, but",
											  "it could use a little washing."
											 };
					char *options[1] = {""};
					pop_up_window(message, options, 5, 0, 0, 1);
					{ // sequence to load an image // draw picture
						char filename[20] = "images\\";
						strcat(filename,"house2.drw");
						load_image(filename,0,0);
					}
						setcolor(15);
						line(0,559,799,559); // help border
						give_player (1, 15); // hammer
				}
			}
			else
			{
				{
				char *message[3] = {"You go to dig some more but then the you",
										  "start to feel shame for the damage you",
										  "have already done."
										 };
				char *options[1] = {""};
				pop_up_window(message, options, 3, 0, 0, 1);
				}
			}
		}

		if (x>15 && x<26 && y>11 && y<25) // if with in door
		{
			if (!(find(0,1001) || find(0,1002) || find(0,1)))
			{
				char *message[9] = {"You enter you home and search around for whatever",
										  "you can find. Luck strikes and you find your",
										  "copy of the fortune teller's message, and",
										  "a healing potion. You then notice that your",
										  "father left behind his free pass to the training",
										  "hall. He may never have got the chance to use it",
										  "and one day move up from the status of a poor",
										  "farmer to that of a knight... But you definately",
										  "plan on putting it to use."
										 };
				char *options[1] = {""};
				pop_up_window(message, options, 9, 0, 0, 1);
				give_player (0, 1); // healing potion
				give_player (0, 1001); // tellers msg
				give_player (0, 1002); // T. Hall Pass
			}
			else
			{
				char *message[1] = {"You go in and sleep"
										 };
				char *options[1] = {""};
				pop_up_window(message, options, 1, 0, 0, 1);
				Player.hitpoints=100;
			}
		}

		if (x>31 && x<41 && y>12 && y<19) // if with in window
		{
			char *message[9] = {"You look in the window of your home and remeber",
									  "your parents and how proud they were of you.",
									  "You then remeber the day when they told you",
									  "why you do not look like them. You were mad",
									  "when you found out but your glad that you know.",
									  "Anyway, they did raise you, so that makes them",
									  "your \"real\" parents. You back away from the",
									  "glass and clench your fist determined to get",
									  "revenge."
									 };
			char *options[1] = {""};
			pop_up_window(message, options, 9, 0, 0, 1);
		}

		if (x>1 && x<46 && y>3 && y<10) // if with in roof
		{
			char *message[3] = {"You look up to your roof and try to climb up there",
									  "but, you just cannot reach it. You wish you were",
									  "taller, then again who doesn't."
									 };
			char *options[1] = {""};
			pop_up_window(message, options, 3, 0, 0, 1);
		}

		if (x==0 || y==0 || x==50 || y==33) // if trying to exit
		{
			char *message[2] = {"Where do you want to go?",
									  ""
									 };
			char *options[3] = {"No where",
									  "To the Training hall",
									  "To the market"};
			location=pop_up_window(message, options, 5, 3, 2, 1);
		}
	}
	if (!town_data[1] && Player.mission == 1) // still the first day
	{
		town_data[1]=1;
		{
			char *message[2] = {" Before you leave you decide to go inside and sleep. ",
									  "You wake up the next morning to a much calmer scene."
									 };
			char *options[1] = {""};
			pop_up_window(message, options, 2, 0, 0, 1);
		}
	}

	return location;
}

int Market (void)
{
	{ // sequence to load a new music file
	char filename[20] = "music\\";
	strcat(filename,"default.m");
	loadmusic(filename);
	} // end of sequence to load new music

	if (find(1,61)) // wooden buckler
	{ // sequence to load an image // draw picture
		char filename[20] = "images\\";
		strcat(filename,"market2.drw");
		load_image(filename,0,0);
	}
	else
	{ // sequence to load an image // draw picture
		char filename[20] = "images\\";
		strcat(filename,"market.drw");
		load_image(filename,0,0);
	}
	int location = 2;
	int x=25,y=17;
	int *xa=&x,*ya=&y;
	while (location==2)
	{
		user_cursor(1,xa,ya);
		// box format
		//if (x>(left bound) && x<(right bound) && y>(upper bound) && y<(lower bound))
		if (x>43 && x<47 && y>29 && y<32) // if with in red circle
		{
			int found = find(1,61); // wooden buckler
			//printf("%d ", found);
			if (!found) // if you don't have it
			{
				{
					char *message[4] = {"You brush the dirt off of the object on the",
											  "ground and discover that it is a small",
											  "sheild. Not very big or effective, but it",
											  "is better than having nothing."
											 };
					char *options[1] = {""};
					pop_up_window(message, options, 4, 0, 0, 1);
					{ // sequence to load an image // draw picture
						char filename[20] = "images\\";
						strcat(filename,"market2.drw");
						load_image(filename,0,0);
					}
						setcolor(15);
						line(0,559,799,559); // help border
						give_player (1, 61); // wooden buckler
				}
			}
		}

		if (x>1 && x<20 && y>17 && y<24) // beggar
		{
			if (!town_data[0] && Player.mission==1) // if you did not pay him already
			{
				char *message[9] = {"You approach the smelly old beggar on the street",
										  "corner and with a shaky grip he lifts his bowl",
										  "up to you and says...",
										  "Beggar :\"Please kind sir, could you spare a",
										  "coin or two?\"",
										  "The mere stench of the man is enough to make you",
										  "feel sorry for him or at least disgusted by him.",
										  "Still you know you need your money. What will",
										  "you do?"
										 };
				char *options[3] = {"Give him 1 GP","Give him 2 GP","Leave"};
				switch (pop_up_window(message, options, 10, 3, 1, 1))
				{
					case 0:
					if (Player.gold>0)
					{
						Player.gold--;
						char *message2[4] = {"Beggar :\"Thank you may the Heavens reward you",
													"kind sir!\"",
													"He then lowers his bowl covers it and goes to",
													"sleep laying over the bowl."
												 };
						char *options2[1] = {""};
						pop_up_window(message2, options2, 4, 0, 0, 1);
						town_data[0]=1; // beggar
					}
					break;
					case 1:
					if (Player.gold>2)
					{
						Player.gold-=2;
						char *message2[3] = {"Beggar :\"Oh thank you! Thank you! Here take this",
													"I have no need for it.\"",
													"The beggar hands you a bottle of healing potion."
												 };
						char *options2[1] = {""};
						pop_up_window(message2, options2, 3, 0, 0, 1);
						give_player (0, 1);
						town_data[0]=2; // beggar
					}
					break;
					case 2:
					{
						char *message2[2] = {"The beggar looks disappointed and he lowers",
													"his head looking in his empty bowl."
												 };
						char *options2[1] = {""};
						pop_up_window(message2, options2, 2, 0, 0, 1);
					}
					break;
				}
			}
			else
			{
				char *message2[1] = {"It appears that the beggar has gone to sleep"
										 };
				char *options2[1] = {""};
				pop_up_window(message2, options2, 1, 0, 0, 1);
			}
		}

		if (x>25 && x<35 && y>6 && y<19) // knight
		{
			int response;
			if (!town_data[2]) // first talk with knight
			{
				char *options[2] = {"Nothing","His knowledge of the cloud"};
				{
					char *message[2] ={"You walk up to the knight and over hear",
											 "him and the store owner talking..."
											};
					pop_up_window(message, options, 2, 0, 0, 1);
				}
				{
					char *message[6] = {"Knight: \"Me and the others are planning on killing that thing.\"",
											  "Store Owner: \"Why? He ended my need to pay taxes.\"             ",
											  "Knight: \"Yes, and he killed the king.\"                         ",
											  "Store Owner: \"...\"                                             ",
											  "Knight: \"Ok, ok... We hear their might be a reward.\"           ",
											  "Store Owner: \"Ok, and how dost thou plan on killing that guy.\" ",
											 };
					pop_up_window(message, options, 6, 0, 0, 1);
				}
				{
					char *message[6] = {"Knight: \"We found out a few things about that thing that will  ",
											  "         help us on our mission. That thing can be killed,     ",
											  "         trust me.\"                                            ",
											  "Store Owner: \"Well then good luck my friend.\"                  ",
											  "The knight then turns and notices you, but he says nothing",
											  "You respond asking him about..."
											 };
					response=pop_up_window(message, options, 7, 2, 1, 1);
				}
				town_data[2]=1;
			}
			else
			{
				char *message[1] ={"             Knight: \"What dost thou wish to know?             ",
										 };
				char *options[2] = {"Nothing","His knowledge of the cloud"};
				response=pop_up_window(message, options, 2, 2, 1, 1);
			}
			if (response)
			{
				char *options[1] = {""};
				if (Player.mission==1)
				{
				  {
					char *message[9]= {"You ask him...                                               ",
											 "Timmy:\"So what of the cloud dost thou know?\"                ",
											 "Knight:\"My what a funny little brat. Why wouldst I tell thee",
											 "anything? Thou couldst do nothing with the info",
											 "anyway.\"",
											 "You draw out your weapon partially and respond...",
											 "Timmy:\"I can be quite capable, so what dost thou know!\"",
											 "The knight chuckles a little and then responds...",
											 "Knight:\"Ye be a funny one. But for thy bravery I'll give thee"
											};
					pop_up_window(message, options, 9, 0, 0, 1);
				  }
				  {
					char *message[8]={"Knight:\"a chance to discover what I know. If thou can go to",
											"the 'Cave of Spiders' located south of here, and clear it",
											"out. I will tell thee what thou dost desire to know. If",
											"thou succeeds then find me at Dalabar and bring me the",
											"head of the Black Widow.\"",
											"Timmy: \"Then that is what shall be done.\"",
											"With that you put back your weapon and the knight leans",
											"back on the side wall of the store."
										};
					pop_up_window(message, options, 8, 0, 0, 1);
				  }
				  Player.mission=2;
				}
				else
				{
					char speaker[]="Knight";
					char text[]="The 'Cave of Spiders' is located south of here."
					"After thou hast vanquished the Black Widow bring "
					"his head to me. I will be in Dalabar.\"";
					dialog_display (speaker,text,1);
				}
			}
		}

		if (x>14 && x<30 && y>3 && y<18) // store
		{
			char speaker[]="Store Keeper";
			char text[]="Good day sir, whatever thy needs be I am sure to have it. "
			"Just take a look around and tell me what you want.\"";
			dialog_display (speaker,text,1);

			idstat on_sale[]={1,4}; // heal, cure
			int prices[]={50,100};
			char *comments[]={"When injury strikes ye shall whish ye had this.",
									"Thou shalt need this if thy travels pass through wilderness."};
			int choices = 2, music = 1;
			Store1 (on_sale, prices, comments, choices, music);
		}

		if (x==0 || y==0 || x==50 || y==33) // if trying to exit
		{
			char *message[2] = {"Where do you want to go?",
									  ""
									 };
			char *options[3] = {"Home",
									  "To the Training hall",
									  "No where"};
			location=pop_up_window(message, options, 5, 3, 2, 1);
		}
	}
	return location;
}

void get_visual (idstat item_id, int *x, int *y, int *width, int *height, char *image)
{
	switch (item_id)
	{
		case 1: // Heal Potion
			*width=50;
			*height=17;
			strcpy(image,"items\\p.drw");
			break;
		case 2: // Major Heal
			*width=50;
			*height=17;
			strcpy(image,"items\\p.drw");
			break;
		case 3: // Revival
			*width=50;
			*height=17;
			strcpy(image,"items\\p.drw");
			break;
		case 4: // Cure
			*width=50;
			*height=17;
			strcpy(image,"items\\c.drw");
			break;
		case 5: // Major Cure
			*width=50;
			*height=17;
			strcpy(image,"items\\p.drw");
			break;
		case 6: // Full Cure
			*width=50;
			*height=17;
			strcpy(image,"items\\p.drw");
			break;
		case 10: // Fairy Dust
			*width=50;
			*height=17;
			strcpy(image,"items\\p.drw");
			break;
	}
	*x = menumargin/2 - *width/2;
	*y = 175 - *height/2;
}

void Store1 (idstat on_sale[], int prices[], char *comments[], int choices, int music)
{
	music*=-1;
	setcolor(15);
	int gone=0, selected=0, x, y, width, height;
	char image[15];

	menu_display(1, 1);
	help_display("Left and right to select, Esc to exit, Enter to Buy, and ? to ask about item");
	get_visual (on_sale[selected], &x, &y, &width, &height, image);
	{
		char caption[24];
		itoa(prices[selected],caption,10);
		strcat(caption," GP\0");
		image_pop_up (x, y, width, height, music, image,caption);
		setcolor(15);
	}
	while (!gone)
	{
		if (_bios_keybrd(_KEYBRD_READY) != 0)
		{
			{
				setcolor(0);
				setfillstyle(0,0);
				bar(menumargin+1, 9*textcoordinate+142,
						799, 10*textcoordinate+142);
				setcolor(15);
			}
			number_to_string(Player.gold, 10, 0);
			strcat(string_transfere," GP");
			center(menumargin+1, 799, 9*textcoordinate+142, string_transfere);
			get_visual (on_sale[selected], &x, &y, &width, &height, image);
			{
				char caption[24];
				itoa(prices[selected],caption,10);
				strcat(caption," GP\0");
				image_pop_up (x, y, width, height, music, image,caption);
				setcolor(15);
			}

			unsigned inkey = _bios_keybrd(_NKEYBRD_READ);
			if ((unsigned char)inkey==27)
			{
				gone=1;
				soundfx(1,20);
			}
			if ((unsigned char)inkey==13) // buy
			{
				if (prices[selected]<=Player.gold)
				{
					Player.gold-=prices[selected];
					give_player(0,on_sale[selected]);
					soundfx(1,100);
					soundfx(1,20);
				}
				if (prices[selected]>Player.gold)
				{
					char *message[1] = {"If only you had the money..."
										 };
					char *options[1] = {""};
					pop_up_window(message, options, 1, 0, 0, music);
				}

				time_t beginT=time(NULL), endT=time(NULL);
				while ((time(NULL)-beginT) < (time_t).01)
				{
					int wrong=0;
					do {
						if (bioskey(1) !=0)
						{
							unsigned char otherkey = bioskey(0);
							if (otherkey==13)
								wrong=1;
						}
					} while (wrong);
				}
			}
			if ((unsigned char)inkey=='/' || (unsigned char)inkey=='?') // ask about
			{
				char name[12];
				char description[80];
				get_item(on_sale[selected], name, description);
				description[63]=0;
				char *message[5] = {"This is a                                                      ",
										  name,
										  "This item",
										  description,
										  comments[selected]
										 };
				char *options[1] = {""};
				pop_up_window(message, options, 5, 0, 0, music);
			}

			switch (inkey)
			{
			case Right:
			selected++;
			break;
			case Left:
			selected--;
			break;
			}
			if (selected>=choices)
				selected=0;
			if (selected<0)
				selected=choices-1;

			{
				setcolor(0);
				setfillstyle(0,0);
				bar(menumargin+1, 9*textcoordinate+142,
						799, 10*textcoordinate+142);
				setcolor(15);
			}
			number_to_string(Player.gold, 10, 0);
			strcat(string_transfere," GP");
			center(menumargin+1, 799, 9*textcoordinate+142, string_transfere);
			get_visual (on_sale[selected], &x, &y, &width, &height, image);
			{
				char caption[24];
				itoa(prices[selected],caption,10);
				strcat(caption," GP\0");
				image_pop_up (x, y, width, height, music, image,caption);
				setcolor(15);
			}
		}
	}
}

void dialog_display (char speaker[], char text[], int music) // formats and displays dialog
{
	int feild_width = 61-strlen(speaker);
	int lines = strlen(text)/feild_width; // estimate
	char line1[63], line2[63], line3[63], line4[63], line5[63], line6[63], line7[63], line8[63];
	char *delivery[8] = {line1,line2,line3,line4,line5,line6,line7,line8};

	{
		for (int cl=0;cl<8;cl++)
			for (int register cc=0;cc<63;cc++)
				*(delivery[cl]+cc)=0;
	}

	strcpy(line1,speaker);
	strcat(line1,": ");

	int indented = strlen(line1);

	{
		char indent[30], letter=' ';
		for (int register az = 0; az<30; az++)
			indent[az]=0;
		strnset(indent, letter, strlen(line1));
		for (int at=1; at<8; at++)
		{
			strcpy(delivery[at],indent);
		}
	}

	int back=0, cut_off=0, charecter=0;
	for (int line=0; line<8; line++)
	{
		char T_string[63];
		{
			for (register int az=0; az<62; az++)
				T_string[az]=0;//' ';
			for (az=0; az<indented; az++)
				T_string[az]=' ';
		}

		for (int i = 0; i<feild_width; i++)
		{
			char one[2]={0,0}, a=0;
			*(delivery[line]+i+indented-1)=text[charecter+back];
			a = *(delivery[line]+i+indented-1);
			one[0]=a;
			if (!(i==0 && a==' '))
				strcat(T_string,one);
			if ((charecter+back)==(strlen(text)-1))
			{
				cut_off=1;
				break;
			}
			charecter++;
		}
		if (*(delivery[line]+feild_width-2+indented)!=' ' && *(delivery[line]+feild_width+indented-1)!=' ' && !cut_off)
		{
			int a=0;
			for (int reverse=1; reverse<63; reverse++)
			{
				if (*(delivery[line]+feild_width-reverse+indented-1)!=' ')
				{
					*(delivery[line]+feild_width-reverse+indented-1)=' ';
					a++;
				}
				else
					break;
			}
			for (int ff=63-a-1;ff<63;ff++)
				T_string[ff]=' ';
			back += reverse*-1;
		}
		if (strlen(T_string)<63)//(cut_off)
		{
			for (int padd=strlen(T_string); padd<63; padd++)
				T_string[padd]=' ';
		}

		if (line==0)
			for (int t = 0;t<indented;t++)
				T_string[t]=line1[t];
		strcpy(delivery[line],T_string);

		if (cut_off)
		{
			for (int padd=strlen(T_string); padd<63; padd++)
				T_string[padd]=' ';
			if (line<7)
				line++;
			break;
		}
	}
	char *options[1] = {""};
	line1[indented-1]='"';
	pop_up_window(delivery, options, line, 0, 0, music);
}
