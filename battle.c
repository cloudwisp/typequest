#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <alloc.h>
#include <dos.h>
#include <mem.h>
#include "globals.h"
#include "vga.h"
#include "draw.h"
#include "bitmap.h"
#include "bmpfnt.h"
#include "keyboard.h"
#include "battle.h"
#include "state.h"
#include "fonts.h"
#include "cards.h"
#include "sound.h"
#include "enemies.h"
#include "ui.h"
#include "book.h"

int sprite_x, sprite_y;
int i,x,y;
unsigned int k;

BITMAP action_bar;
BITMAP action_highlight;
BITMAP action_sword;
BITMAP action_heal;
BITMAP action_fire;

BITMAP cardbg;

GlyphSet activeCardText;
GlyphSet activeCardTitle;
GlyphRenderInfo curRenderChar;


int curGlyph;
int errors;
int quitEarly;
int curCard;
int enemyId;
char scoreText[255];
int activeCard;
int finishCount;
int redrawFlag;
int initRender;
int spellType;
int lastRenderedGlyph;
double enemy_chars_per_sec;
double enemy_error_chance;
unsigned long enemyTypeTick;
BattleStep step;

//intro state
bool intro_done;
unsigned long stepTicks;
GlyphSet introTextTitle;
GlyphSet introTextName;
Enemy currentEnemy;


void init_battle(){
  sprite_x = 0;
  sprite_y = 10;
  curGlyph = 0;
  errors = 0;
  curCard = 0;
  quitEarly = 0;
  enemyId = 0; //TODO: randomize or pick from roster.
  finishCount = 0;
  redrawFlag = 0; //can be used by steps to invalidate screen
  initRender = 0;
  spellType = 0;
  activeCard = game_state.inventory[0];
  play_battle_song();

  backdrop_init("comasset/forest.bmp");
  load_bmp("card.bmp", &cardbg);
  load_bmp("comasset/action.bmp", &action_bar);
  load_bmp("comasset/actbdr.bmp", &action_highlight);
  load_bmp("comasset/sword.bmp", &action_sword);
  load_bmp("comasset/heal.bmp", &action_heal);
  load_bmp("comasset/fire.bmp", &action_fire);

  get_enemy(enemyId, &currentEnemy);
  game_state.enemyid = enemyId;
  game_state.enemystats.health = currentEnemy.Health; //TODO: populate other enemy stats
  game_state.enemystats.dexterity = 70;
  game_state.enemystats.maxhealth = currentEnemy.Health;

  //reset player health
  game_state.stats.health = game_state.stats.maxhealth;

  enter_step_intro();


}

void draw_action_bar(){
   int bar_x, bar_y, offset_x, offset_y, gap, spellOffset;
   bar_x = SCREEN_WIDTH - 10 - action_bar.width;
   bar_y = SCREEN_HEIGHT - action_bar.height;
   offset_x = 12; //spell pos from left
   offset_y = 5; //spell pos from top
   gap = 6;

   spellOffset = 0;
   switch (spellType){
      case SPELL_ATTACK:
      spellOffset = 0;
      break;
      case SPELL_ABILITY:
      spellOffset = 1;
      break;
      case SPELL_HEAL:
      spellOffset = 2;
      break;
   }

   draw_transparent_bitmap(&action_bar, bar_x, bar_y);
   draw_bitmap(&action_sword, bar_x + offset_x, bar_y + offset_y);
   draw_bitmap(&action_fire, bar_x + offset_x + 16 + gap, bar_y + offset_y);
   draw_bitmap(&action_heal, bar_x + offset_x + ((16 + gap) * 2), bar_y + offset_y);
   draw_transparent_bitmap(&action_highlight, bar_x + 8 + ((16 + gap) * spellOffset), bar_y + 1);
}

void draw_card(){
   int card_x, card_y, card_width, card_height;
   card_width = 140;
   card_height = 120;

   card_x = SCREEN_WIDTH - 10 - card_width;
   card_y = SCREEN_HEIGHT - 40 - card_height;

   //draw_ui_sprite(UI_METAL_BOX, card_x, card_y, card_width, card_height);
   draw_ui_sprite(UI_SCROLL_TITLE, card_x + 4, card_y + 4, card_width - 8, card_height);
	//draw_transparent_bitmap(&cardbg, card_x, card_y);
   draw_text(&pixantiqua_font, &pixantiqua_fontimg, cards[activeCard].Title, card_x + 24, card_y + 4, card_width - 48, 40, ALIGN_LEFT, ALIGN_MIDDLE);
   draw_text(&silk_font, &silk_fontimg, cards[activeCard].Description, card_x + 24, card_y + 48, card_width - 48, 60, ALIGN_LEFT, ALIGN_TOP);
}

