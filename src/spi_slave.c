#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <spi_slave.h>

// ---------------------------------------------------------------- Definitions

#define SPI_DDR DDRB
#define PIN_MISO PB3

// --------------------------------------------------------------------- Memory

static volatile uint8_t *_txbuffer;
static volatile uint8_t _txbufend;
static volatile uint8_t _txbufpos;
static volatile void (*_txstart_handler)();
static volatile void (*_txdone_handler)();

// --------------------------------------------------------- External Functions

void spi_slave_init(void) {

	// Configure SPI pins
	SPI_DDR = _BV(PIN_MISO);

	// Initialize tx data
	SPDR = 0xFF;

	// Initialize internal variables
	_txbuffer = NULL;
	_txbufend = 0;
	_txbufpos = 0;
	_txdone_handler = NULL;

	// Enable controller and interrupts
	SPCR = _BV(SPE) | _BV(SPIE);
}

void spi_slave_register_txbuffer(uint8_t *txbuffer, uint8_t txbuflen) {

	// Register the buffer
	_txbuffer = txbuffer;
	_txbufend = txbuflen-1;
	_txbufpos = 0;

	// Copy first byte to controller
	SPDR = *_txbuffer;
}

void spi_slave_register_txstart_handler(void (*txstart_handler)()) {

	// Register handler
	_txstart_handler = txstart_handler;
}

void spi_slave_register_txdone_handler(void (*txdone_handler)()) {

	// Register handler
	_txdone_handler = txdone_handler;
}

// ------------------------------------------------- Interrupt Service Routines

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