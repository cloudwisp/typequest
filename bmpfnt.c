#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <alloc.h>
#include "bitmap.h"
#include "bmpfnt.h"

#define LINEBUF(row, col) lineBuffer[(row) * 256 + (col)]


void parseFntFile(const char *filename, BitmapFont *font) {
    FILE *fp = fopen(filename, "r");
	 char line[MAX_LINE];
	 int charIndex = 0;
	 int j, tallestOffset;


    if (!fp) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    while (fgets(line, MAX_LINE, fp)) {
        if (strncmp(line, "info ", 5) == 0) {
            sscanf(line, "info face=\"%[^\"]\" size=%d bold=%d italic=%d charset=\"%[^\"]\" unicode=%d",
                   font->face, &font->size, &font->bold, &font->italic, font->charset, &font->unicode);
        } else if (strncmp(line, "common ", 7) == 0) {
            sscanf(line, "common lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d",
                   &font->lineHeight, &font->base, &font->scaleW, &font->scaleH, &font->pages);
        } else if (strncmp(line, "page ", 5) == 0) {
            sscanf(line, "page id=%*d file=\"%[^\"]\"", font->pageFile);
        } else if (strncmp(line, "chars count=", 12) == 0) {
            sscanf(line, "chars count=%d", &font->charCount);
        } else if (strncmp(line, "char id=", 8) == 0) {
            FontChar *ch = &font->chars[charIndex];
            sscanf(line, "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d chnl=%d",
                   &ch->id, &ch->x, &ch->y, &ch->width, &ch->height,
                   &ch->xoffset, &ch->yoffset, &ch->xadvance, &ch->page, &ch->chnl);
            charIndex++;
        }
         else if (strncmp(line, "kernings count=", 16) == 0) {
               sscanf(line, "kernings count=%d", &font->kerningCount);
         } else if (strncmp(line, "kerning first=", 14) == 0) {
               KerningPair *kp = &font->kernings[font->kerningCount++];
               sscanf(line, "kerning first=%d second=%d amount=%d",
                   &kp->first, &kp->second, &kp->amount);
         }
    }

    tallestOffset = 0;
    for (j = 0; j < font->charCount; j++) {
    	if (font->chars[j].yoffset > tallestOffset){
       	tallestOffset = font->chars[j].yoffset;
      }
    }
    font->inverseBase = font->base - tallestOffset;

    fclose(fp);
}

int getKerning(BitmapFont *font, int first, int second) {
    int i;
    for (i = 0; i < font->kerningCount; i++) {
        if (font->kernings[i].first == first && font->kernings[i].second == second) {
            return font->kernings[i].amount;
        }
    }
    return 0;
}

void draw_text(BitmapFont *font, BITMAP *spritesheet, const char *text,
								int startX, int startY, int containerWidth, int containerHeight, TextAlign align, VerticalAlign vAlign) {
 GlyphSet *glyphs = (GlyphSet far *)farmalloc(sizeof(GlyphSet));
	 get_text_glyphs(font, glyphs, text, startX, startY, containerWidth, containerHeight, align, vAlign);

	 render_text_glyphs(font, spritesheet, glyphs, 0);

	 farfree(glyphs);
}


