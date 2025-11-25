#ifndef BMPFNT_H
#define BMPFNT_H

#define MAX_CHARS 256
#define MAX_LINE 256
#define MAX_KERNINGS 256
#define MAX_GLYPHS 400
#define GLYPHSTYLE_DEFAULT 0
#define GLYPHSTYLE_NEXT 1
#define GLYPHSTYLE_CORRECT 2
#define GLYPHSTYLE_ERROR 3

typedef struct {
    int id;
    int x, y;
    int width, height;
    int xoffset, yoffset;
    int xadvance;
    int page;
    int chnl;
} FontChar;


typedef struct {
    int first;
    int second;
    int amount;
} KerningPair;

typedef struct {
    char face[64];
    int size;
    int bold;
    int italic;
    char charset[128];
    int unicode;
    int lineHeight;
    int base;
    int inverseBase;
    int scaleW, scaleH;
    int pages;
    char pageFile[128];
    int charCount;
    FontChar chars[MAX_CHARS];
    int kerningCount;
    KerningPair kernings[MAX_KERNINGS];
} BitmapFont;

typedef enum {
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
} TextAlign;

typedef enum {
    ALIGN_TOP,
    ALIGN_MIDDLE,
    ALIGN_BOTTOM
} VerticalAlign;

typedef struct {
    int charId;
    int x, y;
    int style; // e.g. 0 = normal, 1 = correct, 2 = incorrect, 3 = next
} GlyphRenderInfo;

typedef struct {
    int glyphCount;
    GlyphRenderInfo glyphs[MAX_GLYPHS];
} GlyphSet;

void parseFntFile(const char *filename, BitmapFont *font);
int getKerning(BitmapFont *font, int first, int second);
void get_text_glyphs(BitmapFont *font, GlyphSet *glyphs, const char *text, int startX, int startY, int containerWidth, int containerHeight, TextAlign align, VerticalAlign vAlign);
void render_text_glyphs(BitmapFont *font, BITMAP *spritesheet, GlyphSet *glyphs, int customTextColor);
void draw_text(BitmapFont *font, BITMAP *spritesheet, const char *text, int startX, int startY, int containerWidth, int containerHeight, TextAlign align, VerticalAlign vAlign);
void validate_font_bitmap(BitmapFont *font, BITMAP *spritesheet);

#endif
