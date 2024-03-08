#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>



#define BIT(x) (1 << (x))
#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define SPI_SCK 1
#define SPI_MOSI 2
#define SPI_MISO 3
#define SPI_SS 0

void wait(int ms) {
	for (int i = 0; i < ms; i++) {
		_delay_ms(1);
	}
}

void spi_masterInit(void) {	
	DDR_SPI = 0xff;
	DDR_SPI &= ~BIT(SPI_MISO);
	PORT_SPI |= BIT(SPI_SS);
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);
}

void spi_write(unsigned char data) {
	SPDR = data;
	while (!(SPSR & BIT(SPIF)));
}

void spi_slaveSelect(unsigned char chipNumber) {
	PORTB &= ~BIT(chipNumber);
}

void spi_slaveDeSelect(unsigned char chipNumber) {
	PORTB |= BIT(chipNumber);
}

void displayDriverInit() {
	spi_slaveSelect(0);
	spi_write(0x09);
	spi_write(0xFF);
	spi_slaveDeSelect(0);

	spi_slaveSelect(0);
	spi_write(0x0A);
	spi_write(0x0F); // Max intensity
	spi_slaveDeSelect(0);

	spi_slaveSelect(0);
	spi_write(0x0B);
	spi_write(0x03); // Display digits 0..3
	spi_slaveDeSelect(0);

	spi_slaveSelect(0);
	spi_write(0x0C);
	spi_write(0x01);
	spi_slaveDeSelect(0);
}

void spi_writeWord(unsigned char address, unsigned char data) {
	spi_slaveSelect(0);
	spi_write(address);            // Write address byte
	spi_write(data);               // Write data byte
	spi_slaveDeSelect(0);          // Deselect display chip
}


int main() {
	DDRB = 0x01;
	spi_masterInit();
	displayDriverInit();

	// Clear display
	for (char i = 1; i <= 4; i++) {
		spi_writeWord(i, 0);
	}
	wait(1000);

	// Write 1-2-3-4 to all four digits
	for (char i = 1; i <= 4; i++) {
		spi_writeWord(i, i);
		wait(1000);
	}

	return 1;
}
