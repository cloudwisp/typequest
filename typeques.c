#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <alloc.h>
#include <dos.h>
#include <mem.h>
#include "globals.h"
#include "vga.h"
#include "keyboard.h"
#include "menu.h"
#include "battle.h"
#include "enemies.h"
#include "timer.h"
#include "sound.h"
#include "draw.h"
#include "bmpfnt.h"
#include "fonts.h"
#include "ui.h"
#include "state.h"
#include "book.h"

KeyEvent key;
int forceExit;
unsigned long lastTick;
unsigned long lastRenderTick;

void update_screen(){
	switch (game_state.screen){
    	case Menu:
      	update_menu();
         break;
      case Battle:
      	update_battle();
         break;
   }
}

void render_screen(){
	switch (game_state.screen){
    	case Menu:
      	render_menu();
         break;
      case Battle:
      	render_battle();
         break;
   }
}

void show_loading(){
   draw_fill(COLOR_BLACK);
   draw_text(&venice_font, &venice_fontimg, "Loading...", 0, 0, 320, 200, ALIGN_CENTER, ALIGN_MIDDLE);
	show_offscreen_buffer();
}

void change_screen(GameScreen screen){
   show_loading();
	switch (game_state.screen){
    	case Menu:
      	destroy_menu();
         break;
      case Battle:
      	destroy_battle();
         break;
   }
   game_state.screen = screen;
   switch (game_state.screen){
   	case Menu:
      	init_menu();
         break;
      case Battle:
      	init_battle();
         break;
   }
}

void on_keypress(KeyEvent event){
	switch (game_state.screen){
   	case Menu:
      	keypress_menu(event);
         break;
      case Battle:
      	keypress_battle(event);
         break;
      default:
      	exit_program();
   }
}

void exit_program(){
	forceExit = 1;
}

void main(void) {

  forceExit = 0;
  lastTick = 0;
  lastRenderTick = 0;

  init_timer();

  opl_init();
  if (opl2){
   printf("OPL 2 detected");
   getch();
  } else {
   printf("No OPL 2 detected");
   getch();
  }

  set_initial_state(&game_state);

  load_global_fonts();
  load_ui_assets();
  load_books();
  load_enemies();
  load_cards();

  printf(get_random_sentence(1));
  getch();

  set_video_mode(MODE13H);

  if(!create_offscreen_buffer()) {
	 set_video_mode(TEXT_MODE);
	 printf("Not enough memory to create offscreen buffer.\n");
	 printf("Exiting to DOS...\n");
	 exit(1);
  }

  set_shared_palette();

  change_screen(Menu);

  
  while (1){
  		if (forceExit){
  			break;
      }
		if (kbhit()){
			key = get_key_event();
         on_keypress(key);
		}

      if (ticks60 != lastTick) {
        lastTick = ticks60;
        update_screen();
      }
      if (ticks60 > lastRenderTick){
         render_screen();
         lastRenderTick = ticks60;
      }

  }

  remove_timer();
  
  if (opl2){
   opl_cleanup();
  }
  

  delete_offscreen_buffer();

  unload_ui_assets();
  unload_global_fonts();
  unload_enemies();
  unload_cards();
  unload_books();

  set_video_mode(TEXT_MODE);
}




