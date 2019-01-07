/*************************************************************************************

This is the home automation control panel using a Raspberry Pi

DATE		BY	COMMENTS
2018-04-14	SS	Initial Write


*************************************************************************************/

// ---------- CONFIGURE OLED ----------
#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

// ---------- CONFIGURE DS18B20 ----------
//#include "DS18B20.h"
char w1_address[16] = "28-041752e4cdff";

#include <getopt.h>

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
	OLED_ADAFRUIT_SPI_128x32, 	// Default oled
	false				// Not verbose
};

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH 16

/* ===================================================================
Function: usage
Purpose : display usage
Input   : program name
Output  : -
Comments:
====================================================================== */
void usage( char * name)
{
        printf("%s\n", name );
        printf("Usage is: %s --oled type [options]\n", name);
        printf("  --<o>led type\nOLED type are:\n");
        for (int i=0; i<OLED_LAST_OLED;i++)
                printf("  %1d %s\n", i, oled_type_str[i]);

        printf("Options are:\n");
        printf("  --<v>erbose  : speak more to user\n");
        printf("  --<h>elp\n");
        printf("<?> indicates the equivalent short option.\n");
        printf("Short options are prefixed by \"-\" instead of by \"--\".\n");
        printf("Example :\n");
        printf( "%s -o 1 use a %s OLED\n\n", name, oled_type_str[1]);
        printf( "%s -o 4 -v use a %s OLED being verbose\n", name, oled_type_str[4]);
}


/* =================================================================
Function: parse_args
Purpose : parse argument passed to the program
Input   : -
Output  : -
Comments:
====================================================================== */
void parse_args(int argc, char *argv[])
{
        static struct option longOptions[] =
        {
                {"oled"   , required_argument,0, 'o'},
                {"verbose", no_argument,                0, 'v'},
                {"help"         , no_argument,                  0, 'h'},
                {0, 0, 0, 0}
        };

        int optionIndex = 0;
        int c;

        while (1)
        {
                /* no default error messages printed. */
                opterr = 0;

    c = getopt_long(argc, argv, "vho:", longOptions, &optionIndex);

                if (c < 0)
                        break;

                switch (c)
                {
                        case 'v': opts.verbose = true   ;       break;

                        case 'o':
                               opts.oled = (int) atoi(optarg);

                                if (opts.oled < 0 || opts.oled >= OLED_LAST_OLED )
                                {
                                                fprintf(stderr, "--oled %d ignored must be 0 to %d.\n", opts.oled, OLED_LAST_OLED-1);
                                                fprintf(stderr, "--oled set to 0 now\n");
                                                opts.oled = 0;
                                }
                        break;

                        case 'h':
                                usage(argv[0]);
                                exit(EXIT_SUCCESS);
                        break;

                        case '?':
                        default:
                                fprintf(stderr, "Unrecognized option.\n");
                                fprintf(stderr, "Run with '--help'.\n");
                                exit(EXIT_FAILURE);
                }
        } /* while */

        if (opts.verbose)
        {
                printf("%s v%s\n", PRG_NAME, PRG_VERSION);
                printf("-- OLED params -- \n");
                printf("Oled is    : %s\n", oled_type_str[opts.oled]);
                printf("-- Other Stuff -- \n");
                printf("verbose is : %s\n", opts.verbose? "yes" : "no");
                printf("\n");
        }
}

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
  // Oled supported display in ArduiPi_SSD1306.h
  // Get OLED type
  parse_args(argc, argv);

  // SPI
  if (display.oled_is_spi_proto(opts.oled))
  {
     // SPI change parameters to fit to your LCD
     if ( !display.init(OLED_SPI_DC,OLED_SPI_RESET,OLED_SPI_CS, opts.oled) )
        exit(EXIT_FAILURE);
  }
  else
  {
     // I2C change parameters to fit to your LCD
     if ( !display.init(OLED_I2C_RESET,opts.oled) )
         exit(EXIT_FAILURE);
  }

  // prepare DS18B20 
  printf("Prepare DS18B20...\n");
  //DS18B20 w1Device1 (w1_address);
  double tempNow;
  char* msg_temp;

  printf("Beginning display work!\n");
  display.begin();

  printf("Clear display\n");

  while(1) {
    //tempNow = w1Device1.getTemp();
    //msg_temp = (char*) (&tempNow);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("TEMP\n");
    display.setTextSize(2);
    display.setTextColor(WHITE);
    //display.printf(msg_temp);
    display.display();
    sleep(1);
  }


  printf("THE END!\n");

  return 0;

}



























