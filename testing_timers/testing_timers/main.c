#define F_CPU 8000000UL   // Klokfrequentie van 8 MHz
#define BAUD 9600         // Baudrate
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/delay.h>



volatile uint16_t timer_counter = 0;

void uart_init(unsigned int ubrr) {
	// Stel baudrate in
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	
	// Schakel ontvanger en zender in
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	
	// Stel frameformaat in: 8 data bits, 1 stopbit
	UCSR0C = (3 << UCSZ00);
}

void uart_transmit(unsigned char data) {
	// Wacht op lege zendbuffer
	while (!(UCSR0A & (1 << UDRE0)));
	
	// Plaats gegevens in buffer, verzend de gegevens
	UDR0 = data;
}

void timer1_init() {
	// Stel Timer/Counter1 in op CTC-modus met prescaler 64
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);
	
	// Stel de vergelijkingswaarde (OCR1A) in voor de gewenste timeout
	OCR1A = 12499;  // Voor een time-out van 1 seconde bij een klok van 8 MHz en prescaler 64
	
	// Schakel Timer/Counter1 Compare Match A interrupt in
	TIMSK |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
	// Verhoog de timer-teller variabele
	timer_counter++;
}

int main(void) {
	// Initialiseer UART
	uart_init(MYUBRR);
	
	// Initialiseer Timer/Counter1
	timer1_init();
	
	// Schakel globale interrupts in
	sei();
	
	while (1) {
		// Druk de waarde van timer_counter af via UART
		char buffer[20];
		sprintf(buffer, "Timer Counter: %u\n", timer_counter);
		for (int i = 0; buffer[i] != '\0'; i++) {
			uart_transmit(buffer[i]);
		}
		
		// Vertraging voor leesbaarheid (optioneel)
		_delay_ms(1000); // Vereist <util/delay.h>
	}
	
	return 0;
}
