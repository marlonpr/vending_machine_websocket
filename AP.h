void WiFi_AP() {
  Serial.println("Setting up AP mode");
  String ssid = String("WARELAN_VM_" + GetMacAddress());
  WiFi.softAP(ssid.c_str(), "Warelan01!");
  IPAddress IP = WiFi.softAPIP();
  Serial.println(IP);
  server.begin();
}
