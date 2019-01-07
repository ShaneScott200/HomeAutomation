#include <wiringPi.h>
#include <stdio.h>

#define SensorPin 0
#define LedOutput 1

void INOUT(char* ledstate)
{
	pinMode(LedOutput, OUTPUT);
	if (ledstate == "OUTPUT")
	{
		digitalWrite(LedOutput, HIGH);
	}
	else if (ledstate == "INPUT")
	{
		digitalWrite(LedOutput, LOW);
	}
	else
		printf("Error Led Output");
}

int main(void)
{
	if(wiringPiSetup() == -1) 	//If no response from wiringpi.h  print 
	{
		printf("Wiring Pi Initialization Faild");
		return 1;
	}

	pinMode(SensorPin, INPUT);
	INOUT("INPUT");

	while(1)
	{
		if(0 == digitalRead(SensorPin))
		{
			delay(10);
			if(0 == digitalRead(SensorPin))
			{
				INOUT("OUTPUT");
//				printf("Button is pressed\n");
			}
		}
		else if(1 == digitalRead(SensorPin))
		{
			delay(10);
			if(1 == digitalRead(SensorPin))
			{
				while(!digitalRead(SensorPin));
				INOUT("INPUT");
			}
		}
	}
	
	return 0;
}
