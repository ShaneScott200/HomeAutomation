//#include <cstdlib>
#include <cstdio>
//#include <stdlib.h>
#include <ctime>
#include <iostream>
#include </usr/include/mosquittopp.h>
#include "MQTTWrapper.h"

int main()
{
    std::time_t result = std::time(NULL);
    std::cout << std:: asctime(std::localtime(&result)) << result << " seconds since the Epoch\n";  // replace this with the mqtt.publish function
   
    mosqpp::mosquittopp* m = new mosqpp::mosquittopp("Test", false);
    m->connect("192.168.0.200", 1883, 60);
    char resultstring[32];
    sprintf(resultstring, "%ld", result);
    m->publish(NULL, "time", 32, resultstring, 0, false);

}