void destroy_battle(){
   farfree(cardbg.data);
   backdrop_free();
   farfree(action_bar.data);
   farfree(action_highlight.data);
   farfree(action_sword.data);
   farfree(action_heal.data);
   farfree(action_fire.data);

}

void update_battle(){
   stepTicks++;
	if (quitEarly){
   	change_screen(Menu);
      return;
   }
	switch (step){
    	case Intro:
      	update_step_intro();
      break;
      case Select:
      	update_step_select();
      break;
      case Play:
      	update_step_play();
      break;
      case EnemyPlay:
         update_step_enemyplay();
      break;
      case Score:
      	update_step_score();
      break;
   }
}

void render_battle(){


	switch (step){
    	case Intro:
      	render_step_intro();
      break;
      case Select:
      	render_step_select();
      break;
      case Play:
      	render_step_play();
      break;
      case EnemyPlay:
         render_step_enemyplay();
      break;
      case Score:
      	render_step_score();
      break;
   }
  	show_offscreen_buffer();

}

void keypress_battle_select(KeyEvent key){
	if (key.isAscii){
      if (key.code == KEY_ESC){
         quitEarly = 1;
         return;

      } else if (key.code == KEY_ENTER){
      	enter_step_play();
      }
   } else if (key.code == KEY_RIGHT){
   	if (activeCard + 1 >= game_state.inventoryCount){
      	activeCard = 0;
      	return; //wrap to first
      }
      activeCard++;
      return;
   } else if (key.code == KEY_LEFT){
    	if (activeCard == 0){
       	activeCard = game_state.inventoryCount - 1;
         return;
      }
      activeCard--;
   }
}

void keypress_battle(KeyEvent key){
	if (key.code == KEY_ESC){
   	change_screen(Menu);
      return;
   }
   switch (step){
    	case Intro:
      	keypress_step_intro(key);
      break;
      case Select:
      	keypress_step_select(key);
      break;
      case Play:
      	keypress_step_play(key);
      break;
      case EnemyPlay:
         keypress_step_enemyplay(key);
         break;
      case Score:
      	keypress_step_score(key);
      break;
   }


}

void enter_step_intro(){
	step = Intro;
	intro_done = 0;
   stepTicks = 0;
   initRender = 1;

   get_text_glyphs(&pixantiqua_font, &introTextTitle, currentEnemy.Name, 0, 100, 320, 20, ALIGN_CENTER, ALIGN_MIDDLE);
   get_text_glyphs(&pixantiqua_font, &introTextName, "FIGHT START", 0, 120, 320, 20, ALIGN_CENTER, ALIGN_MIDDLE);


}

void render_step_intro(){
   if (initRender){
      backdrop_draw();
      initRender = 0;
   }
   //render in reverse order
   if (stepTicks >= 240 && redrawFlag){
      render_text_glyphs(&pixantiqua_font, &pixantiqua_fontimg, &introTextName, 0);
      redrawFlag = 0;
      return;
   }
	if (stepTicks >= 180 && redrawFlag){
		render_text_glyphs(&pixantiqua_font, &pixantiqua_fontimg, &introTextTitle, 0);
      redrawFlag = 0;
   }
}

void update_step_intro(){
   if (stepTicks == 180 || stepTicks == 240){
      redrawFlag = 1;
   }
   if (stepTicks > 360){
    	enter_step_select();
   }
}

void keypress_step_intro(KeyEvent event){
	// if (intro_done){
   //  	enter_step_select();
   // }
}

void enter_step_select(){
 	step = Select;
   stepTicks = 0;
   redrawFlag = 1;
   initRender = 1;
   activeCard = game_state.inventory[0];
}


void draw_player_info(){
   byte buffer[20];
   backdrop_draw_part(0, 0, 50, 24);
	draw_text(&silk_font, &silk_fontimg, game_state.player_name, 0, 0, 320, 12, ALIGN_LEFT, ALIGN_TOP);
   sprintf(&buffer, "Health: %d", game_state.stats.health);
   draw_text(&silk_font, &silk_fontimg, buffer, 0, 12, 320, 12, ALIGN_LEFT, ALIGN_TOP);
}

