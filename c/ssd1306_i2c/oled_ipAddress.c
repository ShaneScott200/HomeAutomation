/******
Demo for ssd1306 i2c driver for  Raspberry Pi 
******/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "ssd1306_i2c.h"

int main() {

	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);

	ssd1306_display(); //Adafruit logo is visible
	ssd1306_clearDisplay();
	delay(2000);

	int n;
	struct ifreq ifr;
	char array[] = "eth0";

	n = socket(AF_INET, SOCK_DGRAM, 0);
	//Type of address to retrieve - IPv4 IP address
	ifr.ifr_addr.sa_family = AF_INET;
	//Copy the interface name in the ifreq structure
	strncpy(ifr.ifr_name , array , IFNAMSIZ - 1);
	ioctl(n, SIOCGIFADDR, &ifr);
	close(n);
	//display result
	char a[40];
	strcpy(a, inet_ntoa(( (struct sockaddr_in *) & ifr.ifr_addr) -> sin_addr));


	ssd1306_clearDisplay();
	ssd1306_drawString("IP ADDRESS:\n");
	ssd1306_drawString(a);
	ssd1306_drawString("\n");
	ssd1306_drawString("Home Office\n");
	ssd1306_drawString("*** TESTING ***");
	
	ssd1306_display();
	
	delay (10000);	
	ssd1306_dim(1);

	return 0;
}
