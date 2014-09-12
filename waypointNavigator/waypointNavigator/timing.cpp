// Chris Barr
//Note: Quite heavily inspired by the Arduino millis() and micros() code.
#include "timing.h"


volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

long lastIRTime = 0;


ISR(TIMER0_OVF_vect)
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}

	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}

long  millis()
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	m = timer0_millis;
	SREG = oldSREG;
	sei();

	return m;
}

long  micros() {

	unsigned long m;
	uint8_t oldSREG = SREG, t;
	
	cli();
	m = timer0_overflow_count;
	t = TCNT0;
  
#ifdef TIFR0
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
		m++;
#else
	if ((TIFR & _BV(TOV0)) && (t < 255))
		m++;
#endif

	SREG = oldSREG;
	
	sei();
	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}


void initTimers()
{
	// enable timer overflow interrupt for Timer0
	TIMSK0=(1<<TOIE0);
	
	//set timer to zero
	TCNT0=0x00;

	// start timer0 with /1024 prescaler
	TCCR0B = (1<<CS01) | (1<<CS00);
}
