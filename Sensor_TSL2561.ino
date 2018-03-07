#if SENSOR_TSL2561_ENABLE

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
Thread sensorTSL2561meassureThread = Thread();

void setup_Sensor_TSL2561() {
  if (!tsl.begin()) {
    LogTSL2561.error("No device found");
  } else {
    //tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
    //tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
    tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
    
    /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
    tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
    // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
    // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

    sensorTSL2561meassureThread.onRun(sensorTSL2561meassureFunc);
    sensorTSL2561meassureThread.setInterval(TSL2561_INTERVAL);
    threadControl.add(&sensorTSL2561meassureThread); 
  }
}

void sensorTSL2561meassureFunc() {
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  if (event.light) {
    mqtt.publish("brightness/tsl2561", String(event.light, 0));
  } else {
    /* If event.light = 0 lux the sensor is probably saturated and no reliable data could be generated! */
    LogTSL2561.warn("Sensor overload");
  }
}

#else
void setup_Sensor_TSL2561() {}
void loop_Sensor_TSL2561() {}
#endif
