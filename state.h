#include "typedef.h"
#ifndef STATE_H
#define STATE_H


#define MAX_INVENTORY 50

typedef enum {
	Unset,
   Menu,
   Map,
   Battle
} GameScreen;

#pragma pack(push, 1)
typedef struct
{
	byte level;
	byte health;
   byte mana;
   byte endurance;
   byte shield;
   byte damage;
   byte dexterity;
   byte intelligence;
   byte charisma;
} PlayerStats;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
	char player_name[25];
   GameScreen screen;
   int difficulty;
   PlayerStats stats;
   int inventory[MAX_INVENTORY];
   int enemyid;
} GAME_STATE;
#pragma pack(pop)


//TODO: add load/save?

void set_initial_state(GAME_STATE *state);

void change_screen(GameScreen screen); //Implement this in main c file not state.c
void exit_program();

#endif