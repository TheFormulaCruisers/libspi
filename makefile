#
# Parameters
#===========

APPNAME     = spi
CXX         = avr-gcc
CXXFLAGS    = -Wall
LIBS        = -Iinc
DEVICE		= at90can128
FCPU		= 16000000
OFILES		= main.o spi_slave.o

#
# Make
#===========

main: $(OFILES)
	$(CXX) $(CXXFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) -o bin/$(APPNAME).elf $(OFILES)

main.o: src/main.c
	$(CXX) $(CXXFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) -c src/main.c $(LIBS)

spi.o: src/spi_slave.c
	$(CXX) $(CXXFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) -c src/spi_slave.c $(LIBS)

clean:
	rm *.o
