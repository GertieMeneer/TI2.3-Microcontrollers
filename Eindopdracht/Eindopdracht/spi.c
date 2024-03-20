/*
 * spi.c
 *
 * Created: 3/20/2024 11:32:38 AM
 *  Author: GertieMeneer
 */
#include <avr/io.h>

#define BIT(x) (1 << (x))
#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define SPI_SCK 1
#define SPI_MOSI 2
#define SPI_MISO 3
#define SPI_SS 0

//init spi as master
void spi_masterInit(void) {
	DDR_SPI = 0xff;
	DDR_SPI &= ~BIT(SPI_MISO);
	PORT_SPI |= BIT(SPI_SS);
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);
}

//write to spi
void spi_write(unsigned char data) {
	SPDR = data;
	while (!(SPSR & BIT(SPIF)));
}

//select slave
void spi_slaveSelect(unsigned char chipNumber) {
	PORTB &= ~BIT(chipNumber);
}

//unselect slave
void spi_slaveDeSelect(unsigned char chipNumber) {
	PORTB |= BIT(chipNumber);
}

//init display
void displayDriverInit() {
	spi_slaveSelect(0);
	spi_write(0x09);
	spi_write(0xFF);
	spi_slaveDeSelect(0);

	spi_slaveSelect(0);
	spi_write(0x0A);
	spi_write(0x0F); //brightness, max intensity
	spi_slaveDeSelect(0);

	spi_slaveSelect(0);
	spi_write(0x0B);
	spi_write(0x03); //all display digits
	spi_slaveDeSelect(0);

	spi_slaveSelect(0);
	spi_write(0x0C);
	spi_write(0x01);
	spi_slaveDeSelect(0);
	
	//^^^ these 4 blocks can be replaced with spi_writeWord,
	//but to show the brightness and selecting all digits
	//we leave them like this.
}


//default commands for writing data
void spi_writeWord(unsigned char digit, unsigned char value, unsigned char dot) {
	// Selecteer de SPI-slave (display chip)
	spi_slaveSelect(0);

	// Schrijf het adresbyte (digit-nummer) naar de display chip
	spi_write(digit);

	// Construeer het databyte met de waarde van het cijfer en het dot-bit
	unsigned char dataToSend = value & 0x0F;
	if (dot) {
		dataToSend |= 0x80; // Als dot aan is, zet het dot-bit
	}

	// Schrijf het databyte naar de display chip
	spi_write(dataToSend);

	// Deselecteer de display chip
	spi_slaveDeSelect(0);
}



//for writing '-' on specific digit
void writeCharacterMinusOnDisplay(unsigned char digitDisplay)
{
	spi_writeWord(digitDisplay, 10, 0);
}

//write a value to the led display
void writeLedDisplay(int32_t value) {
	uint8_t digit1, digit2, digit3, digit4;
	int negative = 0;

	if (value < 0) {
		negative = 1;
		value = -value; //get absolute value of number
	}

	digit4 = value / 1000;
	digit3 = (value / 100) % 10;
	digit2 = (value / 10) % 10;
	digit1 = value % 10;

	spi_writeWord(1, digit1, 0);
	spi_writeWord(2, digit2, 1); // Write digit 2 with dot turned on
	spi_writeWord(3, digit3, 0);
	spi_writeWord(4, digit4, 0);
	
	if (negative) {
		writeCharacterMinusOnDisplay(4); //only write '-' if negative number
	}
}

void clearDisplay(void) {
	for (char i = 1; i <= 4; i++) {
		spi_writeWord(i, 0, 0);
	}
}