char* mqtt_server   = "10.1.1.50";
char* mqtt_id       = "WifiSensorBoard-0001";

char* mqtt_uptime_topic = "/debug/fastled/uptime";

#define DHTPIN 5
#define DHT_SENSOR_TYPE DHT_TYPE_22
#define DHT_INTERVAL 10000
#define DHT_TEMPERATURE_TOPIC "/temperature/dht"
#define DHT_HUMIDITY_TOPIC "/humidity/dht"

#define ONE_WIRE_BUS 2
#define PULLUP_PIN 14
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

// Definitions for Node MCU Board
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;