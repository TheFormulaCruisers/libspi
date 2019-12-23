#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <spi_slave.h>

static volatile uint8_t *_txbuffer;
static volatile uint8_t _txbuflen;
static volatile uint8_t _txbufpos;
static volatile void (*_txdone_handler)();

void spi_slave_init(void) {

	// Initialize tx data
	SPDR = 0xFF;

	// Initialize internal variables
	_txbuffer = NULL;
	_txbuflen = 0;
	_txbufpos = 0;
	_txdone_handler = NULL;
}

void spi_slave_register_txbuffer(uint8_t *txbuffer, uint8_t txbuflen) {

	// Disable interrupts
	SPCR &= ~_BV(SPIE);

	// Register the buffer
	_txbuffer = txbuffer;
	_txbuflen = txbuflen;
	_txbufpos = 0;

	// Copy first byte to controller
	SPDR = *_txbuffer;
}

void spi_slave_register_txdone_handler(void (*txdone_handler)()) {

	// Disable interrupts
	SPCR &= ~_BV(SPIE);

	// Register the handler
	_txdone_handler = txdone_handler;
}

void spi_slave_enable(void) {

	// Enable interrupts and controller
	SPCR = _BV(SPIE) | _BV(SPE);
}

ISR(SPI_STC_vect) {

	if (_txbufpos < _txbuflen-1) {

		// Increment buffer position
		_txbufpos++;

	} else {

		// Reset buffer position
		_txbufpos = 0;

		// Call tx done handler
		if (_txdone_handler != NULL) {
			(*_txdone_handler)();
		}
	}

	// Copy next byte from tx buffer to controller
	if (_txbuffer != NULL) {	
		SPDR = (*_txbuffer+_txbufpos);
	}
}