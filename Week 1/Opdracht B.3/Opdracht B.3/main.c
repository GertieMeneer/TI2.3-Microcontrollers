#include <avr/io.h>
#include <util/delay.h>

int main(void){
    DDRD |= _BV(DDD7); // Set PORTD.7 as output
    DDRC &= ~_BV(DDC0); // Clear PORTC.0; set as input
    
    while(1){
        if ((PINC & _BV(PC0))){ // Check if button is pressed
            while(1){
                PORTD ^= _BV(PD7); // Toggle LED on PORTD.7
                _delay_ms(500);   // Delay for half second
				if (!(PINC & _BV(PC0))) break;
            }
        } else {
            PORTD &= ~_BV(PD7); // Turn off LED on PORTD.7
            _delay_ms(1);       // Minimal delay
        }
    }
}