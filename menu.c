#include "draw.h"
#include "keyboard.h"
#include "menu.h"
#include "bmpfnt.h"
#include "fonts.h"
#include "stdio.h"
#include "sound.h"
#include "ui.h"
#include "vga.h"


#define MENU_ITEM_COUNT 3

int currentItem;
GlyphSet menuGlyphs[MENU_ITEM_COUNT];
char menuItems[MENU_ITEM_COUNT][20] = {"New Game", "Continue", "Exit to DOS"};
int redraw_menu = 0;
int init_redraw_menu = 0;
int menu_width = 0;
int menu_height = 0;
int menu_x = 0;
int menu_y = 0;
BITMAP menu_backimg;
BITMAP menu_title;

void draw_menu_bg(){
	draw_bitmap(&menu_backimg, 0, 0);
}

void draw_menu_title(){
	draw_transparent_bitmap(&menu_title, 0, 0);
}

void init_menu(){
	int i, lineHeight, yoffset;

	play_menu_song();
	load_bmp("comasset/plains.bmp", &menu_backimg);
	load_bmp("devasset/title.bmp", &menu_title);
	currentItem = 0;
	yoffset = 110;
	lineHeight = (200 - yoffset) / MENU_ITEM_COUNT;

	for (i = 0; i < MENU_ITEM_COUNT; i++){
		get_text_glyphs(&venice_font, &menuGlyphs[i],
			menuItems[i],
			0, (i * lineHeight) + yoffset, SCREEN_WIDTH, lineHeight, ALIGN_CENTER, ALIGN_MIDDLE);
	}
	menu_y = yoffset;
	menu_height = lineHeight * MENU_ITEM_COUNT;
	menu_x = SCREEN_WIDTH * 0.3;
	menu_width = SCREEN_WIDTH - (menu_x * 2);
	redraw_menu = 1;
	init_redraw_menu = 1;
}

void update_menu(){
	int i, j;
   FontStyle style;
   for (i = 0; i < MENU_ITEM_COUNT; i++){
   	style = Normal;
   	if (currentItem == i){
      	style = Correct;
      }
      for (j = 0; j < menuGlyphs[i].glyphCount; j++){
      	menuGlyphs[i].glyphs[j].style = style;
      }
   }
}

void draw_menu_box(){
	//draw_rect(160, 100, 320, 200, 14);
	draw_ui_sprite(UI_METAL_BOX, menu_x, menu_y, menu_width, menu_height);
}

void render_menu(){
	int i;
	if (init_redraw_menu){
		draw_menu_bg();
		draw_menu_title();
		draw_menu_box();
		init_redraw_menu = 0;
	}
	if (!redraw_menu){
		return;
	}
	for (i = 0; i < MENU_ITEM_COUNT; i++){
		render_text_glyphs(&venice_font, &venice_fontimg, &menuGlyphs[i], 0);
	}
	show_offscreen_buffer();
}

void destroy_menu(){
	farfree(menu_backimg.data);
	farfree(menu_title.data);
}

void keypress_menu(KeyEvent event){
	if (event.isAscii){
      if (event.code == KEY_ENTER){
   	switch (currentItem){
   		case 0:
         	change_screen(Battle);
      		break;
         case 1:
         	//do nothing - tbd
            change_screen(Battle);
            break;
         case 2:
         	exit_program();
         	break;
      }
   	}

   	return;


   }
	if (event.code == KEY_UP){
   	if (currentItem == 0){
       	return;
      }
			redraw_menu = 1;
      currentItem--;
   }
   if (event.code == KEY_DOWN){
   	if (currentItem == MENU_ITEM_COUNT - 1){
      	return;
      }
			redraw_menu = 1;
      currentItem++;
   }

}
