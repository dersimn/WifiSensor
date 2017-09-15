#include <OneWire.h>
#include <DallasTemperature.h>

Thread meassureThread = Thread();
ThreadRunOnce outputThread = ThreadRunOnce();

//OneWire oneWire(ONE_WIRE_BUS);
OneWire oneWire(ONE_WIRE_BUS, PULLUP_PIN);
DallasTemperature sensors(&oneWire);

DeviceAddress devices[MAX_DEVICES];
String devices_str[MAX_DEVICES];
uint8_t device_count;

void setup_dallas() {
  Serial.println("Detecting Dallas Temperature ICs");
  sensors.begin();
  Serial.print("Locating devices...");
  Serial.print("Found ");
  device_count = sensors.getDeviceCount();
  Serial.print(device_count, DEC);
  Serial.println(" devices.");

  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode())
    Serial.println("ON");
  else
    Serial.println("OFF");

  // by index
  for (uint8_t i = 0; i < device_count; i++) {
    if (!sensors.getAddress(devices[i], i)) Serial.print("Unable to find address for Device ");Serial.println(i);
  }

  // show the addresses we found on the bus
  for (uint8_t i = 0; i < device_count; i++) {
    Serial.print("Device ");Serial.print(i);Serial.print(" Address: ");
    devices_str[i] = stringPrintAddress(devices[i]);
    Serial.print(devices_str[i]); Serial.println();
  }

  // set the resolution per device
  for (uint8_t i = 0; i < device_count; i++) {
    sensors.setResolution(devices[i], TEMPERATURE_PRECISION);
  }
  for (uint8_t i = 0; i < device_count; i++) {
    Serial.print("Device ");Serial.print(devices_str[i]);Serial.print(" Resolution: ");
    Serial.print(sensors.getResolution(devices[i]), DEC); 
    Serial.println();
  }

  // Non-blocking temperature reads
  sensors.setWaitForConversion(false);

  meassureThread.onRun(measure_func);
  meassureThread.setInterval(DALLAS_INTERVAL);
  threadControl.add(&meassureThread);

  outputThread.onRun(output_func);
  outputThread.enabled = false;
  threadControl.add(&outputThread);
}

void measure_func() {
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
  outputThread.setRunOnce(2000);
}
void output_func() {
  char msg[50];
  // print the device information
  for (uint8_t i = 0; i < device_count; i++) {
    float tempC = sensors.getTempC(devices[i]);
    Serial.print("Device ");Serial.print(devices_str[i]);Serial.print(" has ");Serial.print(tempC);Serial.println(" *C");
  
    String topic_str, msg_str;
    topic_str = String(DALLAS_BASETOPIC); 
    topic_str += devices_str[i];
    msg_str = String(tempC, 2);
    
    mqtt_publish(topic_str, msg_str);
  }
}

String stringPrintAddress(DeviceAddress deviceAddress) {
  String tmp;
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) tmp += String("0");
    tmp += String(deviceAddress[i], HEX);
  }
  return tmp;
}
