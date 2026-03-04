#include "draw.h"
#include "keyboard.h"
#include "menu.h"
#include "bmpfnt.h"
#include "fonts.h"
#include "stdio.h"


#define MENU_ITEM_COUNT 3

int currentItem;
GlyphSet menuGlyphs[MENU_ITEM_COUNT];
char menuItems[MENU_ITEM_COUNT][20] = {"New Game", "Continue", "Exit to DOS"};

void init_menu(){
	int i, lineHeight;

	currentItem = 0;
   lineHeight = 200 / MENU_ITEM_COUNT;

 
   for (i = 0; i < MENU_ITEM_COUNT; i++){
   	  get_text_glyphs(&venice_font, &menuGlyphs[i],
   	menuItems[i],
	  0, i * lineHeight, 320, lineHeight, ALIGN_CENTER, ALIGN_MIDDLE);
   }

   getch();
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

void render_menu(){
	int i;
   for (i = 0; i < MENU_ITEM_COUNT; i++){
		render_text_glyphs(&venice_font, &venice_fontimg, &menuGlyphs[i], 0);
   }
	show_offscreen_buffer();
}

void destroy_menu(){


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
      currentItem--;
   }
   if (event.code == KEY_DOWN){
   	if (currentItem == MENU_ITEM_COUNT - 1){
      	return;
      }
      currentItem++;
   }

}
