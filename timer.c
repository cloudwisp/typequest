#include <dos.h>
#include "timer.h"

void interrupt (*oldTimer)(void);
volatile unsigned long ticks60;
unsigned accumulator;

// Timer setup - update at 60hz

void setPITRate(unsigned hz) {
   unsigned divisor = 1193180UL / hz;

   outp(0x43, 0x36); // binary, mode 3, LSB/MSB, channel 0
   outp(0x40, divisor & 0xFF); // LSB
   outp(0x40, divisor >> 8); // MSB
}


void interrupt newTimer(void) {
    ticks60++;

    // maintain BIOS tick count (18.2 Hz), rougly every 3 ticks of the 60hz timer
    accumulator += 60;
    if (accumulator >= 182) {
        accumulator -= 182;
        oldTimer(); // call original BIOS handler
        music_timer(); //todo: move to 60hz.
    } else {
      outp(0x20, 0x20); // send EOI to PIC
    }
}

void init_timer(void) {
    ticks60 = 0;
    accumulator = 0;
    oldTimer = getvect(8);
    disable();
    setvect(8, newTimer);
    setPITRate(60);
    enable();
}

void remove_timer(void) {
    disable();
    setvect(8, oldTimer);
    setPITRate(18);   // restore BIOS rate
    enable();
}