void get_text_glyphs(BitmapFont *font, GlyphSet *glyphs, const char *text, int startX, int startY, int containerWidth, int containerHeight, TextAlign align, VerticalAlign vAlign){
	int x = startX;
    int y = startY;
    int i = 0;
    int prevCharId = -1;

    char far *lineBuffer = (char far *)farmalloc(20 * 256);
    int far *lineWidths = (int far *)farmalloc(20 * sizeof(int));
    char far *word = (char far *)farmalloc(128 * sizeof(char));
    int lineCount = 0;
    int j, k;
    FontChar *ch;
    int charId;
    int l, drawX;
    int wordStart;
    int curGlyph = 0;
    int totalHeight = 0;
	 int yOffset = 0;
    glyphs->glyphCount = 0;

	 // Step 1: Wrap text into lines
    while (text[i] != '\0') {
        int lineLen = 0;
        int lineWidth = 0;
        prevCharId = -1;

        // Build one line
        while (text[i] != '\0' && text[i] != '\n') {
            int wordLen = 0;
            int wordWidth = 0;

            // Extract word
            wordStart = i;
            while (text[i] != '\0' && text[i] != ' ' && text[i] != '\n') {
                word[wordLen++] = text[i++];
            }
            word[wordLen] = '\0';

            // Measure word
            prevCharId = -1;
            for (j = 0; j < wordLen; j++) {
                charId = (int)word[j];
                ch = NULL;
                for (k = 0; k < font->charCount; k++) {
                    if (font->chars[k].id == charId) {
                        ch = &font->chars[k];
                        break;
                    }
                }
                if (!ch) continue;
                if (prevCharId != -1) wordWidth += 0; //getKerning(font, prevCharId, charId);
                wordWidth += ch->xadvance;
                prevCharId = charId;
            }

            // Check if word fits
            if (lineWidth + wordWidth > containerWidth && lineWidth > 0){
            	i = wordStart;
               break;
            }

            // Append word to line
            for (j = 0; j < wordLen; j++) {
                LINEBUF(lineCount, lineLen++) = word[j];
            }

            // Add space if needed
            if (text[i] == ' ') {
                LINEBUF(lineCount,lineLen++) = ' ';
                for (j = 0; j < font->charCount; j++) {
                    if (font->chars[j].id == ' ') {
                        lineWidth += font->chars[j].xadvance;
                        break;
                    }
                }
                i++;
            }

            lineWidth += wordWidth;
        }

        // Handle newline
        if (text[i] == '\n') i++;

        LINEBUF(lineCount,lineLen) = '\0';
        lineWidths[lineCount] = lineWidth;
        lineCount++;
	 }

    totalHeight = lineCount * font->lineHeight;
    switch (vAlign) {
        case ALIGN_TOP:    yOffset = 0; break;
        case ALIGN_MIDDLE: yOffset = (containerHeight - totalHeight) / 2; break;
        case ALIGN_BOTTOM: yOffset = containerHeight - totalHeight; break;
    }


    // Step 2: Render each line with alignment

    for (l = 0; l < lineCount; l++) {
        drawX = startX;

        if (align == ALIGN_CENTER) {
            drawX += (containerWidth - lineWidths[l]) / 2;
        } else if (align == ALIGN_RIGHT) {
            drawX += (containerWidth - lineWidths[l]);
        }

        x = drawX;
        prevCharId = -1;

        for (j = 0; LINEBUF(l,j) != '\0'; j++) {
            charId = (int)LINEBUF(l,j);
				ch = NULL;
            for (k = 0; k < font->charCount; k++) {
                if (font->chars[k].id == charId) {
                    ch = &font->chars[k];
                    break;
                }
            }
            if (!ch) continue;
            if (prevCharId != -1) x += getKerning(font, prevCharId, charId);

            glyphs->glyphs[curGlyph].charId = charId;
            glyphs->glyphs[curGlyph].x = x + ch->xoffset;
            glyphs->glyphs[curGlyph].y = (y + ch->yoffset - font->inverseBase) + yOffset;
            glyphs->glyphs[curGlyph].style = GLYPHSTYLE_DEFAULT;
            glyphs->glyphCount++;
            curGlyph++;
            x += ch->xadvance;
            prevCharId = charId;
        }

        y += font->lineHeight;
	 }
    farfree(lineBuffer);
    farfree(lineWidths);
    farfree(word);
}

void render_text_glyphs(BitmapFont *font, BITMAP *spritesheet, GlyphSet *glyphs, int customTextColor){
	int i, k;
	FontChar *ch;
	int maskColor;
   FontStyle style;

   for (i = 0; i < glyphs->glyphCount; i++){

   			ch = NULL;
            for (k = 0; k < font->charCount; k++) {
                if (font->chars[k].id == glyphs->glyphs[i].charId) {
						  ch = &font->chars[k];
                    break;
                }
            }
				style = glyphs->glyphs[i].style;
				if (customTextColor){
					maskColor = customTextColor;
				} else {
					maskColor = 15;
					switch (style){
					  case Normal: maskColor = 15; break;
					  case Next: maskColor = 7; break;
					  case Incorrect: maskColor = 12; break;
					  case Correct: maskColor = 10; break;
					}
				}
		
            draw_bitmap_sprite(
                spritesheet,
                ch->x, ch->y,
                ch->width, ch->height,
					 glyphs->glyphs[i].x, glyphs->glyphs[i].y,
					 true,
                maskColor
            );

   }
}

void validate_font_bitmap(BitmapFont *font, BITMAP *spritesheet) {
    int i;
    FontChar *ch;

    printf("Validating font against spritesheet...\n");

    for (i = 0; i < font->charCount; i++) {
        ch = &font->chars[i];

        // Check for invalid dimensions
        if (ch->width <= 0 || ch->height <= 0) {
            printf("Char ID %d has invalid size: %dx%d\n", ch->id, ch->width, ch->height);
            continue;
        }

        // Check for out-of-bounds source rect
        if (ch->x < 0 || ch->y < 0 ||
            ch->x + ch->width > spritesheet->width ||
            ch->y + ch->height > spritesheet->height) {
            printf("Char ID %d out of bounds: x=%d y=%d w=%d h=%d\n",
                   ch->id, ch->x, ch->y, ch->width, ch->height);
        }

        // Check for unsupported page index
        if (ch->page != 0) {
            printf("Char ID %d uses unsupported page %d (only page 0 supported)\n", ch->id, ch->page);
        }
    }

    printf("Validation complete.\n");
}
