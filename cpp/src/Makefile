IDIR = ../include
CXX=g++
CXXFLAGS=-I$(IDIR) -lmosquittopp

ODIR=obj
LDIR=../lib

LIBS=

_DEPS = DS18B20.h ArduiPi_OLED.h Adafruit_GFX.h bcm2835.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = ha.o DS18B20.o ArduiPi_OLED.o Adafruit_GFX.o bcm2835.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

ha: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

# create object files based on cpp files
$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# create object files based on c files
$(ODIR)/%.o: %.c $(DEPS)
	gcc -c -o $@ $< $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
