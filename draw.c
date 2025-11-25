#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <alloc.h>
#include <dos.h>
#include <mem.h>
#include "globals.h"
#include "draw.h"


void draw_fill(byte color) {
  _fmemset(offscreen_buffer, color, 64000);
}


void draw_pixel(int x, int y, unsigned char color) {
  offscreen_buffer[((y<<8) + (y<<6)) + x] = color;
}

void draw_sprite(int x, int y) {
  int i, j;
  for (i=0; i<=10; i++) {
    for (j=0; j<=10; j++) {
      draw_pixel(i+x, j+y, 12);
    }
  }
}
