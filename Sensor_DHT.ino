#if SENSOR_DHT_ENABLE

DHT_nonblocking dht_sensor( DHT_PIN, DHT_SENSOR_TYPE );

void loop_Sensor_DHT() {
  float temperature;
  float humidity;
  if( measure_environment( &temperature, &humidity ) == true )
  {
    mqtt.publish(s+MQTT_PREFIX+"/status/"+BOARD_ID+"/temperature/dht", String(temperature, 1));
    mqtt.publish(s+MQTT_PREFIX+"/status/"+BOARD_ID+"/humidity/dht", String(humidity, 1));
  }  
}

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every few seconds. */
  if( millis( ) - measurement_timestamp > DHT_INTERVAL )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

#else
void setup_Sensor_DHT() {}
void loop_Sensor_DHT() {}
#endif
