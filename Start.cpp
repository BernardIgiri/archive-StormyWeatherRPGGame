//   K  E  Y  B  O  A  R  D     S  C  A  N     C  O  D  E  S

// A R R O W   K E Y S

#define Down 20704
#define Left 19424
#define Right 19936
#define Up 18656


#define Delete 21472

// end of scan codes

#define menumargin 639
#define textcoordinate textvsize()
#define PI 3.14159265359

#include <stdio.h>
#include <process.h>
#include <malloc.h>
#include <bios.h>
#include <graphics.h>
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <dir.h>

float totaltime=0, oldtime=0, tempo=.1;
int position=0, octave=4;
char song[400], string_transfere[64];

void pause(int mode);
// Plays Loaded music
void playmusic(float time_passed);

int textvsize(void)
{
	return (textheight("|") + 1);
}


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


void center(int beginx, int endx, int y, char enteredtext[]) // for centering text graphically
{
	int x = ((endx-beginx)/2) - (textwidth(enteredtext)/2) + beginx;
	outtextxy(x,y,enteredtext);
	return;
}

int findycenter(int lines) // for vertically centering text graphically
{
	return (getmaxy()/2)-((textheight("|")*lines)/2);
}

int pop_up_window(char *message[], char *options[], int lines, int choices, int mode, int music)
{
	sound(300);
	delay(50);
	nosound();
	int result = 0,
	// define useful varibles xymin to xymax and width
	x1 = ((menumargin-1)/2)-(textwidth(message[0])/2),
	y1 = findycenter(lines) - 10,
	x2 = (((menumargin-1)/2) - (textwidth(message[0])/2)+textwidth(message[0])),
	y2 = (findycenter(lines)+textcoordinate*lines)-10,
	width = x2-x1;

	void far *graphbuffer[2]; // graphics buffer

	unsigned size = imagesize(x1-2, y1-2, x2+2, y2+2);
	if ((graphbuffer[0] = farmalloc(size)) == NULL) {
		closegraph();
		printf("Error: not enough heap space in save_screen().\n");
		exit(1);
	}

	getimage(x1-2, y1-2, x2+2, y2+2, graphbuffer[0]); // store background

	setcolor(8);
	// window
	{
		int poly[10]={x1-2, y1-2, x2+2, y1-2, x2+2, y2+2, x1-2, y2+2, x1-2, y1-2};
		setfillstyle(1, 7);
		fillpoly(5, poly);
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
	}
	nosound();

	putimage(x1-2, y1-2, graphbuffer[0], COPY_PUT); // put image
	farfree(graphbuffer[0]); // use to fre ram after usage

	return result;
}

//*********************************M U S I C   F U N C T I O N S**********************************
// Plays a musical note
void playnote(char note);
// Performs the next command in a Music string
void check_for_octave(void);
// Loads the music into the music buffer
void loadmusic(char filename[]);


