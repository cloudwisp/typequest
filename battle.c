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


int sprite_x, sprite_y;
int i,x,y;
unsigned int k;
BITMAP backimg;

BITMAP cardbg;

GlyphSet johnny;
GlyphRenderInfo curRenderChar;


int curGlyph;
int errors;
int quitEarly;
int curCard;
int battleEnd;
int battleStart;
int card_x;
int card_y;
char scoreText[255];

void draw_player_info(){
	draw_text(&silk_font, &silk_fontimg, game_state.player_name, 0, 0, 320, 200, ALIGN_CENTER, ALIGN_TOP);
}

void init_battle(){
  sprite_x = 0;
  sprite_y = 10;
  curGlyph = 0;
  errors = 0;
  curCard = 0;
  battleEnd = 0;
  battleStart = 0;
  quitEarly = 0;

  load_bmp("castle.bmp", &backimg);
  load_bmp("card.bmp", &cardbg);

  card_x = 320 - cardbg.width - 2;
  card_y = 2;
  
  get_text_glyphs(&pixantiqua_font, &johnny,
	 "There was Johnny McEldoo and Mcgee and me and a couple two or three went on a spree one day",
	  10, 10, 300, 140, ALIGN_RIGHT, ALIGN_MIDDLE);


}

void destroy_battle(){
  farfree(cardbg.data);
  farfree(backimg.data);
}

void update_battle(){
	if (battleEnd && !quitEarly){
   	
		sprintf(scoreText, "You made %d errors", errors);

   }
   else if (quitEarly)
   {
   	exit_program();
   }
   else
   {
   	//normal update here. Animations etc.
      return;
   }
}

void render_battle(){
	if (!battleStart){
    	//any battle start animations, or screens
      battleStart = 1;
   	return;
   }
	if (!battleEnd){
   	//main draw
   	draw_bitmap(&backimg, 0, 0);

      draw_transparent_bitmap(&cardbg, card_x, card_y);

   	draw_player_info();

		render_text_glyphs(&pixantiqua_font, &pixantiqua_fontimg, &johnny, 0);

		show_offscreen_buffer();
      return;
   }
   if (battleEnd && !quitEarly){
              
		draw_fill(20);

		draw_text(&silk_font, &silk_fontimg, scoreText, 0, 0, 320, 200, ALIGN_CENTER, ALIGN_MIDDLE);
      
		show_offscreen_buffer();
      getch(); //pause program
      exit_program();
   } 

}
void keypress_battle(KeyEvent key){
	if (key.isAscii){
      if (key.code == KEY_ESC){
         quitEarly = 1;
         battleEnd = 1;
         return;

      }
      if (key.code == johnny.glyphs[curGlyph].charId){
         johnny.glyphs[curGlyph].style = Correct;
      } else {
         johnny.glyphs[curGlyph].style = Incorrect;
         errors++;
      }
      curGlyph++;
      if (curGlyph >= johnny.glyphCount){
       battleEnd = 1;
       return;
      }
      johnny.glyphs[curGlyph].style = Next;
   }
}
