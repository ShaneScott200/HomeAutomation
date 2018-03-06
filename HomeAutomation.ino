// ====================== Home Automation ======================
// 
// This sketch uses MQTT to publish data from the Arduino to a host (running Linux)
// This sketch also subscribes to a topic to receive data from the host
// Publish:
//      - Topics: hal/temp, hal/humidity, hal/garage
//
//      Client should use command "mosquitto_sub -t ID12345 to subscribe to data from Arduino
// 
// Subscribe:
//      - Topics: inTopic
//      - usage: mosquitto_pub -t inTopic -m "Hello"
//
//      Client should use command "mosquitto_pub -t inTopic -m "Hello World" to publish data to Arduino
// 
// =============================================================

// ====================== OLED Display Setup ======================
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 3
Adafruit_SSD1306 display(OLED_RESET);

// ====================== DHT Temperature/Humidity Setup ======================
#include "dht11.h"
#define DHTPIN          3
#define DHTTYPE         DHT11
dht11 DHT11;
int t = 0;
int h = 0;

// ====================== Ethernet Setup ======================
#include <UIPEthernet.h>
//#include <EtherCard.h>
uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
EthernetClient ethClient;

// ====================== MQTT Setup ======================
#include "PubSubClient.h"
PubSubClient mqttClient (ethClient);
#define CLIENT_ID       "UnoMQTT"
#define MQTT_KEEPALIVE  60

// ====================== TimeLib Setup ======================
#include <TimeLib.h>
time_t prevDisplay = 0; // when the digital clock was displayed

// ====================== Barometer Setup ======================
#include <SFE_BMP180.h>
SFE_BMP180 pressure;
//#define ALTITUDE 1655.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters
#define ALTITUDE 70.0 // Altitude of Ottawa ON in meters
char status;
double T,P,Ppsi,p0,a;

// ====================== DS18B20 Setup ======================
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into pin 4 on the Arduino
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
float DS18B20_temp = 0;

  
// ====================== PIR Motion Sensor Setup ======================
int pirInputPin = 4;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

// ====================== Program Setup ======================
#define INTERVAL            5000      // 1 sec delay between publishing
#define RECONNECT_INTERVAL  5000  // 5 sec delay between reconnect tries
int pinPB1 = 11;
bool statusPB1=LOW;         //push button #1
int pinPB2 = 10;
bool statusPB2=LOW;         //push button #2
int pinPB3 = 9;
bool statusPB3=LOW;         //push button #3
int pinPB4 = 8;
bool statusPB4=LOW;         //push button #4
int lastButtonState = 0;     // previous state of the mode change button
int state = -1;

int ledPin = 13;                // choose the pin for the LED


long previousMillis;
long reconPreviousMillis;

void setup() {
  pinMode(pinPB1,INPUT_PULLUP);
  pinMode(pinPB2,INPUT_PULLUP);
  pinMode(pinPB3,INPUT_PULLUP);
  pinMode(pinPB4,INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(pirInputPin, INPUT);     // declare sensor as input
  
  // --------- setup serial communication --------- 
  Serial.begin(57600);
  Serial.println("Initializing...");

  // --------- setup barometer --------- 
  // Initialize the sensor (it is important to get calibration values stored on the device).
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    Serial.println("BMP180 init fail\n\n");
  }

  // --------- setup OLED  --------- 
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen internally, this will display the splashscreen.
  display.display();
  delay(1000);
  // Clear the buffer.
  display.clearDisplay();
  // init done
  Serial.println("OLED Display Initialized!");

  // --------- setup DS18B20 Temperature Sensor  --------- 
  sensors.begin();

  // --------- setup ethernet communication using DHCP --------- 
   if(Ethernet.begin(mac) == 0) {
     Serial.println(F("Ethernet configuration using DHCP failed"));
     for(;;);
   }
   Serial.print("Ethernet IP Address: ");
   Serial.println(Ethernet.localIP());
  
  // --------- setup mqtt client for publish --------- 
   mqttClient.setClient(ethClient);
   mqttClient.setServer("192.168.0.27",1883);          // for using local broker
   Serial.println(F("MQTT client configured"));

  // --------- setup mqtt client for subscribe --------- 
   mqttClient.setCallback(callback);
   mqttClient.subscribe("inTopic");

  reconnect();
  
  previousMillis = millis();
  reconPreviousMillis = millis();
  prevDisplay = now();
}

void loop() {
 // Need to update this to read time from MQTT 
   
  readInputs();

  stateChange();
        
  // check interval
  if(millis() - previousMillis > INTERVAL) {
    sendData();
    previousMillis = millis();
  }

  if (!mqttClient.connected()) {
      state=3;
      stateChange();
      if(millis() - reconPreviousMillis > RECONNECT_INTERVAL) 
      {
        Serial.println("Attempting reconnection...");
        reconnect();
        // Wait 5 seconds before retrying
        reconPreviousMillis = millis();
      }
  }
  //mqttClient.loop();
  if (!mqttClient.loop()) {
    Serial.print("Client disconnected...");
    if (mqttClient.connect(CLIENT_ID)) {
      Serial.println("reconnected.");
    } else {
      Serial.println("failed.");
    }
  }

  writeOutputs();
}


