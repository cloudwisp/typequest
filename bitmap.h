#include "globals.h"

#ifndef BITMAP_H
#define BITMAP_H

typedef struct tagBITMAP              /* the structure for a bitmap. */
{
  word width;
  word height;
  far byte *data;
} BITMAP;

void load_bmp(const char *file, BITMAP *b);
void draw_bitmap_sprite(BITMAP *spritesheet, int sourceX, int sourceY, int width, int height, int drawX, int drawY, bool transparent, int maskColor);
void draw_bitmap(BITMAP *bmp, int x,int y);
void draw_transparent_bitmap(BITMAP *bmp, int x,int y);


#endif
