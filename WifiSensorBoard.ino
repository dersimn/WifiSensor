#include "default_config.h"
#include "handy_functions.h"

#include <GenericLog.h>         // https://github.com/dersimn/ArduinoUnifiedLog
#include <NamedLog.h>
#include <LogHandler.h>
#include <LogSerialModule.h>
#include <LogMqttModule.h>

#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>
#include <ThreadRunOnce.h>      // https://github.com/dersimn/ArduinoThreadRunOnce

#if USE_WIFI_MANAGER
#include <WiFiManager.h>        // https://github.com/tzapu/WiFiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#endif

#include <PubSubClient.h>       // https://github.com/knolleary/pubsubclient
#include <PubSubClientTools.h>  // https://github.com/dersimn/ArduinoPubSubClientTools

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

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h> // https://github.com/adafruit/Adafruit_TSL2561

#include <I2CSoilMoistureSensor.h> // https://github.com/Apollon77/I2CSoilMoistureSensor

// --------------------------------------------------------

LogHandler logHandler;
LogSerialModule serialModule(115200);

GenericLog Log    (logHandler);
NamedLog   LogWiFi(logHandler, "WiFi");
NamedLog   LogMqtt(logHandler, "MQTT");
NamedLog   LogDallas(logHandler, "Dallas");
NamedLog   LogBMP(logHandler, "BMP");
NamedLog   LogTSL2561(logHandler, "TSL2561");
NamedLog   LogChirp(logHandler, "Chirp");

ThreadController threadControl = ThreadController();

WiFiClient espClient;
PubSubClient mqttClient(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(mqttClient);

String s = "";
const String ESP_ID = upperCaseStr(String(ESP.getChipId(), HEX));
const String BOARD_ID = s+BOARD_ID_PREFIX+ESP_ID;
char   BOARD_ID_CHAR[50];

// --------------------------------------------------------

void setup() {
  BOARD_ID.toCharArray(BOARD_ID_CHAR, 50);

  logHandler.addModule(&serialModule);
  Log.info("Initializing 'WifiSensor'");
  Log.info( String("ESP ID: ") + ESP_ID );

  // Init Submodules
  setup_WiFi();
  setup_MQTT();
  ArduinoOTA.setHostname(BOARD_ID_CHAR);
  ArduinoOTA.begin();
  
  setup_FastLED();
  setup_mosfetLED();
  
  setup_Sensor_Dallas();
  setup_Sensor_BMP();
  setup_Sensor_TSL2561();
  setup_Sensor_Chirp();
  
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
  ArduinoOTA.handle();

  loop_Sensor_DHT();

  threadControl.run();
}