// ====================== sendData Method ======================
// 
// Publish data to MQTT broker
// 
// =============================================================
void sendData() 
{
  //char msgBuffer[20];

  char* msg = prepareMessage();
  
if(mqttClient.connect(CLIENT_ID)) 
  {
    mqttClient.publish("ID12345", msg);
    //mqttClient.publish("hal/temp", dtostrf(t, 6, 2, msgBuffer));
    //mqttClient.publish("hal/humid", dtostrf(h, 6, 2, msgBuffer));
    //mqttClient.publish("hal/door", (statusBD == HIGH) ? "OPEN" : "CLOSED");
    //mqttClient.publish("hal/garage",(statusGD == HIGH) ? "OPEN" : "CLOSED");
    //mqttClient.publish("hal/room",(statusKD == HIGH) ? "OPEN" : "CLOSED");
    //mqttClient.publish("hal/light", dtostrf(lightstatus, 4, 0, msgBuffer));
 }
}


// ====================== callback Method ======================
// 
// Method that is called when MQTT broker publishes a message
// 
// =============================================================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    //digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    //digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}


// ====================== reconnect Method ======================
// 
// Method that is called when MQTT loses connection to the broker
// 
// =============================================================
void reconnect() {
  // Loop until we're reconnected
  //while (!mqttClient.connected()) 
  mqttClient.disconnect();    // Try a disconnect, just in case a connection is left open.
  // Try to connect 3 times.  If connection is successful, break out of loop.
  for (int i = 0; i< 3;i++)
  {
      Serial.print(i);
      Serial.print(": ");
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      //String clientId = "ArduinoClient-";
      //clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (mqttClient.connect(CLIENT_ID)) 
      {
        Serial.println("connected");
        // Once connected, publish an announcement...
        mqttClient.publish("ID12345", "hello world");
        // ... and resubscribe
        mqttClient.subscribe("inTopic");
        state=0;
        break;
      } 
      else 
      {
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" try again in 5 seconds");
        /*
         * int - the client state, which can take the following values (constants defined in PubSubClient.h):
          -4 : MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time
          -3 : MQTT_CONNECTION_LOST - the network connection was broken
          -2 : MQTT_CONNECT_FAILED - the network connection failed
          -1 : MQTT_DISCONNECTED - the client is disconnected cleanly
          0 : MQTT_CONNECTED - the cient is connected
          1 : MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT
          2 : MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier
          3 : MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection
          4 : MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected
          5 : MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect
         */
      }
   }
}


// ====================== readInputs Method ======================
// 
// Method that is called to read the hardwired input signals
// 
// ===============================================================
void readInputs() {

    // Read DHT inputs
    int chk = DHT11.read(DHTPIN);
    h = (float)DHT11.humidity;
    t = (float)DHT11.temperature;

    // Read DS18B20 input
    sensors.requestTemperatures();  
    DS18B20_temp = sensors.getTempCByIndex(0); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  
    
    // Read raw inputs
    statusPB1=digitalRead(pinPB1);
    statusPB2=digitalRead(pinPB2);
    statusPB3=digitalRead(pinPB3);
    statusPB4=digitalRead(pinPB4);

    readPressure();

    val = digitalRead(pirInputPin);  // read input value
    if (val == HIGH) {            // check if the input is HIGH
      digitalWrite(ledPin, HIGH); // turn LED ON
      display.dim(0);
      display.display();
      if (pirState == LOW) {
        // we have just turned on
        Serial.println("Motion detected!");
        // We only want to print on the output change, not state
        pirState = HIGH;
      }
    } else {
      digitalWrite(ledPin, LOW); // turn LED OFF
      display.dim(1);
      display.display();
      if (pirState == HIGH){
        // we have just turned of
        Serial.println("Motion ended!");
        // We only want to print on the output change, not state
        pirState = LOW;
      }
    }
  
    // Set State
    // compare the buttonState to its previous state
    if (statusPB1 != lastButtonState) {
      // if the state has changed, increment the counter
      if (statusPB1 == HIGH) {
        // if the current state is HIGH then the button
        // went from off to on:
        state++;
        if (state > 2)  // Can't use buttons to access mode 3.  Mode 3 is an error state!
          state =0;
      } else {
        // if the current state is LOW then the button
        // went from on to off:
      }
      // Delay a little bit to avoid bouncing
      delay(5);
    }
    // save the current state as the last state,
    //for next time through the loop
    lastButtonState = statusPB1;
}

// ====================== readInputs Method ======================
// 
// Method that is called to read the hardwired input signals
// 
// ===============================================================
void readPressure() {
   // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:
  
  Serial.println();
  Serial.print("provided altitude: ");
  Serial.print(ALTITUDE,0);
  Serial.print(" meters, ");
  Serial.print(ALTITUDE*3.28084,0);
  Serial.println(" feet");

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");
      
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.
      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Ppsi = p0 * 0.0145038;
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(Ppsi,2);
          Serial.println(" PSIg");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

}



