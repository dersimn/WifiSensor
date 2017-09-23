#if SENSOR_BMP_ENABLE

Adafruit_BMP085 bmp;
Thread sensorBMPmeassureThread = Thread();

void setup_Sensor_BMP() {
  if (!bmp.begin()) {
    LogBMP.error("No device found");
  } else {
    sensorBMPmeassureThread.onRun(sensorBMPmeassureFunc);
    sensorBMPmeassureThread.setInterval(BMP_INTERVAL);
    threadControl.add(&sensorBMPmeassureThread); 
  }
}

void sensorBMPmeassureFunc() {
  mqtt_publish("temperature/bmp", String(bmp.readTemperature(), 1));
  mqtt_publish("pressure/bmp", String(bmp.readPressure()/100.0, 2));
}

#else
void setup_Sensor_BMP() {}
void loop_Sensor_BMP() {}
#endif
