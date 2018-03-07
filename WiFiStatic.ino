#if !USE_WIFI_MANAGER

Thread wifiThread = Thread();

void setup_WiFi() {
  WiFi.softAPdisconnect(true);
  Log.info(String("Connecting to Wifi SSID ") + ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Log.info(String("Connected with IP: ") + WiFi.localIP().toString() );

  wifiThread.onRun(wifiFunction);
  wifiThread.setInterval(60 * 1000);
  threadControl.add(&wifiThread);
}

void wifiFunction() {
  if (WiFi.status() != WL_CONNECTED) {
    LogWiFi.warn("Connection lost");
  }
}

#endif
