#include "pldata.h"

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