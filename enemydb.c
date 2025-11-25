#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shared\enemy.h"

#define MAX_LINE 255


int pack_file(const char *in_path, const char *out_path){

	FILE *in_file;
   FILE *out_file;
   char line[MAX_LINE];
   int enemy_count;

   EnemyHeader header;
   Enemy enemy;

   enemy_count = 0;
   header.Count = 0;
   header.Version = 1;

   if ((in_file = fopen(in_path, "r")) == NULL) {
     printf("Error opening file %s.\n", in_path);
     return 1;
   }

   if ((out_file = fopen(out_path, "wb")) == NULL) {
     printf("Error opening file %s.\n", out_path);
     fclose(in_file);
     return 1;
   }

   fwrite(&header, sizeof(EnemyHeader), 1, out_file);


   while (fgets(line, MAX_LINE, in_file)){
      if (strncmp(line, "name=", 5) == 0) {
        const char *src = line + 5;
        size_t len = strcspn(src, "\n"); // length up to newline
        if (len >= sizeof(enemy.Name)) {
          len = sizeof(enemy.Name) - 1; // truncate safely
        }
        memcpy(enemy.Name, src, len);
        enemy.Name[len] = '\0'; // null terminate at actual length
      }
      if (strncmp(line, "health=", 7) == 0) {
      	sscanf(line, "health=%d",
         &enemy.Health);
      }
      if (strncmp(line, "[End]", 5) == 0) {
      	fwrite(&enemy, sizeof(Enemy), 1, out_file);
      }
      if (strncmp(line, "[Start]", 7) == 0){
      	//card.Title = "";
         //card.Description = "";
         enemy.Health = 0;
         enemy_count++;
      }
   }

   header.Count = enemy_count;

   fseek(out_file, 0, SEEK_SET);
   fwrite(&header, sizeof(EnemyHeader), 1, out_file);


   fclose(in_file);
   fclose(out_file);

   return 0;
}

int validate(const char *packed_file){

	EnemyHeader header;
   Enemy enemy;
   int i;
   FILE *file;

   if ((file = fopen(packed_file, "rb")) == NULL) {
     printf("Error opening file %s.\n", packed_file);
     return 1;
   }

   fread(&header, sizeof(EnemyHeader), 1, file);
   printf("Enemy count %d\n", header.Count);
   printf("Version %d\n", header.Version);
	for (i = 0; i < header.Count; i++){
		fread(&enemy, sizeof(Enemy), 1, file);
      printf("Name = %s\n", enemy.Name);
      printf("Health = %d\n", enemy.Health);
      printf("======\n");
   }

   return 0;
}

/*
	This program will pack an enemy definition file into struct and then
   into binary for use in the game.
*/

int main(int argc, char *argv[]) {

	 const char *in_path;
    const char *out_path;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s (<input.txt>|validate) <output.ene>\n", argv[0]);
        return 1;
    }

	 in_path = argv[1];
    out_path = argv[2];

    if (strncmp(in_path, "validate", 8) == 0){
    	return validate(out_path);
    } else {
      return pack_file(in_path, out_path);
    }
    

}