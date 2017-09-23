/*
 * Build MQTT topics like
 *     
 *     PREFIX  CMD    DEVICE          SERVICE
 *     dersimn/status/DevilRemote0815/hifi/volume
 *     dersimn/set   /DevilRemote0815/hifi/volume
 *     
 * ID and services will be added in file MQTT.ino
 */
#define MQTT_PREFIX           "dersimn"
#define MQTT_SERVER           "10.1.1.50"

#define FASTLED_ENABLE        1
#define MOSFETLED_ENABLE      1
#define SENSOR_DHT_ENABLE     1
#define SENSOR_DALLAS_ENABLE  1
#define SENSOR_BMP_ENABLE     1

// Sensor_DHT
#define DHT_PIN               D4
#define DHT_SENSOR_TYPE       DHT_TYPE_22
#define DHT_INTERVAL          10000

// Sensor_Dallas
#define DS_ONE_WIRE_BUS       D3
#define DS_PULLUP_PIN         D0
#define DS_MAX_DEVICES        5
#define DS_INTERVAL           10000
#define DS_PRECISION          12

// Sensor_BMP
#define BMP_INTERVAL          10000

// FastLED
#define FASTLED_NUM_LEDS      16
#define FASTLED_FPS           120

// mosfetLED
#define MOSFETLED_PIN         D5
