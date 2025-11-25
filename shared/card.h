#ifndef CARD_H
#define CARD_H

#pragma pack(push, 1)
typedef struct {
	char Title[20];
   int EffectType;
   int Effect;
   int MinWPM;
   int MinAccuracy;
   int IconSheet;
   int IconIndex;
   char Description[150];
} Card;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	int version;
	int count;
} CardHeader;
#pragma pack(pop)

void read_header(CardHeader *header, const char file_name);

#endif