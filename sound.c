#include <dos.h>
#include "sound.h"
#include "globals.h"
#include "timer.h"


int opl2;
int opl3;
unsigned int music_note_duration;
unsigned int music_bpm;
unsigned int music_resolution;
unsigned long next_note_tick;

//frequency lookup table - from docs
unsigned int flookup[12] = {
  0x157,
  0x16B,
  0x181,
  0x198,
  0x1B0,
  0x1CA,
  0x1E5,
  0x202,
  0x220,
  0x241,
  0x263,
  0x287
};

unsigned char modulator_offsets[9] = {
  0x0, 0x01, 0x02, 0x08, 0x09, 0x0A, 0x10, 0x11, 0x12
};

unsigned char carrier_offsets[9] = {
  0x03, 0x04, 0x05, 0x0B, 0x0C, 0x0D, 0x13, 0x14, 0x15
};

int sequence_counter = -1;
int note_counter = -1;
Song currentSong;

OPLInstrument bass_drum_6 = {
    1,
    0x01,   // mod_20
    0x10,   // mod_40: output level
    0xF6,   // mod_60: attack=F, decay6
    0x09,   // mod_80: sustain=0, release=9
    0x00,   // mod_E0: sine
    0x01,   // car_20
    0x00,   // car_40
    0xF2,   // car_60
    0x09,   // car_80
    0x00,   // car_E0
    0x00    // feedback = 0 (important)
};

OPLInstrument snare_hihat_7 = {
    1,
    0x01,   // mod_20
    0x10,   // mod_40
    0xF8,   // attack/decay
    0xF5,   // sustain/release
    0x00,   // mod_E0: sine
    0x01,   // car_20
    0x10,   // car_40
    0xF6,   // attack/decay
    0xF5,   // sustain/release
    0x00,   // car_E0
    0x00
};

OPLInstrument tom_cymbal_8 = {
    1,
    0x01,   // mod_20
    0x00,   // mod_40
    0xF8,   // attack/decay
    0x05,   // sustain/release
    0x00,   // mod_E0: sine
    0x01,   // car_20
    0x10,   // car_40
    0xF6,   // attack/decay
    0xFF,   // sustain/release
    0x00,   // car_E0
    0x00
};

