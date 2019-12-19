#
# Parameters
#===========

CXX         = avr-gcc
CXXFLAGS    = -Wall
LIBS        = -Iinc
DEVICE		= at90can128
FCPU		= 16000000

#
# Make
#===========

SPI_SLAVE_O = spi_slave_tests.o spi_slave.o
SPI_SLAVE_BIN = spi_slave_tests

spi_slave_build: $(SPI_SLAVE_O)
	$(CXX) $(CXXFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) -o bin/$(SPI_SLAVE_BIN) $(SPI_SLAVE_O)

spi_slave_tests.o: tests/spi_slave_tests.c
	$(CXX) $(CXXFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) -c tests/spi_slave_tests.c $(LIBS)

spi_slave.o: src/spi_slave.c
	$(CXX) $(CXXFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) -c src/spi_slave.c $(LIBS)

clean:
	rm *.o