void loadmusic(char filename[]) // loads music
{
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


void pause(int mode) // pause PAUSE;
{
	time_t beginT=time(NULL);
	while ((time(NULL)-beginT) < .01)
		if (bioskey(1) !=0)
				char inkey = bioskey(0);

	if (mode==1)
	{
		while (bioskey(1) == 0)
		{
			delay(10);
			playmusic(.01+.001); // .01 can be set to anything
		}
		nosound();
	}
	if (mode>=10)
	{
		time_t t;
		t = time(NULL);
		time_t total_time = mode/10 - .01;
		while (difftime(time(NULL),t) < total_time) {
			//delay(10);
			//playmusic(.01+.001);
			if (bioskey(1))
				break;
		};
	}
	else
		while (bioskey(1) == 0);
	return;
}

//void playnote(char note);
//**************************************************************************************************
//*********************************** I M A G E   L O A D I N G ************************************
void draw_image(char command[], int x, int y);
void Draw(char command[], int pos, int type, int x, int y);
void submitt_pal_settings(char command[]);
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

void load_pallete(char filename[]) // loads pallete file into the command string
{
	FILE *fptr;
	if ((fptr = fopen(filename, "r")) == NULL){
		printf("Cannot open %s.\n", filename);
		exit(1);
	}
	int c = 0;
	int i=1;
	char command[400]=""; // to receive data
	// ---------
	char all_the_chars[256], i2;
	all_the_chars[0] = 0;
	for (i = 1, i2 = 1; i2 != 0; i++, i2++)
		all_the_chars[i] = i2;
	// ---------
	i = 0;
	for (int ioi=0; ioi<400; ioi++)
		command[ioi]=0;
	while ((c=fgetc(fptr)) != EOF){
		command[i]=(all_the_chars[c]);
		i++;
		if (c == ';') // end of command marked by a ";"
		{
			command[i]=0; // append the null charecter
			submitt_pal_settings(command); // figures what to draw
			break;
			//i = 0;
			//strcpy(command,""); // clear, partially
			//for (int ioi=0; ioi<400; ioi++)
			//	command[ioi]=0;
		}
	}
	fclose(fptr);
}

void submitt_pal_settings(char command[]) // coverts command sring into numbers in the pallete array
{
	int varibles[48] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int v=0;
	//int varnum=0;
	char var[4] = "   ";
	for (int i = 0, varnum = 0; i < strlen(command); i++)
	{
		var[4] = 0;
		if (v==4)
		{
			v=0;
			var[0] = ' ';
			var[1] = ' ';
			var[2] = ' ';
			var[3] = ' ';
			varnum++;
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
				//varibles[varnum] = atoi(var);
				//int value = varnum;
				//value++;
				int a = varnum;
				varibles[varnum] = atoi(var);
				a++;
				varnum = a;
				//printf("%d ",atoi(var));
				//printf("%d ",varibles[varnum]);
				//printf("%d ",varnum);
				//if(atoi(var)&&varnum); // do not know why but it makes it work
				v=0;
				var[0] = ' ';
				var[1] = ' ';
				var[2] = ' ';
				var[3] = ' ';
			}
			break;
		}
	}
	int tv = 0;
	for (int d1 = 0; d1 < 16; d1++)
		for (int d2 = 0; d2 < 3; d2++)
		{
			palset[d1][d2] = varibles[tv];
			tv++;
		};
	setuppallete();
	return;
}

void pallete_command(char command[]) // reads filename of pallete to be opened
{
	char filename[80];
	for (int iv = 0; iv <=80; iv++)
		filename[iv]=0;
	for (int i = 1, iz=0; i < strlen(command); i++, iz++)
	{
		filename[iz]=command[i]; // copy string
		if (filename[iz]==';') // prevent ; from being read
			filename[iz]=0;
	}
	load_pallete(filename);
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


	struct palettetype pal;
	getpalette(&pal);

	for (int ii=0; ii < 16; ii++)
	{
		//palset[ii][0],palset[ii][1],palset[ii][2]
		int red = palset[ii][0],
			green = palset[ii][1],
			blue = palset[ii][2];
		setrgbpalette(pal.colors[ii],red,green,blue);
	}//*/
}

////////////////////////////////////END OF IMAGE LOADING//////////////////////////////////////////
//************************************************************************************************

void initialize_graphics(void) // initializes graphics (640x350, 16 colors, 2 pages)
{
	clrscr ();
	/* request autodetection */
	int gdriver = 9, gmode=1, errorcode;

	/* initialize graphics mode */
	initgraph(&gdriver, &gmode, "c:\bc45\bgi\*.*");

	/* read result of initialization */
	errorcode = graphresult();

	if (errorcode != grOk)    /* an error occurred */
	{
		printf("Graphics error: %s\n", grapherrormsg(errorcode));
		printf("Press any key to exit.");
		getch();
		exit(1);               /* return with error code */
	}
	return;
}

void fadeout(void)
{
	struct palettetype pal;
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
	}
	return;
}
//settextstyle(int font, int direction, int charsize)

