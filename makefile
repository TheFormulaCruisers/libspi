#
# Parameters
#===========

APPNAME     = spi
CXX         = avr-gcc
CXXFLAGS    = -Wall
LIBS        = -Iinc
DEVICE		= at90can128
FCPU		= 16000000
OFILES		= test.o spi_slave.o

#
# Make
#===========

main: $(OFILES)
	$(CXX) $(CXXFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) -o bin/$(APPNAME) $(OFILES)

test.o: src/test.c
	$(CXX) $(CXXFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) -c src/test.c $(LIBS)

spi_slave.o: src/spi_slave.c
	$(CXX) $(CXXFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) -c src/spi_slave.c $(LIBS)

clean:
	rm *.o
