/*
 * delay.c
 *
 * Created: 3/20/2024 11:31:41 AM
 *  Author: GertieMeneer
 */ 
#define F_CPU 8000000UL

#include <util/delay.h>

void wait(int ms) {
	for (int i = 0; i < ms; i++) {
		_delay_ms(1);
	}
}
