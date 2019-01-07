CFLAGS=-c -Wall -O2 
LIBS = -loled96 

all: ha 

ha: main.o 
	$(CC) main.o $(LIBS) -o ha 

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *o ha
