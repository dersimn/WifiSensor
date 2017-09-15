#if MOSFETLED_ENABLE

uint8_t mosfetLED_brightness = 0;

void setup_mosfetLED() {
  pinMode(MOSFETLED_PIN, OUTPUT);
  digitalWrite(MOSFETLED_PIN, LOW);

  mqtt_subscribe("lights/mosfetled",  mosfetLED_subscribe);
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
  mosfetLED_brightness = 255;
}
void mosfetLED_off() {
  mosfetLED_brightness = 0;
}

void mosfetLED_postNewState() {
  mqtt_publish("lights/mosfetled", s+rescale(mosfetLED_brightness, 255, 100));
}


void mosfetLED_solid(String inputString) {
  // Brightness 0-100
  uint8_t bri_tmp;
  float bri_raw = inputString.toFloat();
  if (bri_raw < 2.0) { 
    bri_tmp = 0;
  } else {
    bri_tmp = (uint8_t)rescale(bri_raw, 100, 255);
  }
  if (bri_raw > 98.0) { 
    bri_tmp = 255;
  } else {
    bri_tmp = (uint8_t)rescale(bri_raw, 100, 255);
  }
  
  //TODO: check values
  mosfetLED_brightness = bri_tmp;
}

void mosfetLED_setOutput() {
  if (mosfetLED_brightness < 5) { 
    analogWrite(MOSFETLED_PIN, 0);
    digitalWrite(MOSFETLED_PIN, LOW);
  } else if (mosfetLED_brightness > 250) { 
    analogWrite(MOSFETLED_PIN, 0);
    digitalWrite(MOSFETLED_PIN, HIGH);
  } else {
    analogWrite(MOSFETLED_PIN, mosfetLED_brightness);
  }

  // Output
  mosfetLED_postNewState();
}

#else
void setup_mosfetLED() {}
void loop_mosfetLED() {}
#endif
