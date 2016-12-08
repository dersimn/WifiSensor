char* mqtt_server   = "10.1.1.50";
char* mqtt_id       = "WifiSensorBoard-0001";

char* mqtt_uptime_topic = "/debug/fastled/uptime";

#define DHTPIN D2
#define DHT_SENSOR_TYPE DHT_TYPE_22
#define DHT_INTERVAL 10000
#define DHT_TEMPERATURE_TOPIC "/temperature/dht"
#define DHT_HUMIDITY_TOPIC "/humidity/dht"

#define ONE_WIRE_BUS D7
#define PULLUP_PIN D0
#define TEMPERATURE_PRECISION 9
#define MAX_DEVICES 5
#define DALLAS_BASETOPIC "/temperature/"
#define DALLAS_INTERVAL 10000

#define NUM_LEDS 68
//#define NUM_LEDS 74
#define FRAMES_PER_SECOND  120

#define LED_STATE_TOPIC "/debug/led/state"
#define LED_SOLID_TOPIC "/debug/led/input"
#define LED_ANIMATION_TOPIC "/debug/led/animation"

