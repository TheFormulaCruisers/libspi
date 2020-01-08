#include <avr/io.h>
#include <avr/interrupt.h>
#include <spi_slave.h>

volatile uint8_t dat1[] = {1,2,4,8,16,32,64,128};
volatile uint8_t dat2[] = {128,64,32,16,8,4,2,1};
volatile uint8_t current_buffer;

void spi_swap_buffer() {
	if (current_buffer == 1) {
		spi_slave_register_txbuffer((void *)dat2, 8);
		current_buffer = 2;
	} else {
		spi_slave_register_txbuffer((void *)dat1, 8);
		current_buffer = 1;
	}
}

int main(void) {

	spi_slave_init();
	spi_slave_register_txbuffer((void *)dat1, 8);
	spi_slave_register_txdone_handler(spi_swap_buffer);
	spi_slave_enable();
	
	sei();

	while(1);
	return 0;
}