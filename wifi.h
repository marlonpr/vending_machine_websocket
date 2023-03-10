#include <WiFi.h>

void SetSession(String ssid, String pass) {
  Serial.println("Writing WiFi configuration to EEPROM");
  memory.setSsid(ssid);
  memory.setPass(pass);
}

void GetSession(String *ssid, String *pass){
  *ssid = memory.getSsid();
  *pass = memory.getPass();
}

void AttemptConnection() {  
  screen.print("Conectando a " + flash_ssid);
  screen.draw();
  Serial.println("Attempting connection");
  Serial.println("SSID: " + flash_ssid);
  Serial.println("Password: " + flash_pass);
  Serial.println("---------------------");
  char ssidChar[flash_ssid.length()];
  char passwordChar[flash_pass.length()];
  flash_ssid.toCharArray(ssidChar, flash_ssid.length() + 1);
  flash_pass.toCharArray(passwordChar, flash_pass.length() + 1);
  WiFi.begin(ssidChar, passwordChar);
  WiFi.setHostname("WARELAN-VM");
  for (int i = 0; i < 10; i++) {
    Serial.print(".");
    if (ConnectedToWifi()) {
      Serial.println("\nConnected to " + flash_ssid);
      screen.print("Conectado a " + flash_ssid);
      delay(2000);
      screen.welcomeMessage();
      break;
    }
    delay(1000);
  }
}
