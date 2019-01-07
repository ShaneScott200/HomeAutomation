#include <stdio.h>
#include <time.h>
#include <string>

#include <wiringPi.h>
#include <pcf8574.h>

using namespace std;

int main (int argc, char *argv[])
{
	printf("Raspberry Pi\n");
	printf("http://geek.adachsoft.com/\n");

	wiringPiSetup();
	pcf8574Setup(100,0x20);

	for (int i=0; i < 8; i++)
	{
		pinMode(100 + i, INPUT);
	}

	int b = 0;
	int v = 0;
	while (1==1)
	{
		printf("LOOP %u\n", b);
		printf("\tVALUE:\t");
		for (int i = 0; i < 8; i++)
		{
			digitalWrite(100 + i, i==b ? 0 : 1);
			delay(100);
			v = digitalRead(100 + i);
			printf("%u", v);
			printf("\t");
		}
		printf("\n");
		b++;
		if (b >= 8) b = 0;
		delay (1000);
	}

	delay (1000);
	digitalWrite(100 + 0, 0);

	delay (1000);
	digitalWrite(100 + 0, 1);

	return 0;
}
