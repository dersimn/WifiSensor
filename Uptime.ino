Thread uptimeThread = Thread();

// --------------------------------------------------------

void setup_uptime() {
  uptimeThread.onRun(uptime_func);
  uptimeThread.setInterval(1000);
  threadControl.add(&uptimeThread);
}

void uptime_func() {
  static uint32_t uptime = 0;
  char uptime_buf[12]; // 2^32 = 4.294.967.296 ^= 10 characters +2 spare
  
  String(uptime++).toCharArray(uptime_buf, 12);
  mqttClient.publish(mqtt_uptime_topic, uptime_buf);
}

