#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shared\card.h"

#define MAX_LINE 255


int pack_file(const char *in_path, const char *out_path){

	FILE *in_file;
   FILE *out_file;
   char line[MAX_LINE];
   int card_count;

   CardHeader header;
   Card card;

   card_count = 0;
   header.count = 0;
   header.version = 1;

   if ((in_file = fopen(in_path, "r")) == NULL) {
     printf("Error opening file %s.\n", in_path);
     return 1;
   }

   if ((out_file = fopen(out_path, "wb")) == NULL) {
     printf("Error opening file %s.\n", out_path);
     fclose(in_file);
     return 1;
   }

   fwrite(&header, sizeof(CardHeader), 1, out_file);


   while (fgets(line, MAX_LINE, in_file)){
      if (strncmp(line, "title=", 6) == 0) {
        const char *src = line + 6;
        size_t len = strcspn(src, "\n"); // length up to newline
        if (len >= sizeof(card.Title)) {
          len = sizeof(card.Title) - 1; // truncate safely
        }
        memcpy(card.Title, src, len);
        card.Title[len] = '\0'; // null terminate at actual length
      }
      if (strncmp(line, "effect_type=", 12) == 0) {
      	sscanf(line, "effect_type=%d",
         &card.EffectType);
      }
      if (strncmp(line, "effect_amount=", 14) == 0) {
      	sscanf(line, "effect_amount=%d",
         &card.Effect);
      }
      if (strncmp(line, "min_wpm=", 8) == 0) {
      	sscanf(line, "min_wpm=%d",
         &card.MinWPM);
         printf("found min_wpm %d", card.MinWPM);
      }
      if (strncmp(line, "min_accuracy=", 13) == 0) {
      	sscanf(line, "min_accuracy=%d",
         &card.MinAccuracy);
      }
      if (strncmp(line,  "icon_sheet=", 11) == 0) {
      	sscanf(line, "icon_sheet=%d",
         &card.IconSheet);
      }
      if (strncmp(line, "icon_index=", 11) == 0) {
      	sscanf(line, "icon_index=%d",
         &card.IconIndex);
      }
      if (strncmp(line, "description=", 12) == 0) {
        const char *src = line + 12;
        size_t len = strcspn(src, "\n"); // length up to newline
        if (len >= sizeof(card.Description)) {
          len = sizeof(card.Description) - 1; // truncate safely
        }
        memcpy(card.Description, src, len);
        card.Description[len] = '\0'; // null terminate at actual length
      }
      if (strncmp(line, "[End]", 5) == 0) {
      	fwrite(&card, sizeof(Card), 1, out_file);
      }
      if (strncmp(line, "[Start]", 7) == 0){
      	//card.Title = "";
         //card.Description = "";
         card.EffectType = 0;
         card.Effect = 0;
         card.MinWPM = 0;
         card.MinAccuracy = 0;
         card.IconSheet = 0;
         card.IconIndex = 0;
         card_count++;
      }
   }

   header.count = card_count;

   fseek(out_file, 0, SEEK_SET);
   fwrite(&header, sizeof(CardHeader), 1, out_file);


   fclose(in_file);
   fclose(out_file);

   return 0;
}

int validate(const char *packed_file){

	CardHeader header;
   Card card;
   int i;
   FILE *file;

   if ((file = fopen(packed_file, "rb")) == NULL) {
     printf("Error opening file %s.\n", packed_file);
     return 1;
   }

   fread(&header, sizeof(CardHeader), 1, file);
   printf("Card count %d\n", header.count);
   printf("Version %d\n", header.version);
	for (i = 0; i < header.count; i++){
		fread(&card, sizeof(Card), 1, file);
      printf("Title = %s\n", card.Title);
      printf("Description = %s\n", card.Description);
      printf("Min WPM = %d\n", card.MinWPM);
      printf("Effect Type = %d\n", card.EffectType);
      printf("Effect = %d\n", card.Effect);
      printf("Min Accuracy = %d\n", card.MinAccuracy);
      printf("Icon Sheet = %d\n", card.IconSheet);
      printf("Icon Index = %d\n", card.IconIndex);
      printf("======\n");
   }

   return 0;
}

/*
	This program will pack a card definition file into struct and then
   into binary for use in the game.
*/

int main(int argc, char *argv[]) {

	 const char *in_path;
    const char *out_path;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s (<input.txt>|validate) <output.crd>\n", argv[0]);
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
