#include "default_config.h"
#include "default_wifi.h"
#include "handy_functions.h"

// --------------------------------------------------------

#include <ESP8266WiFi.h>        // Wifi Library
#include <PubSubClient.h>       // MQTT Library
#include <Thread.h>
#include <ThreadController.h>

#include <DallasTemperature.h>  // Throws compiler error if not specified here

// --------------------------------------------------------

ThreadController threadControl = ThreadController();

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// --------------------------------------------------------

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial port initialization

  setup_wifi();
  setup_mqtt();
  setup_uptime();
  setup_sensor_dht();
  setup_dallas();
  setup_fastled();
  setup_mosfetLED();
}

void loop() {
  loop_mqtt();
  loop_sensor_dht();

  threadControl.run();
}
