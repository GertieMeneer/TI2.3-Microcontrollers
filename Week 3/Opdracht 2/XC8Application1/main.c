#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t pulse_high = 1; // Indicator voor hoog- en laagperiode van de puls

// initialiseer timer/counter 2 in CTC-modus
void timer2_init() {
	// zet timer/counter in ctc-modus
	TCCR2 |= (1 << WGM21);
	
	// prescaler op 1024
	TCCR2 |= (1 << CS22) | (1 << CS21);
	
	OCR2 = 146; // prescaler 1024: 15 ms = 146 pulsen
	
	// interrupt voor compare match A
	TIMSK |= (1 << OCIE2);
}

// isr voor timer/counter compare match A
ISR(TIMER2_COMPA_vect) {
	if (pulse_high) {
		// portd 7 hoog
		PORTD |= (1 << PD7);
		
		OCR2 = 244; // prescaler 1024: 25 ms = 244 pulsen
		pulse_high = 0;
		} else {
		// portd 7 laag
		PORTD &= ~(1 << PD7);
		
		// terug naar hoogpuls na 25 ms
		OCR2 = 146; // prescaler 1024: 15 ms = 146 pulsen
		pulse_high = 1;
	}
}

int main() {
	// portd 7 als uitgang
	DDRD |= (1 << PD7);

	timer2_init();

	// global interupts aanzetten
	sei();

	while (1) {
		_delay_ms(1);
	}

	return 0;
}
