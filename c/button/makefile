CFLAGS=-c -Wall -O2 
LIBS = -lwiringPi

all: button 

button: main.o 
	$(CC) main.o $(LIBS) -o button 

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *o button
