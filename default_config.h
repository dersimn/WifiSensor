#define MQTT_SERVER     "10.1.1.50"
/*
 * Build MQTT topics like
 *     
 *     PREFIX  CMD    DEVICE          SERVICE
 *     dersimn/status/DevilRemote0815/hifi/volume
 *     dersimn/set   /DevilRemote0815/hifi/volume
 *     
 * ID and services will be added in file MQTT.ino
 */
#define MQTT_PREFIX     "dersimn"

#define DHTPIN D4
#define DHT_SENSOR_TYPE DHT_TYPE_22
#define DHT_INTERVAL 10000

#define ONE_WIRE_BUS D3
#define PULLUP_PIN D0
#define TEMPERATURE_PRECISION 12
#define MAX_DEVICES 5
#define DALLAS_INTERVAL 10000

#define NUM_LEDS 16
#define FRAMES_PER_SECOND  120

#define MOSFETLED_PIN D5
