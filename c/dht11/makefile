CFLAGS=-c -Wall -O2 
LIBS = -lwiringPi

all: dht 

dht: main.o 
	$(CC) main.o $(LIBS) -o dht 

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *o dht
