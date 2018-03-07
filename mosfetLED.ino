#if MOSFETLED_ENABLE

uint8_t mosfetLED_brightness = 0;

void setup_mosfetLED() {
  pinMode(MOSFETLED_PIN, OUTPUT);
  analogWrite(MOSFETLED_PIN, 0);

  mqtt.subscribe("lights/mosfetled",  mosfetLED_subscribe);
}

void mosfetLED_subscribe(String topic, String message) {
  if (message == "ON") {
    mosfetLED_on();
    mosfetLED_setOutput();
  } else if (message == "OFF") {
    mosfetLED_off();
    mosfetLED_setOutput();
  } else {
    mosfetLED_solid(message);
    mosfetLED_setOutput();
  }
}

void mosfetLED_on() {
  mosfetLED_brightness = PWMRANGE;
}
void mosfetLED_off() {
  mosfetLED_brightness = 0;
}

void mosfetLED_postNewState() {
  mqtt.publish("lights/mosfetled", s+rescale(mosfetLED_brightness, PWMRANGE, 100));
}


void mosfetLED_solid(String inputString) {
  // Brightness 0-100
  uint8_t bri_tmp;
  float bri_raw = inputString.toFloat();
  if (bri_raw < 2.0) { 
    bri_tmp = 0;
  } else {
    bri_tmp = (uint8_t)rescale(bri_raw, 100, PWMRANGE);
  }
  if (bri_raw > 98.0) { 
    bri_tmp = PWMRANGE;
  } else {
    bri_tmp = (uint8_t)rescale(bri_raw, 100, PWMRANGE);
  }
  
  //TODO: check values
  mosfetLED_brightness = bri_tmp;
}

void mosfetLED_setOutput() {
  analogWrite(MOSFETLED_PIN, mosfetLED_brightness);
  mosfetLED_postNewState();
}

#else
void setup_mosfetLED() {}
void loop_mosfetLED() {}
#endif
