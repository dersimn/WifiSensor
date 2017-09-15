#include <dht_nonblocking.h>

DHT_nonblocking dht_sensor( DHTPIN, DHT_SENSOR_TYPE );

void loop_Sensor_DHT() {
  float temperature;
  float humidity;
  if( measure_environment( &temperature, &humidity ) == true )
  {
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" );

    mqtt_publish("temperature/dht", String(temperature, 1));
    mqtt_publish("humidity/dht"   , String(humidity, 1));
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

