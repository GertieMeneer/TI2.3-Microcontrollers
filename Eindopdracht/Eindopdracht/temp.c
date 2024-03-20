/*
 * temp.c
 *
 * Created: 3/20/2024 11:43:42 AM
 *  Author: GertieMeneer
 */ 

#include <avr/io.h>

void adcInit(void) {
	ADMUX = 0b01100000;	// aref=vcc, result left adjusted, channel 0 at pin PF0
	ADCSRA = 0b11100110;	// adc-enable, no interrupt, start, free running
}