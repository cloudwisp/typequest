#include <alloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include "cards.h"

CardHeader card_header;
Card far * cards;

void load_cards(){
   int i;
   FILE *file;
   Card * temp;

   if ((file = fopen("cards.db", "rb")) == NULL) {
     printf("Error opening file %s.\n", "cards.db");
     return 1;
   }

   fread(&card_header, sizeof(CardHeader), 1, file);
   cards = (Card far *)farmalloc(sizeof(Card) * card_header.Count);
	if (cards == NULL){
		printf("far malloc failed");
	}
	fread(cards, sizeof(Card), card_header.Count, file);

}

void unload_cards(){
	farfree(cards);
}