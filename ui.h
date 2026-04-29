#include "bitmap.h"
#ifndef UI_H
#define UI_H

typedef struct {
  unsigned short x;
  unsigned short y;
  unsigned short width;
  unsigned short height;
  int canExpandX;
  int canExpandY;
} UISprite;

//named assets
#define UI_METAL_BOX 0
#define UI_SCROLL_TITLE 1

void load_ui_assets();
void unload_ui_assets();

void draw_ui_sprite(int spriteIndex, int targetX, int targetY, int targetWidth, int targetHeight);
void draw_ui_sprite_img(BITMAP * sprite, int targetX, int targetY, int targetWidth, int targetHeight);

void backdrop_init(const char * path);
void backdrop_draw();
void backdrop_draw_part(int x, int y, int width, int height);
void backdrop_free();

#endif