void draw_enemy_info(){
   byte buffer[20];
   backdrop_draw_part(320 - 50, 0, 50, 24);
	draw_text(&silk_font, &silk_fontimg, currentEnemy.Name, 0, 0, 320, 12, ALIGN_RIGHT, ALIGN_TOP);
   sprintf(&buffer, "Health: %d", game_state.enemystats.health );
   draw_text(&silk_font, &silk_fontimg, buffer, 0, 12, 320, 12, ALIGN_RIGHT, ALIGN_TOP);
}

void render_step_select(){
   if (initRender){
      backdrop_draw();
      draw_player_info();
      draw_enemy_info();
      initRender = 0;
   }
   if (redrawFlag){
      draw_card();
      draw_action_bar();
      redrawFlag = 0;
   }
}

void update_step_select(){
   
}

void keypress_step_select(KeyEvent key){
	if (key.isAscii){
      if (key.code == KEY_ESC){
         quitEarly = 1;
         return;

      } else if (key.code == KEY_ENTER){
      	enter_step_play();
      } else if (key.code == 49){
         //TODO: Switch out the inventory items to scroll through based on spell type
         // numeric 1
         spellType = SPELL_ATTACK;
         redrawFlag = 1;
      } else if (key.code == 0x32){
         // numeric 2
         spellType = SPELL_ABILITY;
         redrawFlag = 1;
      } else if (key.code == 0x33){
         // numeric 3
         spellType = SPELL_HEAL;
         redrawFlag = 1;
      }
   } else if (key.code == KEY_RIGHT){
   	if (activeCard + 1 >= game_state.inventoryCount){
      	activeCard = 0;
         redrawFlag = 1;
      	return; //wrap to first
      }
      activeCard++;
      redrawFlag = 1;
      return;
   } else if (key.code == KEY_LEFT){
    	if (activeCard == 0){
       	activeCard = game_state.inventoryCount - 1;
         redrawFlag = 1;
         return;
      }
      activeCard--;
      redrawFlag = 1;
   }
}



void enter_step_play(){
	step = Play;
   stepTicks = 0;
   initRender = 1;
   redrawFlag = 1;
   curGlyph = 0;
   lastRenderedGlyph = 0;

   get_text_glyphs(&pixantiqua_font, &activeCardText, get_random_sentence(cards[activeCard].EffectType), 10, 10, 300, 140, ALIGN_RIGHT, ALIGN_MIDDLE);
}

void render_step_play(){
   if (initRender){
      backdrop_draw();
      draw_player_info();
      draw_enemy_info();
      initRender = 0;
   }
	//currently the text can overlay previously rendered text. Use redraw flag if we're adding animations or anything.
   if (redrawFlag){
      render_text_glyphs(&pixantiqua_font, &pixantiqua_fontimg, &activeCardText, 0);
      redrawFlag = 0;
   }
	
}

void update_step_play(){
	//TODO: count/calculate WPM and timeout
}

void on_play_end(){
   int success = 1; //TODO: compute accuracy and WPM and compare it against the card stats
   int newEnemyHealth;
   int newPlayerHealth;
   if (!success){
      enter_step_enemyplay();
   }
   newEnemyHealth = game_state.enemystats.health;
   newPlayerHealth = game_state.stats.health;
   
   switch (cards[activeCard].EffectType){
      case SPELL_ATTACK:
         newEnemyHealth = game_state.enemystats.health - cards[activeCard].Effect;
      case SPELL_HEAL:
         newPlayerHealth = game_state.stats.health + cards[activeCard].Effect;
   }
   if (newEnemyHealth <= 0){
      game_state.enemystats.health = 0;
      //enter_step_score();
      enter_step_select();
      return;
   } else {
      game_state.enemystats.health = newEnemyHealth;
   }
   if (newPlayerHealth >= game_state.stats.maxhealth){
      game_state.stats.health = game_state.stats.maxhealth;
   } else {
      game_state.stats.health = newPlayerHealth;
   }
   enter_step_enemyplay();
}

