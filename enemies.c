#include <alloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include "enemies.h"

EnemyHeader enemy_header;
Enemy far * enemies;

void load_enemies(){
   int i;
   FILE *file;
   Enemy * temp;

   if ((file = fopen("enemies.db", "rb")) == NULL) {
     printf("Error opening file %s.\n", "enemies.db");
     return 1;
   }

   fread(&enemy_header, sizeof(EnemyHeader), 1, file);
   enemies = (Enemy far *)farmalloc(sizeof(Enemy) * enemy_header.Count);
	if (enemies == NULL){
		printf("far malloc failed");
	}
	fread(enemies, sizeof(Enemy), enemy_header.Count, file);

}

void unload_enemies(){
	farfree(enemies);
}

void get_enemy(int id, Enemy* enemy){
  Enemy far *src;
  if (id >= enemy_header.Count){
    return;
  }
  src = (Enemy far *)enemies + id;
  //copy to the near pointer
  enemy->Health = src->Health;
  _fstrcpy(enemy->Name, src->Name);
}