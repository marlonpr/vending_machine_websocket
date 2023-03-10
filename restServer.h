WebServer server(80);

void getNetworks() {
  Serial.println("Scanning networks...");
  byte ssidCount = WiFi.scanNetworks();
  DynamicJsonDocument response(512);
  JsonArray networks = response.createNestedArray("networks");

  for (int i=0; i<ssidCount; i++) {
    DynamicJsonDocument network(128);
    network["ssid"] = WiFi.SSID(i);
    network["signal_strength"] = WiFi.RSSI(i);
    networks.add(network);
  }
  
  String buf;
  serializeJsonPretty(response, buf);
  Serial.println("Networks found:");
  Serial.println(buf);
  server.send(200, F("application/json"), buf);
}

void setConnection() {
  flash_ssid = server.arg(String("ssid"));
  flash_pass = server.arg(String("password"));

  AttemptConnection();

  if (!ConnectedToWifi()) {
      Serial.println("\nIncorrect password or no IP assigned, try again");
      server.send(400, F("text/plain"), "Incorrect password or no IP assigned, try again");
      flash_ssid = "No ssid";
      flash_pass = "No pass";
  }  

  if (ConnectedToWifi()) {
    Serial.println(WiFi.localIP());
    SetSession(flash_ssid, flash_pass);
    Serial.println("Connected...");
    server.send(200, F("application/json"), "Connected to wifi");
  }
}

// void setWarelanServer() {
//   if (server.arg(String("path")) == "") {
//     server.send(400, F("text/plain"), "No path provided");
//     return;
//   } else {
//     warelan_path = server.arg(String("path"));
//     Serial.println("Warelan path set to: " + warelan_path);
//     server.send(200, F("application/json"), "Warelan path set to " + warelan_path);
//   }
// }

void InitServer() {   
  server.on("/networks", HTTP_GET, getNetworks);
  server.on("/connect", HTTP_POST, setConnection);
  // server.on("/warelan_path", HTTP_POST, setWarelanServer);
}
