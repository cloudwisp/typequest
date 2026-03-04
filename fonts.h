#ifndef FONTS_H
#define FONTS_H

extern BITMAP venice_fontimg;
extern BitmapFont venice_font;
extern BITMAP silk_fontimg;
extern BitmapFont silk_font;
extern BITMAP pixantiqua_fontimg;
extern BitmapFont pixantiqua_font;

void load_global_fonts();
void unload_global_fonts();

#endif