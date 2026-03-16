#include <string.h>
#include "state.h"

void set_initial_state(GAME_STATE *state){
   int i;
   state->difficulty = 0;
   strcpy(state->player_name, "Player 1");
   state->stats.level = 1;
   state->stats.health = 40;
   state->stats.mana = 40;
	state->stats.endurance = 40;
   state->stats.shield = 40;
   state->stats.damage = 40;
   state->stats.dexterity = 70;
   state->stats.intelligence = 70;
   state->stats.charisma = 70;

   state->enemyid = 0;
   

   state->screen = Battle;

   state->inventoryCount = 2;
   for (i = 0; i < MAX_INVENTORY; i++){
   	state->inventory[i] = 0;
   }
   state->inventory[0] = 0; //basic attack
   state->inventory[1] = 1; //heal

}

