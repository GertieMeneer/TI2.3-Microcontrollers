#ifndef DHT11_H_
#define DHT11_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

//Timer 0 is used for calculations
//define prescale for timer 0
#define DHT11_SETTPRESCALE	TCCR0B|=(1<<CS01)|(1<<CS00)
#define DHT11_TCNT	TCNT0

//Timer tick, us = 1/F_CPU*1.000.000*TIMER_PRESCALE
//For F_CPU 16MHz and PreScale 64, Timer increment every 4 us
#define DHT11_T0_INC	4

#define DHT11_DDR		DDRD		//port direction
#define DHT11_PORT		PORTD		//port for writing
#define DHT11_PIN		PIND		//port for reading
#define DHT11_PINX		PD2			//pin for communication

void DHT11_Init();
void DHT11_GetData(char* cHumInt, char* cHumDec, char* cTempInt, char* cTempDec);

static char DHT11_GetBit();

#endif /* DHT11_H_ */