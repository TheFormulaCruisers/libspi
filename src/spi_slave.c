#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <spi_slave.h>

static volatile uint8_t *_txbuffer;
static volatile uint8_t _txbufend;
static volatile uint8_t _txbufpos;
static volatile void (*_txstart_handler)();
static volatile void (*_txdone_handler)();

void spi_slave_init(void) {

	// Disable SPI controller
	SPCR = 0x00;

	// Configure SPI pins
	SPI_DDR = _BV(SPI_MISO);

	// Initialize tx data
	SPDR = 0xFF;

	// Initialize internal variables
	_txbuffer = NULL;
	_txbufend = 0;
	_txbufpos = 0;
	_txdone_handler = NULL;
}

void spi_slave_register_txbuffer(uint8_t *txbuffer, uint8_t txbuflen) {

	// Disable interrupts
	SPCR &= ~_BV(SPIE);

	// Register the buffer
	_txbuffer = txbuffer;
	_txbufend = txbuflen-1;
	_txbufpos = 0;

	// Copy first byte to controller
	SPDR = *_txbuffer;
	
	// Enable interrupts
	SPCR |= _BV(SPIE);
}

void spi_slave_register_txstart_handler(void (*txstart_handler)()) {

	// Disable interrupts
	SPCR &= ~_BV(SPIE);

	// Register the handler
	_txstart_handler = txstart_handler;
	
	// Enable interrupts
	SPCR |= _BV(SPIE);
}

void spi_slave_register_txdone_handler(void (*txdone_handler)()) {

	// Disable interrupts
	SPCR &= ~_BV(SPIE);

	// Register the handler
	_txdone_handler = txdone_handler;
	
	// Enable interrupts
	SPCR |= _BV(SPIE);
}

void spi_slave_enable(void) {

	// Enable controller
	SPCR |= _BV(SPE);
}

ISR(SPI_STC_vect) {

	if (_txbufpos == 0) {
		
		// Call tx start handler
		if (_txstart_handler != NULL) {
			(*_txstart_handler)();
		}
	}

	if (_txbufpos < _txbufend) {

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
		SPDR = *(_txbuffer+_txbufpos);
	}
}