#include "typedef.h"
#include "state.h"
#ifndef GLOBALS_H
#define GLOBALS_H


extern unsigned char far *video_buffer;
extern unsigned char far *offscreen_buffer;
extern GAME_STATE game_state;


#define DEBUG_SPRITE_BOXES 0 //set to 1 to show bounding boxes around all rendered sprites

#endif