void on_enemy_play_end(){
   int success = 1; //TODO: compute accuracy and WPM and compare it against the card stats
   int newEnemyHealth;
   int newPlayerHealth;
   if (!success){
      enter_step_select();
   }
   newEnemyHealth = game_state.enemystats.health;
   newPlayerHealth = game_state.stats.health;
   
   switch (cards[activeCard].EffectType){
      case SPELL_ATTACK:
         newPlayerHealth = game_state.stats.health - cards[activeCard].Effect;
      case SPELL_HEAL:
         newEnemyHealth = game_state.enemystats.health + cards[activeCard].Effect;
   }
   if (newPlayerHealth <= 0){
      game_state.stats.health = 0;
      enter_step_select();
      //enter_step_score();
      return;
   } else {
      game_state.stats.health = newPlayerHealth;
   }
   if (newEnemyHealth >= game_state.enemystats.maxhealth){
      game_state.enemystats.health = game_state.enemystats.maxhealth;
   } else {
      game_state.enemystats.health = newEnemyHealth;
   }
   enter_step_select();
}

void keypress_step_play(KeyEvent key){
	if (key.isAscii){
   	if (key.code == activeCardText.glyphs[curGlyph].charId){
   		activeCardText.glyphs[curGlyph].style = Correct;
   	} else {
   		activeCardText.glyphs[curGlyph].style = Incorrect;
      	errors++;
   	}
   	curGlyph++;
   	if (curGlyph >= activeCardText.glyphCount){
   		on_play_end();
   		return;
   	}
   	activeCardText.glyphs[curGlyph].style = Next;
      redrawFlag = 1;
   }
}

void enter_step_score(){
	step = Score;
   stepTicks = 0;
   initRender = 1;
   if (game_state.stats.health <= 0){
      sprintf(scoreText, "You have been defeated");   
   } else {
      sprintf(scoreText, "You have defeated your enemy");
   }
}

void render_step_score(){
   if (initRender){
      backdrop_draw();
      draw_text(&silk_font, &silk_fontimg, scoreText, 0, 0, 320, 200, ALIGN_CENTER, ALIGN_MIDDLE);
   }
}

void update_step_score(){

}

void keypress_step_score(KeyEvent event){
	change_screen(Menu); //TODO: Return to overworld or some other screen.
}

int get_random_card(int spellType){
   //TODO: The enemy should have its own inventory with limited choice of spells
   //TODO: for real random instead of just first heal
   int i;
   for (i = 0; i < card_header.Count; i++){
      if (cards[i].Effect != spellType){
         continue;
      }
      return i;
   }
   return 0;
}

void enter_step_enemyplay(){
   step = EnemyPlay;
   stepTicks = 0;
   enemyTypeTick = 0;
   initRender = 1;
   redrawFlag = 1;
   curGlyph = 0;
   lastRenderedGlyph = 0;

   //here the enemy should decide what action to take.
   enemy_chars_per_sec = 20 * (game_state.enemystats.dexterity / (double)100); //dexterity is % of peak speed. E.g. 70 is 70% of 6 chars per second
   enemy_error_chance = game_state.enemystats.endurance / (double)100; //endurance is chance of getting an error for any letter
   
   if (game_state.enemystats.health < (game_state.enemystats.maxhealth * 0.2)){
      //heal below 20%
      activeCard = get_random_card(SPELL_HEAL);
   } else {
      activeCard = get_random_card(SPELL_ATTACK);
   }
   
   get_text_glyphs(&pixantiqua_font, &activeCardText, get_random_sentence(cards[activeCard].EffectType), 10, 10, 150, 180, ALIGN_RIGHT, ALIGN_MIDDLE);

   
}

void render_step_enemyplay(){
   int renderGlyph;
   if (initRender){
      backdrop_draw();
      draw_player_info();
      draw_enemy_info();
      draw_card();
      render_text_glyphs(&pixantiqua_font, &pixantiqua_fontimg, &activeCardText, 0);
      initRender = 0;
   }

   //currently the text can overlay previously rendered text. Use redraw flag if we're adding animations or anything.
   if (redrawFlag){
      for (renderGlyph = lastRenderedGlyph; renderGlyph <= curGlyph; renderGlyph++){
         render_text_glyph(&pixantiqua_font, &pixantiqua_fontimg, &activeCardText, 0, renderGlyph);
      }
      lastRenderedGlyph = curGlyph;
      redrawFlag = 0;
   }

}
 
void update_step_enemyplay(){ 
   if (enemyTypeTick >= 1){
      redrawFlag = 1;
      enemyTypeTick = 0;
      //TODO: error chance for now just make it correct always
      activeCardText.glyphs[curGlyph].style = Correct;
      curGlyph++;
      if (curGlyph >= activeCardText.glyphCount){
         on_enemy_play_end();
         return;
      }
      
   }
   enemyTypeTick++;
}

void keypress_step_enemyplay(KeyEvent event){

}
