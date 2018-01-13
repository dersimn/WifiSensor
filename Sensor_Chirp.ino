#if SENSOR_CHIRP_ENABLE

I2CSoilMoistureSensor chirp;
Thread sensorChirpMeassureThread = Thread();
ThreadRunOnce sensorChirpOutputThread = ThreadRunOnce();

void setup_Sensor_Chirp() {
  chirp.begin();
  delay(1000);
  LogChirp.info(s+"Chirp Sensor FW: "+chirp.getVersion());
  
  sensorChirpMeassureThread.onRun(sensorChirpMeassureFunc);
  sensorChirpMeassureThread.setInterval(CHIRP_INTERVAL);
  threadControl.add(&sensorChirpMeassureThread);

  sensorChirpOutputThread.onRun(sensorChirpOutputFunc);
  threadControl.add(&sensorChirpOutputThread);
}

void sensorChirpMeassureFunc() {
  int count = 0;
  while (chirp.isBusy()) {
    delay(10);
    count++;
    if (count >= 10) {
      return;
    }
  }
  mqtt_publish("moisture/chirp", String(chirp.getCapacitance()));
  mqtt_publish("temperature/chirp", String(chirp.getTemperature()/(float)10, 1));

  chirp.startMeasureLight();
  sensorChirpOutputThread.setRunOnce(3000);
}
void sensorChirpOutputFunc() {
  mqtt_publish("brightness/chirp", String(chirp.getLight()));
}

#else
void setup_Sensor_Chirp() {}
void loop_Sensor_Chirp() {}
#endif
