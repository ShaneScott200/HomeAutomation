/*
14CORE Test Code for: Single Channel Relay
https://www.14core.com/wiring-single-channel-relay-with-raspberry-pi-on-wiringpi-python/

*/

#include <wiringPi.h>
#include <stdio.h>

#define RelayPn1 7	// GPIO 4
#define RelayPn2 0	// GPIO 17
#define RelayPn3 2	// GPIO 21
#define RelayPn4 3	// GPIO 22
#define RelayPn5 4	// GPIO 23


int main(void)
{
	if(wiringPiSetup() == -1){ //If wiringpi initialization failed, printf 
		printf("Wiring Pi Initialization Faild");
		return 1;
	}
	
	pinMode(RelayPn1, OUTPUT);
	pinMode(RelayPn2, OUTPUT);
	pinMode(RelayPn3, OUTPUT);
	pinMode(RelayPn4, OUTPUT);

	printf ("\nStart toggling the output\n");

	while(1){
		digitalWrite(RelayPn1, LOW);
		delay(200);
		digitalWrite(RelayPn2, LOW);
		delay(200);
		digitalWrite(RelayPn3, LOW);
		delay(200);
		digitalWrite(RelayPn4, LOW);
		delay(200);
		digitalWrite(RelayPn5, LOW);
		delay(200);
		digitalWrite(RelayPn1, HIGH);
		delay(200);
		digitalWrite(RelayPn2, HIGH);
		delay(200);
		digitalWrite(RelayPn3, HIGH);
		delay(200);	
		digitalWrite(RelayPn4, HIGH);
		delay(200);
		digitalWrite(RelayPn5, HIGH);
		delay(200);

	}

	return 0;
}
