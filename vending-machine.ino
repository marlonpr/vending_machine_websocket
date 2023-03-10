#include <ArduinoJson.h>
#include <WebServer.h>

// Gobals
String flash_ssid;
String flash_pass;
#include "tools.h"
#include "screen.h"
Screen screen;

 byte resetButtonPin = 33;
#include "keypad.h"
TransistorsKeypad keypad;
#include "http.h"
 HttpRequest warelan_app("http://petrum.warelan.com/api/v1/giveaway_claims");
//HttpRequest warelan_app("http://192.168.1.22:3000/api/v1/giveaway_claims");
#include "buzzer.h"
Buzzer buzzer(25);
#include "barcode.h"
#include "memory.h"
Memory memory;
#include "wifi.h"
#include "restServer.h"
#include "AP.h"
#include "websocket.h"
WebSocket websocket;

void setup() {
  Serial.begin(115200);

  IniUsb();
  pinMode(resetButtonPin, INPUT);

  keypad.init();
  memory.init();
  screen.init();
  

  screen.print("Initializing...");
  screen.draw();
  Serial.println("WARELAN VENDING MACHINE");
  
   
  if (digitalRead(resetButtonPin) == HIGH) {
    Serial.println("Reseting stored SSID");
    SetSession("No ssid", "No pass");
  }
  

  GetSession(&flash_ssid, &flash_pass);

  if (flash_ssid != "No ssid" && flash_pass != "No pass") {
    Serial.println("Attempting to connect with stored SSID");
    AttemptConnection();
    websocket.init();
  } else {
    Serial.println("No stored session found...");
    screen.printPersistant("Falta configurar");    
    WiFi_AP();
    InitServer();
  }
}

void loop() {
  screen.draw();
  if (ConnectedToWifi()) {
    screen.deactivatePersistant();
    websocket.data();
  } else if (HasStoredWifi()) {
    screen.deactivatePersistant();
    AttemptConnection();
  } else {
    server.handleClient();
  }
}
