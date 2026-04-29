#include "ui.h"
#include "bitmap.h"
#include "draw.h"

BITMAP ui_metal_box_img;
BITMAP ui_scroll_img;

void load_ui_assets(){
  load_bmp("comasset/ui/metalbox.bmp", &ui_metal_box_img);
  load_bmp("comasset/ui/scroll.bmp", &ui_scroll_img);
}

void unload_ui_assets(){
  farfree(ui_metal_box_img.data);
  farfree(ui_scroll_img.data);
}

BITMAP * resolve_sprite(int sprite){
  switch (sprite){
    case UI_METAL_BOX:
      return &ui_metal_box_img;
    case UI_SCROLL_TITLE:
      return &ui_scroll_img;
  }
}

void draw_ui_sprite(int sprite, int targetX, int targetY, int targetWidth, int targetHeight){
  BITMAP * bmp = resolve_sprite(sprite);
  if (targetWidth <= bmp->width){
    targetWidth = 0;
  }
  if (targetHeight <= bmp->height){
    targetHeight = 0;
  }
  if (targetWidth > 0 || targetHeight > 0){
    draw_ui_sprite_img(bmp, targetX, targetY, targetWidth, targetHeight);
  } else {
    draw_transparent_bitmap(bmp, targetX, targetY);
  } 
}

void draw_ui_sprite_img(BITMAP * sprite, int targetX, int targetY, int targetWidth, int targetHeight){
  int centerColor; //color to fill in 9-slice expanded box middle
  int edgeHeight, middleHeight;
  int edgeWidth, middleWidth;
  int topY, middleY, bottomY;
  int leftX, middleX, rightX;
  int i;
  
  edgeWidth = (sprite->width / 2) - 1; //probably need to check if even or odd.
  edgeHeight = (sprite->height / 2) - 1;
  if (targetWidth > 0){
    middleWidth = targetWidth - (edgeWidth * 2);
  } else {
    middleWidth = 0;
  }
  if (targetHeight > 0){
    middleHeight = targetHeight - (edgeHeight * 2);
  } else {
    middleHeight = 0;
  }
  
  topY = targetY;
  middleY = topY + edgeHeight;
  bottomY = middleY + middleHeight;
  leftX = targetX;
  middleX = leftX + edgeWidth;
  rightX = middleX + middleWidth;

  //top left
  draw_bitmap_sprite(sprite, 0, 0, edgeWidth, edgeHeight, leftX, topY, true, 0);
  if (targetWidth > 0){
    //expanded center top - take middle slice and copy it to fill the space
    for (i = 0; i < middleWidth; i++){
      draw_bitmap_sprite(sprite, edgeWidth, 0, 1, edgeHeight, middleX + i, topY, true, 0);
    }
  }
  
  //top right
  draw_bitmap_sprite(sprite, sprite->width - edgeWidth, 0, edgeWidth, edgeHeight, rightX, topY, true, 0);

  if (targetHeight > 0){
    //expanded middle left
    for (i = 0; i < middleHeight; i++){
      draw_bitmap_sprite(sprite, 0, sprite->height - edgeHeight, edgeWidth, 1, leftX, middleY + i, true, 0);
    }

    if (targetWidth > 0){
      //middle - TODO solid color sampled from source
      centerColor = sprite->data[(edgeHeight * sprite->width) + edgeWidth];
      draw_rect(middleX, middleY, middleWidth, middleHeight, centerColor);
    }

    //expanded middle right
    for (i = 0; i < middleHeight; i++){
      draw_bitmap_sprite(sprite, sprite->width - edgeWidth, edgeHeight, edgeWidth, 1, rightX, middleY + i, true, 0);
    }

  }
  
  //bottom left
  draw_bitmap_sprite(sprite, 0, sprite->height - edgeHeight, edgeWidth, edgeHeight, leftX, bottomY, true, 0);

  if (targetWidth > 0){
    //expanded center bottom
    for (i = 0; i < middleWidth; i++){
      draw_bitmap_sprite(sprite, edgeWidth, sprite->height - edgeHeight, 1, edgeHeight, middleX + i, bottomY, true, 0);
    }
  }
  //bottom right
  draw_bitmap_sprite(sprite, sprite->width - edgeWidth, sprite->height - edgeHeight, edgeWidth, edgeHeight, rightX, bottomY, true, false);
}

BITMAP backdrop;
int backdrop_loaded = false;

void backdrop_init(const char * path){
  if (backdrop_loaded){
    farfree(backdrop.data);
  }
  load_bmp(path, &backdrop);
  backdrop_loaded = true;
}

void backdrop_draw(){
  if (!backdrop_loaded){
    return;
  }
  draw_bitmap(&backdrop, 0, 0);
}

void backdrop_draw_part(int x, int y, int width, int height){
  if (!backdrop_loaded){
    return;
  }
  draw_bitmap_sprite(&backdrop, x, y, width, height, x, y, false, 0);
}

void backdrop_free(){
  if (!backdrop_loaded){
    return;
  }
  farfree(backdrop.data);
  backdrop_loaded = false;
}