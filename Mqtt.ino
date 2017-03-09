void setup_mqtt() {
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(mqtt_callback);
}

void loop_mqtt() {
  if (!mqttClient.connected()) {
    mqtt_reconnect();
  }
  mqttClient.loop();
}

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(mqtt_id)) {
      Serial.println("connected");
      mqtt_subscribe();
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqtt_subscribe() {
  mqttClient.subscribe(LED_SOLID_TOPIC);
  mqttClient.subscribe(LED_ANIMATION_TOPIC);
  mqttClient.subscribe(MOSFETLED_COMMAND_TOPIC);
}

void mqtt_callback(char* topic_char, byte* payload, unsigned int length) {
  String topic = String(topic_char);
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String inputString = "";
  for (int i = 0; i < length; i++) {
    inputString += (char)payload[i];
  }
  Serial.println(inputString);

  if (topic == LED_SOLID_TOPIC ||
      topic == LED_ANIMATION_TOPIC)
  {
    led_input(topic, inputString);
  }

  if (topic == MOSFETLED_COMMAND_TOPIC) {
    mosfetLED_input(topic, inputString);
  }
}

void mqtt_publish(String topic, String message) {
  char topic_char[100];
  char msg_char[500];

  topic.toCharArray(topic_char, 100);
  message.toCharArray(msg_char, 500);
  
  mqttClient.publish(topic_char, msg_char);    
}

