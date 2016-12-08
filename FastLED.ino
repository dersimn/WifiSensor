#include <FastLED.h>
#include <NeoPixelBus.h>

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(NUM_LEDS);
CRGB leds[NUM_LEDS];
Thread led_show = Thread();
Thread led_animation = Thread();
Thread led_base_hue = Thread();

uint8_t baseHue = 0;
uint8_t hue = 0;
uint8_t saturation = 0;
uint8_t brightness = 255;

uint8_t currentPatternNumber = 0;
typedef void (*SimplePatternList[])();
SimplePatternList patterns = { sinelon, rainbow, confetti };

void setup_fastled() {
  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();  

  led_show.onRun(led_show_func);
  led_show.setInterval(1000 / FRAMES_PER_SECOND);
  threadControl.add(&led_show);

  led_animation.enabled = false;
  led_animation.setInterval(20);
  led_animation.onRun(led_animation_func);
  threadControl.add(&led_animation); 

  led_base_hue.enabled = false;
  led_base_hue.setInterval(20);
  led_base_hue.onRun(led_base_hue_func);
  threadControl.add(&led_base_hue); 
}

void loop_fastled() {
  
}

void led_show_func() {
  RgbColor pixel;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    pixel = RgbColor(leds[i].r, leds[i].g, leds[i].b);
    strip.SetPixelColor(i, pixel);
  }
  strip.Show();  
}

void led_animation_func() {
  patterns[currentPatternNumber]();
}
void led_base_hue_func() {
  baseHue++;
}

void led_input(String topic, String inputString) {
  if (topic == LED_SOLID_TOPIC) {
    if (inputString == "ON") {
      led_on();
    } else if (inputString == "OFF") {
      led_off();
    } else {
      led_solid(inputString);
    }
  } else if (topic == LED_ANIMATION_TOPIC) {
    if (inputString == "OFF") {
      led_off();
    } else {
      led_animation_msg(inputString);
    }
  }
}

void led_solid(String inputString) {
  Serial.println("led_solid input");
  
  String results[10];
  uint8_t count = explode(results, inputString, ',');
  for (uint8_t i = 0; i < count; i++) {
    Serial.print("i = ");Serial.println(i);
    Serial.print("result = ");Serial.println(results[i]);
  }

  if ( count != 3 ) return;

  // Hue 0-360
  uint8_t hue_tmp;
  float hue_raw = (results[0]).toFloat();
  hue_tmp = hue_raw / 360.0 * 255.0;

  // Saturation 0-100
  uint8_t sat_tmp;
  float sat_raw = (results[1]).toFloat();
  sat_tmp = sat_raw / 100.0 * 255.0;

  // Brightness 0-100
  uint8_t bri_tmp;
  float bri_raw = (results[2]).toFloat();
  if (bri_raw < 1.0) { 
    bri_tmp = 0;
  } else {
    bri_tmp = bri_raw / 100.0 * 255.0;
  }

  Serial.println("conversion done");

  //TODO: check values
  hue = hue_tmp;
  saturation = sat_tmp;
  brightness = bri_tmp;

  // Output
  led_fillSolidColor();
  postNewState();
}

void postNewState() {
  String state = String(hue / 255.0 * 360.0);
  state += ",";
  state += String(saturation / 255.0 * 100.0);
  state += ",";
  state += String(brightness / 255.0 * 100.0);
  
  mqtt_publish(LED_STATE_TOPIC, state);
}

void led_fillSolidColor() {
  led_animation.enabled = false;
  led_base_hue.enabled = false;
  fill_solid( leds, NUM_LEDS, CHSV( hue, saturation, brightness));
}
void led_on() {
  brightness = 255;
  led_fillSolidColor();
  postNewState();
}
void led_off() {
  brightness = 0;
  led_fillSolidColor();
  postNewState();
}

void led_animation_msg(String inputString) {
  if (inputString == "sinelon") {
    currentPatternNumber = 0;

    led_animation.setInterval(20);
    led_base_hue.setInterval(10);
    
    led_animation.enabled = true;
    led_base_hue.enabled = true;
    
    mqttClient.publish(LED_STATE_TOPIC, "sinelon");
  } else if (inputString == "rainbow") {
    currentPatternNumber = 1;

    led_animation.setInterval(20);
    led_base_hue.setInterval(20);
    
    led_animation.enabled = true;
    led_base_hue.enabled = true;
    
    mqttClient.publish(LED_STATE_TOPIC, "rainbow");
  } else if (inputString == "confetti") {
    currentPatternNumber = 2;
    
    led_animation.setInterval(10);
    led_base_hue.setInterval(50);
    
    led_animation.enabled = true;
    led_base_hue.enabled = true;

    mqttClient.publish(LED_STATE_TOPIC, "confetti");
  }
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS-1);
  leds[pos] += CHSV( baseHue, 255, 192);
}
void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( baseHue + random8(64), 255, 255);
}
void rainbow() {
  fill_rainbow( leds, NUM_LEDS, baseHue);
}

