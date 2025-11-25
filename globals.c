#include <alloc.h>
#include "globals.h"
#include <dos.h>
#include "state.h"

unsigned char far *video_buffer = MK_FP(0xA000, 0);
unsigned char far *offscreen_buffer = NULL;

GAME_STATE game_state;

