#include "bmpfnt.h"
#include "bitmap.h"
#include "fonts.h"


BITMAP venice_fontimg;
BitmapFont venice_font;
BITMAP silk_fontimg;
BitmapFont silk_font;
BITMAP pixantiqua_fontimg;
BitmapFont pixantiqua_font;

void load_global_fonts(){
	load_bmp("venice2.bmp", &venice_fontimg);
  parseFntFile("venice.fnt", &venice_font);

  load_bmp("font/silkscr/silk8.bmp", &silk_fontimg);
  parseFntFile("font/silkscr/silkscr8.fnt", &silk_font);

  load_bmp("font/pixantiq/pixantiq.bmp", &pixantiqua_fontimg);
  parseFntFile("font/pixantiq/pixantiq.fnt", &pixantiqua_font);
}

void unload_global_fonts(){
  farfree(venice_fontimg.data);
  farfree(silk_fontimg.data);
  farfree(pixantiqua_fontimg.data);
}