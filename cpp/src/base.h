#include <string>
#include “ArduiPi_OLED_lib.h”
#include “ArduiPi_OLED.h”
#include “Adafruit_GFX.h”
#include “DS18B20.h”
#include </usr/include/mosquittopp.h>
#include “dht.h”
#include </usr/include/pcf8574.h>

Base.h
public:
// Default Constructor
Base::Base()
// Overload Constructor
Base::Base(..arguments…)

// Configuration Function
void Base::ConfigureOLED(int address, bool type, bool size, int rotation)
void Base::ConfigureDS18B20(string address)
void Base::ConfigureMQTT(string ipaddress, int port, string messageBase)
void Base::ConfigureDHT (bool DHTType)
void Base::ConfigurePCF8574(int address)

// OLED Functions
void Base::WriteOLED(string message)

// DS18B20 Functions
double Base::ReadTemperature()  // (from DS18B20)

// DHT Functions
double Base::ReadTemperature() // (from DHT22 if installed)  --> need to differentiate between DHT & DS18B20
double Base::ReadHumidity() // (from DHT22 if installed)

// MQTT Functions
void Base::PublishMessage(string message)
void Base::SubscribeMessage(string message, callback) // (callback 	provided in main automation program)

// PCF8574 Functions
// define DIO pins (unless pin layout fixed?)
void Base::WritePin(int pin, bool value)
bool Base::ReadPin(int pin)
int Base::ReadAllPins()

private:
bool _configured;