Song menu_song = {
    //tempo
    60,
    //sequence count
    4,
    //instruments for each channel 0-9 (+ 2 dummy for percussion channels)
    {
        { 
            //0: Bass
            1,      // active
            0x21,   // mod_20
            0x18,   // mod_40 output level
            0x52,   // mod_60 attack/decay
            0x0A,   // mod_80 sustain/release
            0x00,   // mod_E0 wave
            0x01,   // car_20
            0x00,   // car_40 output level
            0x68,   // car_60 attack/decay
            0x05,   // car_80 sustain/release
            0x00,   // car_E0 wave
            0x06    // feedback
        },
        //1: Lead 1
        {
            1,      // active
            0x01,   // mod_20
            0x3F,   // mod_40 output level
            0xF7,   // mod_60 attack/decay
            0xF0,   // mod_80 sustain/release
            0x00,   // mod_E0 wave
            0x01,   // car_20
            0x00,   // car_40 output level
            0xF7,   // car_60 attack/decay
            0xF0,   // car_80 sustain/release
            0x00,   // car_E0 wave
            0x00    // feedback
        },
        // 2: Lead 2
        {
            1,      // active
            0x01,   // mod_20
            0x3F,   // mod_40 output level
            0xF7,   // mod_60 attack/decay
            0xF0,   // mod_80 sustain/release
            0x01,   // mod_E0 wave
            0x01,   // car_20
            0x00,   // car_40 output level
            0xF7,   // car_60 attack/decay
            0xF0,   // car_80 sustain/release
            0x01,   // car_E0 wave
            0x02    // feedback
        },
        EMPTY_INSTRUMENT,
        EMPTY_INSTRUMENT,
        EMPTY_INSTRUMENT,
        //percussion - unused
        EMPTY_INSTRUMENT,
        EMPTY_INSTRUMENT,
        EMPTY_INSTRUMENT,
        EMPTY_INSTRUMENT,
        EMPTY_INSTRUMENT
    },
    //patterns, -1 is rest with stop previous note. -2 is rest with sustain (no stop)
    {
        //0: Bass - Slow alternate B then A
        {  C2,  -2,  -2,  -2,  C3,  -2,  -2,  -2, As1,  -2,  -2,  -2, As2,  -2,  -2,  -2 },
        //1: Basee phrase 2.
        {  C4,  D4, Ds4,  F4,  G4,  -2,  -2,  -2,  -2,  C5,  -2, As4,  -2,  -2,  G4,  G4 },
        //2: lead phrase 1
        {  C4,  -2, Gs4,  G4,  -2,  -2,  F4,  G4,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2 },
        //3: lead phrase 2
        {  C5,  -2, Gs5,  G5,  -2,  F5, Ds5,  D5,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2 },
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN
    },
    //sequence - one row per channel, reference pattern index.
    // -1 for no pattern
    {
        {  0,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        {  1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        {  2,  3,  2,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
    }
};


Song battle_song = {
    //tempo
    107,
    //sequence count
    8,
    //instruments for each channel 0-9
    {
        { 
            //0: Bass
            1,      // active
            0x21,   // mod_20
            0x18,   // mod_40 output level
            0x52,   // mod_60 attack/decay
            0x40,   // mod_80 sustain/release
            0x00,   // mod_E0 wave
            0x01,   // car_20
            0x00,   // car_40 output level
            0x68,   // car_60 attack/decay
            0x20,   // car_80 sustain/release
            0x00,   // car_E0 wave
            0x06    // feedback
        },
        //1: Lead 1
        {
            1,      // active
            0x01,   // mod_20
            0xFF,   // mod_40 output level
            0xF7,   // mod_60 attack/decay
            0x80,   // mod_80 sustain/release
            0x01,   // mod_E0 wave
            0x01,   // car_20
            0x00,   // car_40 output level
            0xF7,   // car_60 attack/decay
            0x80,   // car_80 sustain/release
            0x01,   // car_E0 wave
            0x02    // feedback
        },
        // 2: Lead 2
        {
            1,      // active
            0x01,   // mod_20
            0x3F,   // mod_40 output level
            0xF7,   // mod_60 attack/decay
            0xF0,   // mod_80 sustain/release
            0x01,   // mod_E0 wave
            0x01,   // car_20
            0x00,   // car_40 output level
            0xF7,   // car_60 attack/decay
            0xF0,   // car_80 sustain/release
            0x01,   // car_E0 wave
            0x02    // feedback
        },
        EMPTY_INSTRUMENT,
        EMPTY_INSTRUMENT,
        EMPTY_INSTRUMENT,
        //Percussion - unused
        EMPTY_ACTIVE_INSTRUMENT,
        EMPTY_ACTIVE_INSTRUMENT,
        EMPTY_ACTIVE_INSTRUMENT,
        EMPTY_ACTIVE_INSTRUMENT,
        EMPTY_ACTIVE_INSTRUMENT
    },
    //patterns, -1 is rest with stop previous note. -2 is rest with sustain (no stop)
    {
        //0: Bass Ds2
        { Ds2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2 },
        //1: Bass Fs2.
        { Fs2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2 },
        //2: Bass Gs2
        { Gs2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2 },
        //3: Bass As2
        { As2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2 },
        //4: Lead arp
        {  C4,  F4, Fs4, As4,  -2,  -2, Ds4,  -2,  -2,  -2,  -2,  -2, Cs4,  -2,  -2,  -2 },
        //5: Bass drum and hi hat
        {  HH,  HH,  HH,  HH,  HH,  HH,  HH,  HH,  HH,  HH,  HH,  HH,  HH,  HH,  HH,  HH },
        //6: snare off beat
        {  BD,  -2,  -2,  -2,  SD,  -2,  -2,  -2,  BD,  -2,  -2,  -2,  SD,  -2,  -2,  -2 },
        //7: tom and crash
        {  -2,  -2,  -2,  -2,  -2,  -2,  TT,  TT,  CM,  -2,  -2,  -2,  -2,  -2,  -2,  -2 },
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN,
        EMPTY_PATTERN
    },
    //sequence - one row per channel, reference pattern index.
    // -1 for no pattern
    {
        {  0,  1,  2,  3,  0,  1,  2,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        {  4,  4,  4,  4,  4,  4,  4,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        //hi hat
        {  5,  5,  5,  5,  5,  5,  5,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        //bd and snare
        {  6,  6,  6,  6,  6,  6,  6,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        //tt and cymbal
        {  7, -2,  7, -1,  7, -1,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
    }
};


void opl_write(short index, char data) {
  if (index < 0x100) {
    outp(0x388,index);
    outp(0x389,data);
  } else {
    outp(0x38A,index);
    outp(0x38B,data);
  }
}

void opl_set_modulator(int channel, unsigned char attack_decay, unsigned char sustain_release, unsigned char output_level, unsigned char waveform){
  unsigned char offset;
  offset = modulator_offsets[channel];
  opl_write(0xE0 + offset, waveform);
  opl_write(0x60 + offset, attack_decay);
  opl_write(0x80 + offset, sustain_release);
  opl_write(0x40 + offset, output_level);
  
}

void opl_set_carrier(int channel, unsigned char attack_decay, unsigned char sustain_release, unsigned char output_level, unsigned char waveform){
  unsigned char offset;
  offset = carrier_offsets[channel];
  opl_write(0xE0 + offset, waveform);
  opl_write(0x60 + offset, attack_decay);
  opl_write(0x80 + offset, sustain_release);
  opl_write(0x40 + offset, output_level);
  
}

void opl_set_carrier_20h(int channel, unsigned char data){
  opl_write(0x20 + carrier_offsets[channel], data);
}

void opl_set_modulator_20h(int channel, unsigned char data){
  opl_write(0x20 + modulator_offsets[channel], data);
}

void opl_play_note(int channel, unsigned int fnum, int octave){
  unsigned char data;
  //fnum is a 10-bit value. Low bits and high bits
  //need to be written separately to different indexes 0xA[ch] And 0xB[ch].
  opl_write(0xA0 + channel, fnum & 0xFF); //lower 8 bits
  // high bits, octave and key_on flag
  data = ((fnum >> 8) &  0x03) //shift 8 bits, AND with 0x3 (111)
                      | (octave << 2) 
                      | 0x20; // note on mask
  opl_write(0xB0 + channel, data);                  
}

void opl_play_note_fullscale(int channel, unsigned int note){
  //translate note number from sequencer to scale + fnum
  unsigned int octave;
  unsigned int fnum;
  octave = note / 12; //floor value. e.g. 11/12 = 0, 13/12 = 1
  fnum = note - (octave * 12); //e.g. for 13, 13 - (1 * 12) = 1 (2nd note)
  opl_play_note(channel, flookup[fnum], octave);
}

void opl_stop_note(int channel){
  unsigned char data;
  data = inp(0xB0+channel); //read value from B register
  data &= ~0x20; //AND with inverse mask of note on
  opl_write(0xB0 + channel, data);
}

void opl_init_percussion(){
  unsigned int ch;
  unsigned int fnum;
  int octave;
  octave = 4;

  opl_write(0xBD, 0x20); // enable rhythm mode
  
  set_instrument(6, bass_drum_6);
  set_instrument(7, snare_hihat_7);
  set_instrument(8, tom_cymbal_8);

  //set fnums of channel 7 and 8
  for (ch = 6; ch < 9; ch++){
    if (ch == 6){
      fnum = 0x157;
      octave = 1;
    }
    else if (ch == 7){
      fnum = 0x203;
      octave = 1;
    }
    else {
      fnum = 0x157;
      octave = 3;
    }
    opl_write(0xA0 + ch, fnum & 0xFF);
    opl_write(0xB0 + ch, ((fnum >> 8) &  0x03) | (octave << 2) );
  }

}

void opl_stop_percussion_notes(){
  opl_write(0xBD, 0x20); //all off, except for perc mode flag
}


void opl_init(){
  unsigned char s1, s2;

  opl2 = 0;
  opl3 = 0;
  
  //Initialize adlib card
  opl_write(0x04,0x60); //reset both timers
  opl_write(0x04,0x80); //set IRQReset
  s1 = inp(0x388) & 0xE0; //read status port
  opl_write(0x02,0xFF); //initialize Timer 1
  opl_write(0x04,0x21); //start Timer 1
  delay(200); //wait at least 80 MS
  s2 = inp(0x388) & 0xE0; //read status port
  opl_write(0x04,0x60); //reset both timers
  opl_write(0x04,0x80); //set IRQReset

  //debugging, print status registeers
  //printf("s1 %02X s2 %02X\r\n", s1, s2);
  
  opl2 = s1==0 && s2==0xC0; //expect 0 from 1st status, and 0xC0 from second for successful init and detect

  //Check for OPL3 support
  if ((inp(0x388) & 0x06)==0) {
    opl_write(0x105,0x01);
    opl3 = 1;
  }

}

void opl_cleanup(){
  //Cycle through all registers
  //and set them to 0!
  int c;
  for (c = 0x01; c <= 0xF5; c++) {
    opl_write(c,0);
    //Don't forget the OPL3 registers!
    opl_write(0x100|c,0);
  }

}

void play_next_note(){
  int ch;
  int note;
  int pattern;
  int play_bd;
  int play_sd;
  int play_hh;
  int play_tt;
  int play_cm;
  unsigned char drumdata;

  play_bd = 0;
  play_sd = 0;
  play_hh = 0;
  play_tt = 0;
  play_cm = 0;
  drumdata = 0x0;

  if (sequence_counter < 0){
    return;
  }
  opl_stop_percussion_notes();
  for (ch = 0; ch < CHANNEL_COUNT; ch++){
    if (!currentSong.instruments[ch].active){
      continue;
    }
    pattern = currentSong.sequence[ch][sequence_counter];
    if (pattern == -1){
      continue;
    }
    note = currentSong.patterns[pattern][note_counter];
    if (ch >= 6){
      //percussion occupies 6, 7 and 8 in opl, in tracker, additional 2 tracks to support all sounds at once in sequencer
      
      if (note == -1 || note == -2){
        continue;
      }
      switch (note){
        case HH:
          play_hh = 1;
          break;
        case SD:
          play_sd = 1;
          break;
        case BD:
          play_bd = 1;
          break;
        case CM:
          play_cm = 1;
          break;
        case TT:
          play_tt = 1;
      }
      
      continue;
    }

    if (note == -1){
      //stop previous note
      opl_stop_note(ch);
    } else if (note == -2){
      //let previous note sustain
      continue;
    } else {
      opl_stop_note(ch);
      opl_play_note_fullscale(ch, note);
    }
  }

  if (play_hh || play_sd || play_bd || play_cm || play_tt){
    //need to play one or more drum notes
    drumdata = 0x20;
    if (play_bd){
      drumdata = drumdata | 0x10;
    }
    if (play_hh){
      drumdata = drumdata | 0x01;
    }
    if (play_sd){
      drumdata = drumdata | 0x08;
    }
    if (play_tt){
      drumdata = drumdata | 0x04;
    }
    if (play_cm){
      drumdata = drumdata | 0x02; 
    }
    opl_write(0xBD, 0x20);
    opl_write(0xBD, drumdata);
  }
  
  if (note_counter == NOTES_PER_PATTERN - 1){
    note_counter = 0;
    if (sequence_counter == currentSong.sequence_count - 1){
      sequence_counter = 0; //end of song, loop
    } else {
      sequence_counter++;
    }
  } else {
    note_counter++;
  }
}

void set_tempo(unsigned int bpm, unsigned int resolution){
  double notes_per_second;
  music_bpm = bpm; //beats per minute
  music_resolution = resolution; //4 notes per beat

  //calc beats per second:
  notes_per_second = (bpm / 60.0) * resolution;
  music_note_duration = 60 / notes_per_second;

  //calc gap between notes as ms
  // ((music_bpm * 1000) / 60) = a beat every N ms
  // divide by resolution, for note every N ms
  //music_note_duration = (((music_bpm * 1000) / 60) / music_resolution) / (1000 / 60.0) ; //gap between playing notes
  next_note_tick = ticks60 + music_note_duration;
}

void set_instrument(int channel, OPLInstrument instrument){
  opl_set_modulator(channel, instrument.mod_60, instrument.mod_80, instrument.mod_40, instrument.mod_E0);
  opl_set_carrier(channel, instrument.car_60, instrument.car_80, instrument.car_40, instrument.car_E0);
  opl_set_modulator_20h(channel, instrument.mod_20);
  opl_set_carrier_20h(channel, instrument.car_20);
}

void music_timer(){
  if (!opl2 || sequence_counter == -1){
    return;
  }
  if (ticks60 >= next_note_tick){
    play_next_note();
    next_note_tick = ticks60 + music_note_duration;
  }
}

void play_song(Song song){
  int ch;
  currentSong = song;
  sequence_counter = 0;
  note_counter = 0;
  set_tempo(song.tempo, 4);

  //set instruments
  for (ch = 0; ch < 6; ch++){
    if (song.instruments[ch].active){
      set_instrument(ch, song.instruments[ch]);
    }
  }

  opl_init_percussion();

}

void play_battle_song(){
  play_song(battle_song);
}
void play_menu_song(){
  play_song(menu_song);
}

void stop_song(){
  sequence_counter = -1;
  note_counter = -1;
}

