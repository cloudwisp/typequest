#include "shared\card.h"

#ifndef cards_c
#define cards_c

extern CardHeader card_header;
extern Card far * cards;

void load_cards();
void unload_cards();

#endif