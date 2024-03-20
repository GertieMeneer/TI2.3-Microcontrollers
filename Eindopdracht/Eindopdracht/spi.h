/*
 * spi.h
 *
 * Created: 3/20/2024 11:33:49 AM
 *  Author: GertieMeneer
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>


//init spi as master
void spi_masterInit(void);

//write to spi
void spi_write(unsigned char data);

//select slave
void spi_slaveSelect(unsigned char chipNumber);

//unselect slave
void spi_slaveDeSelect(unsigned char chipNumber);

//init display
void displayDriverInit();

//default commands for writing data
void spi_writeWord(unsigned char address, unsigned char data);

//for writing '-' on specific digit
void writeCharacterMinusOnDisplay(unsigned char digitDisplay);

//write a value to the led display
void writeLedDisplay(int32_t value);

#endif /* SPI_H_ */