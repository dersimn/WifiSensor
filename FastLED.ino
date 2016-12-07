#include <FastLED.h>
#include <NeoPixelBus.h>

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(NUM_LEDS);
CRGB leds[NUM_LEDS];
Thread led_show = Thread();
Thread led_animation = Thread();

uint8_t baseHue = 0;

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
  baseHue++;
  sinelon();
}

void led_input(String topic, String inputString) {
  if (topic == LED_SOLID_TOPIC) {
    led_solid(inputString);
  }
  if (topic == LED_ANIMATION_TOPIC) {
    led_animation_msg(inputString);
  }
}

void led_solid(String inputString) {
  led_animation.enabled = false;
  Serial.println("input");
  String results[10];
  uint8_t count = explode(results, inputString, ',');
    for (uint8_t i = 0; i < count; i++) {
    Serial.print("i = ");Serial.println(i);
    Serial.print("result = ");Serial.println(results[i]);
  }
  
  // hue 0-360
  float raw_hue = (results[0]).toFloat();
  uint8_t hue;
  if (raw_hue < 1.0) { 
    hue = 0;
  } else {
    hue = raw_hue / 360.0 * 255.0;
  }
  //Serial.print("hue = ");Serial.println(hue);
  // saturation 0-100
  uint8_t saturation = (results[1]).toFloat() / 100.0 * 255.0;
  //Serial.print("saturation = ");Serial.println(saturation);
  // brightness 0-100
  float raw_brightness = (results[2]).toFloat();
  uint8_t brightness;
  if (raw_brightness < 1.0) { 
    brightness = 0;
  } else {
    brightness = raw_brightness / 100.0 * 255.0;
  }
  //Serial.print("brightness = ");Serial.println(brightness);
  Serial.println("conversion");
  fill_solid( leds, NUM_LEDS, CHSV( hue, saturation, brightness));

  String state = String(hue / 255.0 * 360.0);
  state += ",";
  state += String(saturation / 255.0 * 100.0);
  state += ",";
  state += String(brightness / 255.0 * 100.0);
  char msg[50];
  state.toCharArray(msg, 50);

  mqttClient.publish(LED_STATE_TOPIC, msg);
}
void led_animation_msg(String inputString) {
  if (inputString == "sinelon") {
    led_animation.enabled = true;
    mqttClient.publish(LED_STATE_TOPIC, "sinelon");
  }
  if (inputString == "OFF") {
    led_animation.enabled = false;
    fill_solid( leds, NUM_LEDS, CRGB::Black);
    mqttClient.publish(LED_STATE_TOPIC, "OFF");
  }
}

void sinelon() {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS-1);
  leds[pos] += CHSV( baseHue, 255, 192);
}

