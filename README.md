# HomeAutomation
Home Automation project

#changes/improvements to be made to ha.cpp
1. Add pcf8574 capability.  See example in testing/pcf8574 folder
2. some variables are created outside main() (as globals) while others are created inside main().  All varaible should be created inside main().
3. A separate object should be created to emulate the PCB test board.  It should contain all of the fuctionality that the PCB contains (like OLED, temperature (DS18B20 and/or DHT11), discrete I/O (using PCF8574).  Even though it is not specific to the PCB the object should allow for MQTT capability.  Creating this object will require passing several parameters to the object constructor (e.g. oled size - 128x32 or 128x64, DS18B20 address, PCF8574 configuration - input vs output, MQTT base topic)
4. The main() function prints messages to both the console window and the oled.  The oled should be instantiated first and then all messages should be printed to the oled.
5. Combination of both printf and cout.  Is there a reason?  If not, the replace printf with cout.
6. Should update while(1) function with the capability of using 'Q' or 'q' to quit.
7. Setting up display configuration is done each time the loop is called.  This could be moved outside the loop.
8. MQTT isn't setup correctly.  Use timeServer example to get MQTT to work.
