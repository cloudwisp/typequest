#include "keyboard.h"
#ifndef BATTLE_H
#define BATTLE_H

typedef enum {
	Intro,
   Select,
   Play,
   Score
} BattleStep;

void init_battle();
void destroy_battle();
void update_battle();
void render_battle();
void keypress_battle(KeyEvent event);

void enter_step_intro();
void render_step_intro();
void update_step_intro();
void keypress_step_intro(KeyEvent event);

void enter_step_select();
void render_step_select();
void update_step_select();
void keypress_step_select(KeyEvent event);

void enter_step_play();
void render_step_play();
void update_step_play();
void keypress_step_play(KeyEvent event);

void enter_step_score();
void render_step_score();
void update_step_score();
void keypress_step_score(KeyEvent event);



#endif