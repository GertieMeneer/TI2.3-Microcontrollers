#include <avr/io.h>
#include <avr/interrupt.h>

#define PRESCALER_VALUE 1024

#define HIGH_DURATION 389
#define LOW_DURATION 233


void setup_timer();
void setup_io();

int main() {
	setup_timer(); //init timer
	setup_io();    //init i/0
	
	//enable global interrupts
	sei();

	while (1) {
	}

	return 0;
}

void setup_timer() {
	//set timer/counter 2 to ctc mode
	TCCR2 = (1 << WGM21);
	
	TCCR2 |= (1 << CS22) | (1 << CS20);

	OCR2 = HIGH_DURATION;

	//enable timer/counter 2 ocm interrupt
	TIMSK |= (1 << OCIE2);
}

void setup_io() {
	//portd 7 output
	DDRD |= (1 << PD7);

	//portd 7 low so tart
	PORTD &= ~(1 << PD7);
}

//timer/counter 2 ocm isr
ISR(TIMER2_COMP_vect) {
	PORTD ^= (1 << PD7);

	if (PORTD & (1 << PD7)) {
		OCR2 = HIGH_DURATION;
		} else {
		OCR2 = LOW_DURATION;
	}
}
