CFLAGS=-c -Wall -O2 
LIBS = -lwiringPi 

all: relay 

relay: relay.o 
	$(CC) relay.o $(LIBS) -o relay 

relay.o: relay.c 
	$(CC) $(CFLAGS) relay.c

clean:
	rm -rf *o relay
