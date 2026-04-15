#ifndef SOUND_H
#define SOUND_H

extern int opl2;
extern int opl3;

typedef struct {
  unsigned int active;
  unsigned int mod_20, mod_40, mod_60, mod_80, mod_E0;
  unsigned int car_20, car_40, car_60, car_80, car_E0;
  unsigned int feedback;
} OPLInstrument;
 
#define MAX_INSTRUMENTS 20
#define PATTERN_MAX 50
#define CHANNEL_COUNT 11
#define SEQUENCE_MAX 60
#define NOTES_PER_PATTERN 16
#define SONG_COUNTER_MAX SEQUENCE_MAX * NOTES_PER_PATTERN

#define EMPTY_INSTRUMENT { 0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
#define EMPTY_ACTIVE_INSTRUMENT { 1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
#define EMPTY_PATTERN { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
#define EMPTY_SEQUENCE \
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, \
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }

#define BD -3
#define SD -4
#define TT -5
#define CM -6
#define HH -7

#define C0  0
#define Cs0 1
#define D0  2
#define Ds0 3
#define E0  4
#define F0  5
#define Fs0 6
#define G0  7
#define Gs0 8
#define A0  9
#define As0 10
#define B0  11

#define C1  12
#define Cs1 13
#define D1  14
#define Ds1 15
#define E1  16
#define F1  17
#define Fs1 18
#define G1  19
#define Gs1 20
#define A1  21
#define As1 22
#define B1  23

#define C2  24
#define Cs2 25
#define D2  26
#define Ds2 27
#define E2  28
#define F2  29
#define Fs2 30
#define G2  31
#define Gs2 32
#define A2  33
#define As2 34
#define B2  35

#define C3  36
#define Cs3 37
#define D3  38
#define Ds3 39
#define E3  40
#define F3  41
#define Fs3 42
#define G3  43
#define Gs3 44
#define A3  45
#define As3 46
#define B3  47

#define C4  48
#define Cs4 49
#define D4  50
#define Ds4 51
#define E4  52
#define F4  53
#define Fs4 54
#define G4  55
#define Gs4 56
#define A4  57
#define As4 58
#define B4  59

#define C5  60
#define Cs5 61
#define D5  62
#define Ds5 63
#define E5  64
#define F5  65
#define Fs5 66
#define G5  67
#define Gs5 68
#define A5  69
#define As5 70
#define B5  71

#define C6  72
#define Cs6 73
#define D6  74
#define Ds6 75
#define E6  76
#define F6  77
#define Fs6 78
#define G6  79
#define Gs6 80
#define A6  81
#define As6 82
#define B6  83

#define C7  84
#define Cs7  85
#define D7  86
#define Ds7 87
#define E7  88
#define F7  89
#define Fs7 90
#define G7  91
#define Gs7 92
#define A7  93
#define As7 94
#define B7  95


typedef struct {
  unsigned char tempo;
  unsigned char sequence_count; //how many measures until end of song
  OPLInstrument instruments[CHANNEL_COUNT];
  char patterns[PATTERN_MAX][NOTES_PER_PATTERN]; //[pattern][notes] (8 notes is 1 bar at 4/4)
  char sequence[CHANNEL_COUNT][SEQUENCE_MAX]; //[channel][pattern] (60 * 4 = 240 measures, ~2 minutes at 120bpm or 4 mins at 60)
} Song;

void opl_write(short index, char data);
void opl_play_note(int channel, unsigned int fnum, int octave);
void opl_stop_note(int channel);
void opl_init();
void opl_cleanup();
void set_tempo(unsigned int bpm, unsigned int resolution);
void set_instrument(int channel, OPLInstrument instrument);
void play_song(Song song);
void stop_song();
void play_battle_song();
void play_menu_song();


#endif