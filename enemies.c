#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "enemies.h"

EnemyHeader enemy_header;
Enemy far * enemies;

void load_enemies(){
   int i;
   FILE *file;
   Enemy temp;

   if ((file = fopen("enemies.db", "rb")) == NULL) {
     printf("Error opening file %s.\n", "enemies.db");
     return 1;
   }

   fread(&enemy_header, sizeof(EnemyHeader), 1, file);
   enemies = (Enemy far *)farmalloc(sizeof(Enemy) * enemy_header.Count);

   for (i = 0; i < enemy_header.Count; i++){
      fread(&temp, sizeof(Enemy), 1, file);
      //_fmemcpy((char far *)enemies + (i * sizeof(Enemy)), &temp, sizeof(Enemy));
   }
  //fread(enemies, sizeof(Enemy), enemy_header.Count, file);
}

void unload_enemies(){
	farfree(enemies);
}