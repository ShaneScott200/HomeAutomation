/*************************************************************************************

This is the home automation control panel using a Raspberry Pi

DATE		BY	COMMENTS
2018-04-14	SS	Initial Write


*************************************************************************************/
// ---------- CONFIGURE STANDARD LIBRARIES -----------
#include <iostream>
using namespace std;

// ---------- CONFIGURE OLED ----------
#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

// ---------- CONFIGURE DS18B20 ----------
#include "DS18B20.h"
char w1_address[16] = "28-041752e4cdff";

// ---------- CONFIGURE MQTT ----------
#include </usr/include/mosquittopp.h>


//#include <getopt.h>

#define PRG_NAME  "ha"
#define PRG_VERSION "1.0"

// Instantiate the display
ArduiPi_OLED display;

// Config options
struct s_opts
{
	int oled;
	int verbose;
} ;

int sleep_divisor = 1;

// default options values
s_opts opts = {
	OLED_ADAFRUIT_I2C_128x32, 	// Default oled
	false				// Not verbose
};

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH 16

/* ======================================================================
Function: main
Purpose : Main entry Point
Input   : -
Output  : -
Comments:
====================================================================== */
int main(int argc, char **argv)
{
  printf("Starting...\n");
  if ( !display.init(OLED_I2C_RESET,opts.oled) )
      exit(EXIT_FAILURE);

  // prepare DS18B20 
  printf("Prepare DS18B20...\n");
  double tempNow;
  char *msg_temp;

  DS18B20 w1Device1 (w1_address);
  tempNow = w1Device1.getTemp();
  cout << "Configuring DS18B20 with address: " << w1_address << endl;

  printf("Beginning display work!\n");
  display.begin();

  printf("Clear display\n");
  
  while(1) {
  //for (int i = 65; i < 75; i++) {
    tempNow = w1Device1.getTemp();
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("TEMP\n");
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.printf("%f\n", tempNow);
    display.display();
    sleep(1);
  
  
    mosqpp::mosquittopp* m = new mosqpp::mosquittopp("Test", false);
//    mosqpp::mosquittopp* m = new mosqpp::mosquittopp("Test", false);
/*    m->connect("192.168.0.200", 1883, 60);
    char resultstring[32];
    sprintf(resultstring, "%ld", tempNow);
    m->publish(NULL, "time", 32, resultstring, 0, false);
*/

  }

  printf("THE END!\n");

  return 0;

}



























