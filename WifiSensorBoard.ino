#include "default_config.h"
#include "handy_functions.h"

#include <GenericLog.h>         // https://github.com/dersimn/ArduinoUnifiedLog
#include <NamedLog.h>
#include <LogHandler.h>
#include <LogSerialModule.h>

#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>
#include <ThreadRunOnce.h>      // https://github.com/dersimn/ArduinoThreadRunOnce

#include <WiFiManager.h>        // https://github.com/tzapu/WiFiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#include <PubSubClient.h>       // https://github.com/knolleary/pubsubclient

#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#include <FastLED.h>            // https://github.com/FastLED/FastLED
#include <NeoPixelBus.h>        // https://github.com/Makuna/NeoPixelBus
                                // Usage of both libraries because of flicekring issue, see: 
                                // https://github.com/FastLED/FastLED/issues/306

#include <OneWire.h>
#include <DallasTemperature.h>  // https://github.com/milesburton/Arduino-Temperature-Control-Library

#include <dht_nonblocking.h>    // https://github.com/olewolf/DHT_nonblocking
#include <DallasTemperature.h>  // Throws compiler error if not specified in main file

#include <Adafruit_BMP085.h>    // https://github.com/adafruit/Adafruit-BMP085-Library
                                // This library is NOT a non-blocking one. It uses delay(5) for timing. Better don't use together with FastLED and animations.

// --------------------------------------------------------

LogHandler logHandler;
LogSerialModule serialModule(115200);

GenericLog Log    (logHandler);
NamedLog   LogWiFi(logHandler, "WiFi");
NamedLog   LogMqtt(logHandler, "MQTT");
NamedLog   LogDallas(logHandler, "Dallas");
NamedLog   LogBMP(logHandler, "BMP");

ThreadController threadControl = ThreadController();

WiFiClient espClient;
PubSubClient mqttClient(espClient);

String BOARD_ID = String("WifiSensor")+ESP.getChipId();
char   BOARD_ID_CHAR[50];
String s = "";

// --------------------------------------------------------

void setup() {
  BOARD_ID.toCharArray(BOARD_ID_CHAR, 50);

  logHandler.addModule(&serialModule);
  Log.info("Initializing 'WifiSensor'");
  Log.info( String("ESP ID: ") + ESP.getChipId() );

  // Init Submodules
  setup_WiFi();
  setup_MQTT();
  setup_ArduinoOTA();
  
  setup_FastLED();
  setup_mosfetLED();
  
  setup_Sensor_Dallas();
  setup_Sensor_BMP();
  
  setup_Maintanance();

  Log.info("Setup done");
}

void loop() {
  static bool enteredLoop = false;
  if ( !enteredLoop ) {
    enteredLoop = true;
    Log.info("Entering loop()");
  }

  loop_MQTT();
  loop_ArduinoOTA();

  loop_Sensor_DHT();

  threadControl.run();
}
