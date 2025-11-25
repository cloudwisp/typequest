#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <alloc.h>
#include <dos.h>
#include <mem.h>
#include "globals.h"
#include "vga.h"

unsigned char shared_palette[256 * 3] = {
	0,0,0,
	0,0,170,
	0,170,0,
	0,170,170,
	170,0,0,
	170,0,170,
	170,85,0,
	170,170,170,
	85,85,85,
	85,85,255,
	85,255,85,
	85,255,255,
	255,85,85,
	255,85,255,
	255,255,85,
	255,255,255,
	247,247,247,
	239,239,239,
	231,231,231,
	223,223,223,
	215,215,215,
	207,207,207,
	199,199,199,
	191,191,191,
	183,183,183,
	175,175,175,
	159,159,159,
	151,151,151,
	143,143,143,
	135,135,135,
	127,127,127,
	119,119,119,
	111,111,111,
	103,103,103,
	95,95,95,
	79,79,79,
	71,71,71,
	63,63,63,
	55,55,55,
	47,47,47,
	39,39,39,
	31,31,31,
	23,23,23,
	15,15,15,
	7,7,7,
	191,167,143,
	183,160,136,
	175,152,128,
	167,144,120,
	159,137,113,
	150,129,105,
	142,121,97,
	134,114,90,
	126,106,82,
	117,98,74,
	109,90,66,
	101,83,59,
	93,75,51,
	84,67,43,
	76,60,36,
	67,51,27,
	191,123,75,
	179,115,71,
	171,111,67,
	163,107,63,
	155,99,59,
	143,95,55,
	135,87,51,
	127,83,47,
	119,79,43,
	107,71,39,
	95,67,35,
	83,63,31,
	75,55,27,
	63,47,23,
	51,43,19,
	43,35,15,
	255,235,223,
	255,227,211,
	255,219,199,
	255,211,187,
	255,207,179,
	255,199,167,
	255,191,155,
	255,187,147,
	255,179,131,
	247,171,123,
	239,163,115,
	231,155,107,
	223,147,99,
	215,139,91,
	207,131,83,
	203,127,79,
	255,238,220,
	255,220,185,
	255,203,151,
	255,185,117,
	255,168,85,
	255,151,54,
	255,134,25,
	255,117,0,
	243,109,0,
	229,101,0,
	216,93,0,
	203,85,0,
	190,77,0,
	164,61,0,
	151,54,0,
	255,255,239,
	255,255,207,
	255,255,175,
	255,255,143,
	255,255,111,
	255,255,47,
	255,255,15,
	255,255,0,
	207,207,0,
	175,175,0,
	143,143,0,
	111,111,0,
	79,79,0,
	47,47,0,
	15,15,0,
	255,255,115,
	235,219,87,
	215,187,67,
	195,155,47,
	175,123,31,
	155,91,19,
	135,67,7,
	115,43,0,
	255,223,223,
	255,191,191,
	255,159,159,
	255,127,127,
	255,63,63,
	255,31,31,
	255,0,0,
	239,0,0,
	223,0,0,
	207,0,0,
	175,0,0,
	159,0,0,
	143,0,0,
	127,0,0,
	111,0,0,
	95,0,0,
	79,0,0,
	63,0,0,
	47,0,0,
	31,0,0,
	15,0,0,
	255,183,183,
	243,163,163,
	231,143,143,
	219,123,123,
	203,107,107,
	191,91,91,
	179,79,79,
	167,63,63,
	142,46,0,
	134,39,0,
	126,32,0,
	117,25,0,
	109,18,0,
	101,11,0,
	93,5,0,
	85,0,0,
	112,240,75,
	105,224,70,
	97,208,65,
	90,192,60,
	82,176,55,
	75,160,50,
	67,144,45,
	60,128,40,
	53,112,35,
	45,96,30,
	38,80,25,
	30,64,20,
	23,48,15,
	15,32,10,
	7,15,4,
	222,255,168,
	199,228,148,
	173,200,128,
	149,173,107,
	124,146,88,
	100,119,68,
	74,90,48,
	50,63,29,
	0,255,0,
	0,223,0,
	0,191,0,
	0,127,0,
	0,95,0,
	0,63,0,
	0,31,0,
	255,0,255,
	223,0,223,
	191,0,191,
	127,0,127,
	95,0,95,
	63,0,63,
	233,233,243,
	196,196,225,
	157,157,206,
	119,119,187,
	84,84,167,
	65,65,131,
	46,46,92,
	27,27,52,
	213,241,255,
	191,235,255,
	170,227,255,
	149,221,255,
	128,214,255,
	106,207,255,
	85,200,255,
	63,191,255,
	55,157,223,
	47,143,191,
	39,119,159,
	31,95,127,
	0,191,191,
	0,95,95,
	0,63,63,
	231,231,255,
	198,198,255,
	173,173,255,
	140,140,255,
	115,115,255,
	49,49,255,
	24,24,255,
	0,0,255,
	0,0,231,
	0,0,206,
	0,0,156,
	0,0,132,
	0,0,107,
	0,0,82,
	0,0,79,
	0,0,63,
	0,0,55,
	0,0,39,
	0,0,31,
	0,0,15,
	0,0,7,
	207,127,207,
	183,111,183,
	159,95,159,
	135,79,135,
	111,63,111,
	87,47,87,
	63,31,63,
	39,15,39
};


void set_video_mode(int vmode) {
	asm mov ah, 0
	asm mov al, BYTE PTR vmode
	asm int 10h
}


int create_offscreen_buffer()  {
	// allocate enough memory to hold the double buffer
	if ((offscreen_buffer = (unsigned char far *)farmalloc((unsigned int)SCREEN_WIDTH * SCREEN_HEIGHT))==NULL)
	  return 0;
	// fill the buffer with black
	_fmemset(offscreen_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
	return 1;
}

void wait_for_retrace(void)
{
	/* wait until done with vertical retrace */
	while  ((inp(INPUT_STATUS) & VRETRACE)) {};
	/* wait until done refreshing */
	while (!(inp(INPUT_STATUS) & VRETRACE)) {};
}


void show_offscreen_buffer(void)  {

  unsigned char far *buffer = offscreen_buffer;
  int buffer_size = (SCREEN_WIDTH * SCREEN_HEIGHT) / 2;

  wait_for_retrace();

  // this functions copies the double buffer into the video buffer
  asm   push ds               ; //save DS on stack
  asm   mov cx,buffer_size    ; //this is the size of buffer in WORDS
  asm   les di,video_buffer   ; //es:di is destination of memory move
  asm   lds si,buffer         ; //ds:si is source of memory move
  asm   cld                   ; //make sure to move in the right direction
  asm   rep movsw             ; //move all the words
  asm   pop ds                ; //restore the data segment
}

void delete_offscreen_buffer(void)  {
  // this function free's up the memory allocated by the double buffer
  if (offscreen_buffer) farfree(offscreen_buffer);
}

void set_palette(byte *palette)
{
  int i;

  outp(PALETTE_INDEX,0);              /* tell the VGA that palette data
                                         is coming. */
  for(i=0;i<256*3;i++)
    outp(PALETTE_DATA, palette[i] / 4);    /* write the data */
}

void set_shared_palette(){
	set_palette(&shared_palette);
}


