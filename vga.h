#include "globals.h"

#ifndef VGA_H
#define VGA_H

#define MODE13H             0x13
#define TEXT_MODE           0x03
#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define NUM_COLORS          256       /* number of colors in mode 0x13 */
#define SCREEN_BUFFER_SIZE  SCREEN_HEIGHT * SCREEN_WIDTH
#define PALETTE_INDEX       0x03c8
#define PALETTE_DATA        0x03c9
#define INPUT_STATUS        0x03da
#define VRETRACE            0x08

void set_video_mode(int vmode);
int  create_offscreen_buffer();
void show_offscreen_buffer(void);
void delete_offscreen_buffer(void);
void set_palette(byte *palette);
void set_shared_palette();
void wait_for_retrace(void);

#endif