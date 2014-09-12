/*
 * timing.h
 *
 * Created: 5/09/2014 7:21:32 PM
 *  Author: Chris
 */ 

//NOTE:
//At the moment, this code is heavily lifted from the Arduino "Wiring" library.
//I've removed most of the code unrelated to the timing functionality we need,
//but there's probably still more tidying up to do here. Currently, everything
//compiles and it works like it should, so for now I'm leaving it alone.
#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

long millis(void);
long micros(void);

void delay_ms( uint16_t milliseconds);
void initTimers();

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (256*64/clockCyclesPerMicrosecond())//(256*(64/(clockCyclesPerMicrosecond()*10)))//(256*4)

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)


