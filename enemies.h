#include "shared\enemy.h"

#ifndef enemies_c
#define enemies_c

extern EnemyHeader enemy_header;
extern Enemy far * enemies;

void load_enemies();
void unload_enemies();

#endif