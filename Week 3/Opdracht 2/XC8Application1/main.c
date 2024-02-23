#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t pulse_high = 1; // Indicator voor hoog- en laagperiode van de puls

// Functie om Timer/Counter 2 te initialiseren voor CTC-modus
void timer2_init() {
	// Zet Timer/Counter 2 in CTC-modus (WGM21:0 = 2)
	TCCR2 |= (1 << WGM21);
	
	// Stel prescaler in op 1024 (CS22:0 = 5)
	TCCR2 |= (1 << CS22) | (1 << CS21);
	
	// Bereken en stel de waarde van OCR2 in voor een periode van 15 ms
	OCR2 = 146; // Bij prescaler 1024, 15 ms => 146 pulsen
	
	// Schakel interrupt voor compare match A in
	TIMSK |= (1 << OCIE2);
}

// Interrupt Service Routine voor Timer/Counter 2 Compare Match A
ISR(TIMER2_COMPA_vect) {
	if (pulse_high) {
		// Zet PORTD.7 (PD7) hoog
		PORTD |= (1 << PD7);
		
		// Schakel naar laagpuls over na 15 ms
		OCR2 = 244; // Bij prescaler 1024, 25 ms => 244 pulsen
		pulse_high = 0;
		} else {
		// Zet PORTD.7 (PD7) laag
		PORTD &= ~(1 << PD7);
		
		// Schakel terug naar hoogpuls na 25 ms
		OCR2 = 146; // Bij prescaler 1024, 15 ms => 146 pulsen
		pulse_high = 1;
	}
}

int main() {
	// Zet PORTD.7 (PD7) en PD6 (extra LED) als uitgang
	DDRD |= (1 << PD7) | (1 << PD6);

	// Initialisatie van Timer/Counter 2
	timer2_init();

	// Schakel globale interrupts in
	sei();

	// Blijf in een oneindige lus
	while (1) {
		// Laat de extra LED (PD6) knipperen om te bevestigen dat het programma werkt
		PORTD ^= (1 << PD6); // Toggle PD6
		_delay_ms(500); // Wacht 500 ms
	}

	return 0;
}
