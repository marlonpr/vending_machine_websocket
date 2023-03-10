#include <EEPROM.h>

class Memory {
  private:
    int ssidAddress = 0;
    int passAddress;
  public:
    Memory();
    void init();
    void setSsid(String ssid);
    void setPass(String pass);
    String getSsid();
    String getPass();
};

Memory::Memory() {};

void Memory::init() {
  EEPROM.begin(96);
}

/*
  Sets the SSID in the EEPROM
  ssidAddress is the memory address for the number of characters in the SSID
  ssidAddress + 1 is the memory address for the first character of the SSID
  Each character is stored in the EEPROM one by one
  EEPROM.commit() is called to commit the changes to the EEPROM
  passAddress is set to the next position in the EEPROM after the SSID
*/
void Memory::setSsid(String ssid) {
  EEPROM.write(ssidAddress, ssid.length());
  for (int i = 0; i < ssid.length(); i++) {
    EEPROM.write(ssidAddress + i + 1, ssid.charAt(i));
  }
  EEPROM.commit();
  passAddress = ssidAddress + ssid.length() + 1;
}

void Memory::setPass(String pass) {
  EEPROM.write(passAddress, pass.length());
  for (int i = 0; i < pass.length(); i++) {
    EEPROM.write(passAddress + i  + 1, pass.charAt(i));
  }
  EEPROM.commit();
}

String Memory::getSsid() {
  String ssid = "";
  int length = EEPROM.read(ssidAddress);
  for (int i = 0; i < length; i++) {
    ssid += char(EEPROM.read(ssidAddress + i + 1));
  }
  passAddress = ssidAddress + ssid.length() + 1;
  return ssid;
}

String Memory::getPass() {
  String pass = "";
  int length = EEPROM.read(passAddress);
  for (int i = 0; i < length; i++) {
    pass += char(EEPROM.read(passAddress + i + 1));
  }
  return pass;
}
