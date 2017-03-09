char* mqtt_server   = "10.1.1.50";
char* mqtt_id       = "WifiSensorBoard-Debug";

char* mqtt_uptime_topic = "/devices/espD/uptime";

#define DHTPIN 12
#define DHT_SENSOR_TYPE DHT_TYPE_22
#define DHT_INTERVAL 10000
#define DHT_TEMPERATURE_TOPIC "/temperature/espD/dht"
#define DHT_HUMIDITY_TOPIC "/humidity/espD/dht"

#define ONE_WIRE_BUS 13
//#define PULLUP_PIN D1
#define TEMPERATURE_PRECISION 12
#define MAX_DEVICES 5
#define DALLAS_BASETOPIC "/temperature/"
#define DALLAS_INTERVAL 10000

#define NUM_LEDS 81
//#define NUM_LEDS 74
#define FRAMES_PER_SECOND  120

#define LED_STATE_TOPIC "/devices/espD/led/state"
#define LED_SOLID_TOPIC "/devices/espD/led/command/solid"
#define LED_ANIMATION_TOPIC "/devices/espD/led/command/animation"

