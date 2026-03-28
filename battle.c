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

#define CARD_TITLE_OFFSET_X	 4
#define CARD_TITLE_OFFSET_Y 	 16
#define CARD_TITLE_HEIGHT		 18
#define CARD_TITLE_WIDTH		 116
#define CARD_DESC_OFFSET_X		 8
#define CARD_DESC_OFFSET_Y		 100
#define CARD_DESC_HEIGHT		 50
#define CARD_DESC_WIDTH        104

int sprite_x, sprite_y;
int i,x,y;
unsigned int k;
BITMAP backimg;

BITMAP cardbg;

GlyphSet activeCardText;
GlyphSet activeCardTitle;
GlyphRenderInfo curRenderChar;


int curGlyph;
int errors;
int quitEarly;
int curCard;
int card_x;
int card_y;
int enemyId;
char scoreText[255];
int activeCard;
int finishCount;
BattleStep step;

//intro state
bool intro_done;
int introTick;
GlyphSet introTextTitle;
GlyphSet introTextName;


void init_battle(){
  sprite_x = 0;
  sprite_y = 10;
  curGlyph = 0;
  errors = 0;
  curCard = 0;
  quitEarly = 0;
  enemyId = 0;
  finishCount = 0;
  activeCard = game_state.inventory[0];

  load_bmp("castle.bmp", &backimg);
  load_bmp("card.bmp", &cardbg);

  card_x = 320 - cardbg.width - 2;
  card_y = 2;

  enter_step_intro();


}

void draw_card(){
	draw_transparent_bitmap(&cardbg, card_x, card_y);
   draw_text(&pixantiqua_font, &pixantiqua_fontimg, cards[activeCard].Title, card_x + CARD_TITLE_OFFSET_X, card_y + CARD_TITLE_OFFSET_Y, CARD_TITLE_WIDTH, CARD_TITLE_HEIGHT, ALIGN_LEFT, ALIGN_MIDDLE);
   draw_text(&silk_font, &silk_fontimg, cards[activeCard].Description, card_x + CARD_DESC_OFFSET_X, card_y + CARD_DESC_OFFSET_Y, CARD_DESC_WIDTH, CARD_DESC_HEIGHT, ALIGN_LEFT, ALIGN_TOP);
}

void destroy_battle(){
  farfree(cardbg.data);
  farfree(backimg.data);
}

void update_battle(){
	if (quitEarly){
   	ChangeScreen(Menu);
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
      case Score:
      	update_step_score();
      break;
   }
}

void render_battle(){

	draw_bitmap(&backimg, 0, 0);

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
      case Score:
      	keypress_step_score(key);
      break;
   }


}



void enter_step_intro(){
	step = Intro;
	intro_done = 0;
   introTick = 0;
	step = Intro;

   //TODO: get enemy name

   get_text_glyphs(&pixantiqua_font, &introTextTitle, "New Enemy", 0, 100, 320, 20, ALIGN_CENTER, ALIGN_MIDDLE);
   get_text_glyphs(&pixantiqua_font, &introTextName, "FIGHT START", 0, 120, 320, 20, ALIGN_CENTER, ALIGN_MIDDLE);


}

void render_step_intro(){
	if (introTick > 100){
		render_text_glyphs(&pixantiqua_font, &pixantiqua_fontimg, &introTextTitle, 0);
   }
   if (introTick > 500){
		render_text_glyphs(&pixantiqua_font, &pixantiqua_fontimg, &introTextName, 0);
   }
}

void update_step_intro(){
	introTick++;
   if (introTick > 1000){
    	intro_done = true;
   }
}

void keypress_step_intro(KeyEvent event){
	if (intro_done){
    	enter_step_select();
   }
}

void enter_step_select(){
 	step = Select;
}


void draw_player_info(){
	draw_text(&silk_font, &silk_fontimg, game_state.player_name, 0, 0, 320, 200, ALIGN_CENTER, ALIGN_TOP);
}

void render_step_select(){
	draw_player_info();
   draw_card();
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



void enter_step_play(){
	step = Play;
   get_text_glyphs(&pixantiqua_font, &activeCardText, &(cards[activeCard].Description), 10, 10, 300, 140, ALIGN_RIGHT, ALIGN_MIDDLE);
}

void render_step_play(){
	draw_player_info();
	render_text_glyphs(&pixantiqua_font, &pixantiqua_fontimg, &activeCardText, 0);
}

void update_step_play(){
	//TODO: count/calculate WPM and timeout
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
   		enter_step_score();
   		return;
   	}
   	activeCardText.glyphs[curGlyph].style = Next;
   }
}

void enter_step_score(){
	step = Score;
	sprintf(scoreText, "You made %d errors", errors);
}

void render_step_score(){
	draw_text(&silk_font, &silk_fontimg, scoreText, 0, 0, 320, 200, ALIGN_CENTER, ALIGN_MIDDLE);
}

void update_step_score(){

}

void keypress_step_score(KeyEvent event){
	change_screen(Menu); //TODO: Return to overworld or some other screen.
}
