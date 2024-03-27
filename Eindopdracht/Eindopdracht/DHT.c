#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

#define DHT_PIN PF3

void send_start_signal() {
	// Set DHT_PIN as output
	DDRF |= (1 << DHT_PIN);

	// Pull DHT_PIN low for at least 18ms
	PORTF &= ~(1 << DHT_PIN);
	_delay_ms(20);

	// Pull DHT_PIN high and wait 20-40us
	PORTF |= (1 << DHT_PIN);
	_delay_us(40);  // Wait for DHT response
}

int read_response_signal() {
	// Set DHT_PIN as input
	DDRF &= ~(1 << DHT_PIN);

	_delay_us(80);

	// Check if DHT responded (DHT_PIN should be low)
	if (PINF & (1 << DHT_PIN)){ 
		return 0;  // No response
	}
	else{
		return 1;  // Response received
	}
}

int read_bit() {
	// Wait for start of bit (low voltage)
	_delay_us(50);

	// Read the bit
	_delay_us(30);
	if (PINF & (1 << DHT_PIN))
	return 1;  // Bit is 1
	else
	return 0;  // Bit is 0
}

int read_dht_data(uint8_t* data) {
	send_start_signal();
	if (!read_response_signal())
		return 0;  // No response from DHT11
		

	// Read 40 bits of data
	for (int i = 0; i < 5; i++) {
		uint8_t byte = 0;
		for (int j = 0; j < 8; j++) {
			byte <<= 1;
			byte |= read_bit();
		}
		data[i] = byte;
	}

	// Verify checksum
	uint8_t checksum = data[0] + data[1] + data[2] + data[3];
	if (checksum != data[4])
	return 0;  // Checksum error

	return 1;  // Data read successfully
}

//int main(void) {
//
	////Byte 0: Integral Relative Humidity (RH) data (8 bits)
	////Byte 1: Decimal Relative Humidity (RH) data (8 bits)
	////Byte 2: Integral Temperature (T) data (8 bits)
	////Byte 3: Decimal Temperature (T) data (8 bits)
	////Byte 4: Checksum (8 bits)
	//
	//
//
	//
	//DDRD = 0xff;
//
	//while (1) {
		//if (read_dht_data(sensor_data)) {
		//PORTD = 0xff;
		//
		//}
		//else {
						//
//
		//}
//
		//_delay_ms(2000);
	//}
//
	//return 0;
//}