void main (void)
{
	clrscr ();
	// request autodetection /
	initialize_graphics();
	setuppallete();

	center(0, 639,findycenter(7)+0*(textheight("|")+1),"Presenting");
	pause(20);
	center(0, 639,findycenter(7)+1*(textheight("|")+1),"The worlds most original RPG game ever created");
	pause(20);
	center(0, 639,findycenter(7)+2*(textheight("|")+1),"Brought to you by");
	pause(20);
	center(0, 639,findycenter(7)+4*(textheight("|")+1),"Bernard Igiri");
	pause(20);
	center(0, 639,findycenter(7)+6*(textheight("|")+1),"it begins...");
	pause(20);

	{
		float anime = .1;
		time_t start, end = time(NULL);
		start = time(NULL);
		for (float i=0; i < 25; i++)
		{
			putpixel(.5*(i*i), 240, 1);
			end = time(NULL);
			if (((anime-(difftime(end,start)))*1000)>0)
				delay( ( anime-( difftime(end,start) ) ) *1000 );
			start = time(NULL);
			putpixel(.5*(i*i), 240, 0);
		}
		start = time(NULL);
		i = 0;
		for (float l=0; l < 360; l+=45)
		{
			setcolor(1);
			//arc(338, 200, 270+(.5*(i*i)), 272+(.5*(i*i)), 40);
			arc(338, 200, 270+i, 272+i, 40);
			end = time(NULL);
			if (((anime-(difftime(end,start)))*1000)>0)
				delay( ( anime-( difftime(end,start) ) ) *1000 );
			start = time(NULL);
			setcolor(0);
			//arc(338, 200, 270+(.5*((i-1)*(i-1))), 272+(.5*((i-1)*(i-1))), 40);
			arc(338, 200, 270+i, 272+i, 40);
			i+=45;
			if (i>360)
				i = -270;
		}

		start = time(NULL);
		for (i=27; i < 36; i++)
		{
			putpixel(.5*(i*i), 240, 1);
			end = time(NULL);
			if (((anime-(difftime(end,start)))*1000)>0)
				delay( ( anime-( difftime(end,start) ) ) *1000 );
			start = time(NULL);
			putpixel(.5*((i-1)*(i-1)), 240, 0);
		}
	}


	cleardevice();
	setactivepage(1);
	setvisualpage(0);

	settextstyle(6,0,1);
	setcolor(15);
	center(0, 639,findycenter(5)+0*textheight("|"),"As the light gives way to darkness");
	center(0, 639,findycenter(5)+1*textheight("|"),"And the sky takes form of roughness");
	center(0, 639,findycenter(5)+2*textheight("|"),"Remeber the echoing sound that silenced the heavens");
	center(0, 639,findycenter(5)+3*textheight("|"),"And pray the one that faded glows bright again");
	center(0, 639,findycenter(5)+4*textheight("|"),"So that you may have light again..");

	setvisualpage(1); // visible

	pause(30);
	fadeout();

	setvisualpage(0); //invisible

	cleardevice();

	setactivepage(1);
	setvisualpage(0);
	setuppallete();
	load_image("images\\intro.drw",0,0);
	{
		settextstyle(7,0,10);
		setcolor(4);
		int x = textwidth("R"), y = textheight("R")/3;
		outtextxy(24,14,"R");
		settextstyle(7,0,5);
		outtextxy(24+x,14+y,"eturn of Stormy");
		outtextxy(20+x+textwidth("eturn of Stormy")/2 - textwidth("Weather")/2
		,11+y+textheight("|"),"Weather");
		setusercharsize(1, 1, 3, 1);
		settextstyle(10,0,0);
		setcolor(0);
		center(0, 639, 0, "Shades of Darkness");
	}
	setvisualpage(1);
/*
DEFAULT_FONT	0	8x8 bit-mapped font
TRIPLEX_FONT	1	Stroked triplex font
SMALL_FONT	2	Stroked small font
SANS_SERIF_FONT	3	Stroked sans-serif font
GOTHIC_FONT	4	Stroked gothic font
SCRIPT_FONT	5	Stroked script font
SIMPLEX_FONT	6	Stroked triplex script font
TRIPLEX_SCR_FONT	7	Stroked triplex script font
COMPLEX_FONT	8	Stroked complex font
EUROPEAN_FONT	9	Stroked European font
BOLD_FONT	10	Stroked bold font */

	loadmusic("music\\intro.m");
	pause(30);
	//delay(2000);
	fadeout();
	setactivepage(1);
	cleardevice();
	setvisualpage(1);
	setactivepage(0);
	load_image("images\\menu.drw",0,0);
	setuppallete();
	setvisualpage(0);
	setactivepage(0);

	{
		setusercharsize(1, 2, 3, 2);
		settextstyle(10,0,0);
		setcolor(0);
		center(0, 639, 0, "Shades of Darkness");

		setusercharsize(2, 3, 3, 2);
		settextstyle(8,0,0);
		setcolor(0);
		center(0, 639, 250, "Open the door the knocking echoes.");

		settextstyle(7,0,5);
		setcolor(4);
		int x = textwidth("R"), y = textheight("R")/3;
		outtextxy(84,0,"R");
		settextstyle(7,0,3);
		outtextxy(84+x,0+y,"eturn of Stormy");
		outtextxy(80+x+textwidth("eturn of Stormy")/2 - textwidth("Weather")/2
		,y+textheight("|")-3,"Weather");
	}
	// 113 - 233


	settextstyle(0,0,1);
	setcolor(10);
	center(0, 639,findycenter(3)-textheight("|"),"New Game");
	setcolor(2);
	center(0, 639,findycenter(3)+textheight("|"),"Continue");
	center(0, 639,findycenter(3)+3*textheight("|"),"Exit Game");

	int over = 0, selection = 0, done = 0;
	time_t start, end = time(NULL);
	do	{
		while (!over)
		{
			start = time(NULL);
			if (_bios_keybrd(_KEYBRD_READY) != 0)
			{
				unsigned inkey = _bios_keybrd(_NKEYBRD_READ);
				switch (inkey)
				{
				case Up:
				selection--;
				break;
				case Down:
				selection++;
				break;
				}
				if (selection < 0)
					selection=2;
				if (selection > 2)
					selection=0;
				if ((unsigned char)inkey==13)
					over=1;
				// reshow choices
				setcolor(2);
				center(0, 639,findycenter(3)-textheight("|"),"New Game");
				center(0, 639,findycenter(3)+textheight("|"),"Continue");
				center(0, 639,findycenter(3)+3*textheight("|"),"Exit Game");
				setcolor(10);
				switch (selection)
				{
				case 0:
				center(0, 639,findycenter(3)-textheight("|"),"New Game");
				break;
				case 1:
				center(0, 639,findycenter(3)+textheight("|"),"Continue");
				break;
				case 2:
				center(0, 639,findycenter(3)+3*textheight("|"),"Exit Game");
				break;
				}
			} // if key pressed control end
			end = time(NULL);
			if (((.01-(difftime(end,start)))*1000)>0)
				delay( ( .01-( difftime(end,start) ) ) *1000 );
			playmusic(.01+.001);
		} // while loop end
		nosound();
/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// R E S P O N S E   T  O   U S E R ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
		switch (selection)
		{
		case 0: // new game
		{ // complete transfere of control from this program to the next
			cleardevice();
			closegraph();
			spawnlp(P_OVERLAY, "new", "new.exe", NULL);
		} // end of transfere
		break;
		case 1: // load
		//*********************************************************************************************
		//                                       FILE SELECT TEST
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
			if (result != file_number) // laoding something
			{ // complete transfere of control from this program to the next
				char save_file[9]={0,0,0,0,0,0,0,0,0};
				strcpy(save_file,file_list[result]);
				char *arg0 = save_file;
				cleardevice();
				closegraph();
				spawnlp(P_OVERLAY, "rpg", "rpg.exe",arg0, NULL);
			}
			else
         	over = 0;
		}
		break;
		}
	} while (selection!=2);
	cleardevice();
	closegraph();
}
