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

  enter_step_intro();


}

void draw_action_bar(){
   int bar_x, bar_y, offset_x, offset_y, gap;
   bar_x = SCREEN_WIDTH - 10 - action_bar.width;
   bar_y = SCREEN_HEIGHT - action_bar.height;
   offset_x = 12; //spell pos from left
   offset_y = 5; //spell pos from top
   gap = 6;

   draw_transparent_bitmap(&action_bar, bar_x, bar_y);
   draw_bitmap(&action_sword, bar_x + offset_x, bar_y + offset_y);
   draw_bitmap(&action_fire, bar_x + offset_x + 16 + gap, bar_y + offset_y);
   draw_bitmap(&action_heal, bar_x + offset_x + ((16 + gap) * 2), bar_y + offset_y);
   draw_transparent_bitmap(&action_highlight, bar_x + 8 + ((16 + gap) * spellType), bar_y + 1);
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
}


void draw_player_info(){
	draw_text(&silk_font, &silk_fontimg, game_state.player_name, 0, 0, 320, 200, ALIGN_CENTER, ALIGN_TOP);
}

void render_step_select(){
   if (initRender){
      backdrop_draw();
      draw_player_info();
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
         // numeric 1
         spellType = 0; //TODO: add enum or def for these types
         redrawFlag = 1;
      } else if (key.code == 0x32){
         // numeric 2
         spellType = 1;
         redrawFlag = 1;
      } else if (key.code == 0x33){
         // numeric 3
         spellType = 2;
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
   get_text_glyphs(&pixantiqua_font, &activeCardText, &(cards[activeCard].Description), 10, 10, 300, 140, ALIGN_RIGHT, ALIGN_MIDDLE);
}

void render_step_play(){
   if (initRender){
      backdrop_draw();
      draw_player_info();
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
      redrawFlag = 1;
   }
}

void enter_step_score(){
	step = Score;
   stepTicks = 0;
   initRender = 1;
	sprintf(scoreText, "You made %d errors", errors);
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
