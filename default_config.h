/*
 * Build MQTT topics like
 *     
 *     PREFIX  CMD    DEVICE          SERVICE
 *     dersimn/status/DevilRemote0815/hifi/volume
 *     dersimn/set   /DevilRemote0815/hifi/volume
 *     
 * ID and services will be added in file MQTT.ino
 */
#define BOARD_ID_PREFIX					"WifiSensor_"
#define WIFI_SSID						""
#define WIFI_PASS						""
#define MQTT_PREFIX						"dersimn"
#define MQTT_SERVER						"10.1.1.50"

#define SENSOR_DHT_ENABLE     			1
#define SENSOR_DALLAS_ENABLE  			0
#define SENSOR_BMP_ENABLE     			0
#define SENSOR_TSL2561_ENABLE 			0
#define SENSOR_SINGLE_DALLAS_ENABLE		1

// Maintenance
#define MAINTENANCE_UPTIME_INTERVAL     60
#define MAINTENANCE_INFO_INTERVAL       60

// Sensor_DHT
#define DHT_PIN               D4
#define DHT_SENSOR_TYPE       DHT_TYPE_22
#define DHT_INTERVAL          10000

// Sensor_Dallas
#define DS_ONE_WIRE_BUS       D3
#define DS_MAX_DEVICES        5
#define DS_INTERVAL           10000
#define DS_PRECISION          10

// Sensor_BMP
#define BMP_INTERVAL          10000

// Sensor_TSL2561
#define TSL2561_INTERVAL      10000

