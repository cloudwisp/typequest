#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <alloc.h>
#include <dos.h>
#include <mem.h>
#include "globals.h"
#include "draw.h"
#include "vga.h"



void draw_fill(byte color) {
  _fmemset(offscreen_buffer, color, SCREEN_BUFFER_SIZE);
}


void draw_pixel(int x, int y, unsigned char color) {
  offscreen_buffer[((y<<8) + (y<<6)) + x] = color;
}

void draw_rect(int x, int y, int width, int height, byte color) {
  int i;
  int offset;
  if (x + width > SCREEN_WIDTH){
    width = SCREEN_WIDTH - x;
  }
  if (y + height > SCREEN_HEIGHT){
    height = SCREEN_HEIGHT - y;
  }
  offset = (y * SCREEN_WIDTH) + x;
  for (i = 0; i < height; i++){
    _fmemset(offscreen_buffer + offset, color, width);
    offset += SCREEN_WIDTH;
  }
}