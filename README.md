## Default settings

Currently the MQTT server IP is hard-coded in `default_config.h`.

You can also choose an prefix for your MQTT topics. By default the topics are (without spaces)

	dersimn/status/	WifiSensor1234567/	lights/fastled
	dersimn/status/	WifiSensor1234567/	lights/fastled/animation
	dersimn/status/	WifiSensor1234567/	lights/mosfetled
	dersimn/set/	  ^ same ^ topics ^ with ^ set ^

	dersimn/status/	WifiSensor1234567/	temperature/dht
	dersimn/status/	WifiSensor1234567/	temperature/dallas/<ID>

	dersimn/status/	WifiSensor1234567/	humidity/dht

	dersimn/status/	WifiSensor1234567/	maintenance/uptime 
	dersimn/status/	WifiSensor1234567/	maintenance/uptime/ms 

using `ESP.getChipId()` to distinguish multiple devices.

## Pinout

### Wemos

	D4: AM2302
	D1, D2 (I2C): BMP180, TSL2561
	D3: Dallas
	D0: Pullup
	Rx: FastLED
	D5: mosfetLED

## Credits

This project follows [Oliver "owagner" Wagner](https://github.com/owagner)'s architectural proposal for an [mqtt-smarthome](https://github.com/mqtt-smarthome/mqtt-smarthome).