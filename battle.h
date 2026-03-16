#include "keyboard.h"
#ifndef BATTLE_H
#define BATTLE_H

void init_battle();
void destroy_battle();
void update_battle();
void render_battle();
void keypress_battle(KeyEvent event);
void keypress_battle_select(KeyEvent event);

void draw_card();
void play_card();

#endif