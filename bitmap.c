#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <alloc.h>
#include <mem.h>
#include "globals.h"
#include "vga.h"
#include "bitmap.h"

void fskip(FILE *fp, int num_bytes)
{
   int i;
   for (i=0; i<num_bytes; i++)
      fgetc(fp);
}

void load_bmp(const char *file, BITMAP *b) {
    FILE *fp;
    word num_colors;
    int x, y;
    int rowSize;
    byte *tempRow;
    unsigned long imageSize;

    // Open file
    if ((fp = fopen(file, "rb")) == NULL) {
        printf("Error opening file %s.\n", file);
        exit(1);
    }

    // Validate BMP signature
    if (fgetc(fp) != 'B' || fgetc(fp) != 'M') {
        fclose(fp);
        printf("%s is not a valid BMP file.\n", file);
        exit(1);
    }

    // Skip to width and height (offset 18 and 22)
    fskip(fp, 16);
    fread(&b->width, sizeof(word), 1, fp);
    fskip(fp, 2);
    fread(&b->height, sizeof(word), 1, fp);

    // Skip to color count (offset 46)
    fskip(fp, 22);
    fread(&num_colors, sizeof(word), 1, fp);
    fskip(fp, 6);

    if (num_colors == 0) num_colors = 256;

    // Validate image size
    imageSize = (unsigned long)b->width * b->height;
    if (imageSize > 65535UL) {
        printf("Image too large: %lu bytes\n", imageSize);
        fclose(fp);
        exit(1);
    }

    // Allocate image buffer
    b->data = (byte far *)farmalloc(imageSize);
    if (b->data == NULL) {
        printf("Memory allocation failed for %s.\n", file);
        fclose(fp);
        exit(1);
    }

    // Skip palette
    fskip(fp, num_colors * 4);

    // Calculate padded row size
    rowSize = ((b->width + 3) / 4) * 4;
    tempRow = (byte *)malloc(rowSize);
    if (tempRow == NULL) {
        printf("Failed to allocate temp row buffer.\n");
        farfree(b->data);
        fclose(fp);
        exit(1);
    }

    // Read pixel data (bottom-up)
    for (y = b->height - 1; y >= 0; y--) {
        fread(tempRow, 1, rowSize, fp);
        for (x = 0; x < b->width; x++) {
            b->data[y * b->width + x] = tempRow[x];
        }
    }

    free(tempRow);
    fclose(fp);
}


void draw_bitmap(BITMAP *bmp, int x,int y)
{
	draw_bitmap_sprite(bmp, 0, 0, bmp->width, bmp->height, x, y, false, 0);
}


void draw_transparent_bitmap(BITMAP *bmp, int x,int y)
{
	draw_bitmap_sprite(bmp, 0, 0, bmp->width, bmp->height, x, y, true, 0);
}

int image_byte_pos(int width, int x, int y){
	return	(y * width) + x;
}

void draw_bitmap_sprite(BITMAP *spritesheet, int sourceX, int sourceY, int width, int height, int drawX, int drawY, bool transparent, int maskColor){

	int i,j, clipX, clipY;
	word screen_offset;
	word bitmap_offset;
	byte data;
	clipX = 0;
	clipY = 0;

	if (sourceX + width > spritesheet->width)
		width = spritesheet->width - sourceX;

	if (sourceY + height > spritesheet->height)
		height = spritesheet->height - sourceY;

	if (drawX + width < 0 || drawY + height < 0 || width <= 0 || height <= 0) {
		return;
	}

	if ((drawX < 0 && (0-drawX) >= width) || (drawY < 0 && (0-drawY) >= height)){
		//completely obscured
		return;
	}

	if (drawX < 0){
		clipX = 0-drawX;
	}
	if (drawY < 0){
		clipY = 0-drawY;
	}

	for (j = 0; j < height - clipY; j++) {
		int srcY = sourceY + clipY + j;
		int dstY = drawY + clipY + j;
		if (dstY >= SCREEN_HEIGHT) break;

		for (i = 0; i < width - clipX; i++) {
			int srcX = sourceX + clipX + i;
			int dstX = drawX + clipX + i;
			if (dstX >= SCREEN_WIDTH) break;

			bitmap_offset = image_byte_pos(spritesheet->width, srcX, srcY);
			screen_offset = image_byte_pos(SCREEN_WIDTH, dstX, dstY);
			if (bitmap_offset < 0 || bitmap_offset >= spritesheet->width * spritesheet->height){
				continue;
			}
			if (screen_offset < 0 || screen_offset >= SCREEN_WIDTH * SCREEN_HEIGHT){
				continue;
			}

			data = spritesheet->data[bitmap_offset];

			if (DEBUG_SPRITE_BOXES && (j == 0 || j == height - clipY - 1 || i == 0 || i == width - clipX - 1)){
				offscreen_buffer[screen_offset] = 15;
			} else {
				if (maskColor && data == 15){
					offscreen_buffer[screen_offset] = maskColor;
					continue;
				}
				if (!transparent || data)
					offscreen_buffer[screen_offset] = data;
			}
		}
	}
}
