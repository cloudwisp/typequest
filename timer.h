#ifndef TIMER_H
#define TIMER_H

extern volatile unsigned long ticks60;

void init_timer(void);
    
void remove_timer(void);

void music_timer(void); //implement in music.c not timer.c

#endif