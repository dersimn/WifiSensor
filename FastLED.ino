#if FASTLED_ENABLE

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(FASTLED_NUM_LEDS);
CRGB leds[FASTLED_NUM_LEDS];
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

void setup_FastLED() {
  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();  

  led_show.onRun(led_show_func);
  led_show.setInterval(1000 / FASTLED_FPS);
  threadControl.add(&led_show);

  led_animation.enabled = false;
  led_animation.setInterval(20);
  led_animation.onRun(led_animation_func);
  threadControl.add(&led_animation); 

  led_base_hue.enabled = false;
  led_base_hue.setInterval(20);
  led_base_hue.onRun(led_base_hue_func);
  threadControl.add(&led_base_hue); 

  mqtt_subscribe("lights/fastled",            FastLED_subscribe_solid);
  mqtt_subscribe("lights/fastled/animation",  FastLED_subscribe_animation);
}

void led_show_func() {
  RgbColor pixel;
  for (int i = 0; i < FASTLED_NUM_LEDS; i++)
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

void FastLED_subscribe_solid(String topic, String message) {
  if (message == "ON") {
    led_on();
  } else if (message == "OFF") {
    led_off();
  } else {
    led_solid(message);
  }
}
void FastLED_subscribe_animation(String topic, String message) {
  if (message == "OFF") {
    led_off();
  } else {
    led_animation_msg(message);
  }
}

void led_solid(String inputString) {
  String results[10];
  uint8_t count = explode(results, inputString, ',');

  if ( count != 3 ) return;

  //TODO: check values
  hue =        (uint8_t)rescale( (results[0]).toFloat() , 360.0 , 255.0 );
  saturation = (uint8_t)rescale( (results[1]).toFloat() , 100.0 , 255.0 );
  brightness = (uint8_t)rescale( (results[2]).toFloat() , 100.0 , 255.0 );

  // Output
  led_fillSolidColor();
  postNewState();
}

void postNewState() {
  mqtt_publish("lights/fastled", s+rescale(hue,255,360)+","+rescale(saturation,255,100)+","+rescale(brightness,255,100));
}

void led_fillSolidColor() {
  led_animation.enabled = false;
  led_base_hue.enabled = false;
  fill_solid( leds, FASTLED_NUM_LEDS, CHSV( hue, saturation, brightness));
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
    
    mqtt_publish("lights/fastled/animation", "sinelon");
  } else if (inputString == "rainbow") {
    currentPatternNumber = 1;

    led_animation.setInterval(20);
    led_base_hue.setInterval(20);
    
    led_animation.enabled = true;
    led_base_hue.enabled = true;
    
    mqtt_publish("lights/fastled/animation", "rainbow");
  } else if (inputString == "confetti") {
    currentPatternNumber = 2;
    
    led_animation.setInterval(10);
    led_base_hue.setInterval(50);
    
    led_animation.enabled = true;
    led_base_hue.enabled = true;

    mqtt_publish("lights/fastled/animation", "confetti");
  }
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, FASTLED_NUM_LEDS, 20);
  int pos = beatsin16(13,0,FASTLED_NUM_LEDS-1);
  leds[pos] += CHSV( baseHue, 255, 192);
}
void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, FASTLED_NUM_LEDS, 10);
  int pos = random16(FASTLED_NUM_LEDS);
  leds[pos] += CHSV( baseHue + random8(64), 255, 255);
}
void rainbow() {
  fill_rainbow( leds, FASTLED_NUM_LEDS, baseHue);
}

#else
void setup_FastLED() {}
void loop_FastLED() {}
#endif
