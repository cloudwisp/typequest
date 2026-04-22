#include "typedef.h"
#ifndef DRAW_H
#define DRAW_H

#define COLOR_BLACK 44
 
void draw_fill(byte color);
void draw_pixel(int x, int y, unsigned char color);
void draw_rect(int x, int y, int width, int height, byte color);

#endif