// ====================== writeOutputs Method ======================
// 
// Method that is called to write the hardwired output signals
// 
// =================================================================
void writeOutputs() {

  
  
}


// ====================== prepareMessage Method ======================
// 
// Method that is called to prepare the message that is sent to broker
// 
// ===================================================================
char* prepareMessage() {
    // convert all data to a single string with ':' as separator
    char t_msgBuffer[20], h_msgBuffer[20];
    dtostrf(t, 6, 2, t_msgBuffer);
    dtostrf(h, 6, 2, h_msgBuffer);

  /*display.print(year()); 
  display.print("-");
  display.print(month());
  display.print("-");
  display.println(day());

  display.print(hour());
  display.print(":");
  if(minute() < 10)
    display.print('0');
  display.print(minute());
  display.print(":");
  if(second() < 10)
    display.print('0');
  display.print(second());*/

    char myConcatenation[80];
    sprintf(myConcatenation,"%i-%i-%i %i:%i:%i - %s:%s:%i:%i:%i:%i",year(),month(), day(), hour(), minute(), second(), t_msgBuffer, h_msgBuffer, !statusPB1, !statusPB2, !statusPB3, !statusPB4);

    return myConcatenation;
}

// ====================== UpdateLocationDisplay Method =================
// 
// Method that is called to display the location data on the OLED
// The location data will always be the first line of the OLED
// 
// ===================================================================
void displayLocation(){
  
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(0,0);
  display.print("Office");

  display.setCursor(100,0);
  display.print(state);

}

// ====================== State 0 Display (Main) Method =================
// 
// Method that is called to display the date/time data on the OLED
// 
// ===================================================================
void displayState0(){
  display.clearDisplay();
  displayLocation();

  display.setTextSize(2);
  display.setCursor(0,16);
  
  // digital clock display of the time
  display.print(year()); 
  display.print("-");
  display.print(month());
  display.print("-");
  display.println(day());

  display.setCursor(0,32);
  display.print(hour());
  display.print(":");
  if(minute() < 10)
    display.print('0');
  display.print(minute());
  display.print(":");
  if(second() < 10)
    display.print('0');
  display.print(second());

  display.display();
}


// ====================== State 1 (Secondary) Display Method =================
// 
// Method that is called to display the Temperature and Humidity data on the OLED
// 
// ===================================================================
void displayState1(){
  display.clearDisplay();
  displayLocation();

  display.setTextSize(1);
  display.setCursor(0,16);

  display.print("T:");
  display.print(T,1); // use temperature from Barometer Sensor (better resolution)
  display.println(" C");

  display.print("DS:");
  display.print(DS18B20_temp,1); // use temperature from Barometer Sensor (better resolution)
  display.println(" C");
  
  display.print("H:");
  display.print(h);
  display.println("   %");

  display.print("P:");
  display.println(Ppsi,1);
  display.display();
}

// ====================== State 2 (Configuration) Display Method =================
// 
// Method that is called to display the Settings data on the OLED
// 
// ===================================================================
void displayState2(){
  display.clearDisplay();
  displayLocation();

  display.setTextSize(1);
  display.setCursor(0,16);
  display.print("IP:");
  display.print(Ethernet.localIP());
  
  display.setCursor(0,26);
  display.print("SUB:");
  display.print(Ethernet.subnetMask());
  
  display.setCursor(0,36);
  display.print("GW:");
  display.print(Ethernet.gatewayIP());
  
  display.setCursor(0,46);
  display.print("MAC");
  (mac[0])>9 ? display.print(":") : display.print(":0");
  display.print(mac[0]);
  (mac[1])>9 ? display.print(":") : display.print(":0");
  display.print(mac[1]);
  (mac[2])>9 ? display.print(":") : display.print(":0");
  display.print(mac[2]);
  (mac[3])>9 ? display.print(":") : display.print(":0");
  display.print(mac[3]);
  (mac[4])>9 ? display.print(":") : display.print(":0");
  display.print(mac[4]);
  (mac[5])>9 ? display.print(":") : display.print(":0");
  display.print(mac[5]);
  
  display.display();
}

void displayState3() {
  display.clearDisplay();
  displayLocation();
  
  display.setTextSize(2);
  display.setCursor(0,16);
  display.print("Connection");

  display.setCursor(0,32);
  display.print("Lost!");

  display.display();
  
}

void stateChange()
{
    // turns on the LED every four button pushes by
  // checking the modulo of the button push counter.
  // the modulo function gives you the remainder of
  // the division of two numbers:
  //Serial.println(state);
  if (state % 4 == 0) {             // State 1
    displayState0();
  } 
  if (state % 4 == 1) {             // State 2
    displayState1();
  } 
  if (state % 4 == 2) {             // State 3
    displayState2();
  } 
  if (state % 4 == 3) {             // State 4
    displayState3();
  } 
}

