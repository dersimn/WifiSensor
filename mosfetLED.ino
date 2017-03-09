uint8_t mosfetLED_brightness = 255;

void setup_mosfetLED() {
  pinMode(MOSFETLED_PIN, OUTPUT);
  analogWrite(MOSFETLED_PIN, 0);
}

void mosfetLED_input(String topic, String inputString) {
  if (inputString == "ON") {
    mosfetLED_on();
  } else if (inputString == "OFF") {
    mosfetLED_off();
  } else {
    mosfetLED_solid(inputString);
  }
}

void mosfetLED_on() {
  mosfetLED_brightness = 255;
  analogWrite(MOSFETLED_PIN, 0);
  digitalWrite(MOSFETLED_PIN, HIGH);
  mosfetLED_postNewState();
}
void mosfetLED_off() {
  mosfetLED_brightness = 0;
  analogWrite(MOSFETLED_PIN, 0);
  digitalWrite(MOSFETLED_PIN, LOW);
  mosfetLED_postNewState();
}

void mosfetLED_postNewState() {
  String state = String(mosfetLED_brightness / 255.0 * 100.0);
  mqtt_publish(MOSFETLED_STATE_TOPIC, state);
}


void mosfetLED_solid(String inputString) {
  // Brightness 0-100
  uint8_t bri_tmp;
  float bri_raw = inputString.toFloat();
  if (bri_raw < 2.0) { 
    bri_tmp = 0;
  } else {
    bri_tmp = bri_raw / 100.0 * 255.0;
  }
  if (bri_raw > 98.0) { 
    bri_tmp = 255;
  } else {
    bri_tmp = bri_raw / 100.0 * 255.0;
  }
  
  //TODO: check values
  mosfetLED_brightness = bri_tmp;

  // Output
  analogWrite(MOSFETLED_PIN, mosfetLED_brightness);
  mosfetLED_postNewState();
}
