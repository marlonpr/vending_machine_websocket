#include <hiduniversal.h>
#include <usbhid.h>
#include <Usb.h>
#include <usbhub.h>
#include <hidboot.h>
#include <SPI.h>

String scanCode = "";
bool isScanning = false;
bool scanCodeReady = false;
USB Usb;
HIDUniversal Hid(&Usb);

class KbdRptParser : public KeyboardReportParser {
 protected:
  void OnKeyDown  (uint8_t mod, uint8_t key);
  void OnKeyPressed(uint8_t key);
};

KbdRptParser Prs;

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key) {
  uint8_t c = OemToAscii(mod, key);
  Serial.println("KeyDown: " + String(c) + " key: " + String(key) + " mod:" + String(mod));
  if (c) {
    OnKeyPressed(c);
  }
}

void KbdRptParser::OnKeyPressed(uint8_t key) {
  String pressedKey = String((char)key);
  String headChar = "A";
  String tailChar = "Z";
  if (pressedKey == headChar) {
    isScanning = true;
    buzzer.beep(300);
    screen.print("Leyendo codigo");
  }

  if (isScanning) {
    scanCode += pressedKey;
  }

  if (pressedKey == tailChar) {
    isScanning = false;
    scanCodeReady = true;
  }
}

void IniUsb() {
  #if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");
  if (Usb.Init() == -1) {
    Serial.println("OSC did not start");
  }
  delay( 200 );
  Hid.SetReportParser(0, &Prs);
}

void HandleBarcode(String barcode) {
  int code;
  DynamicJsonDocument response = warelan_app.validateBarcode(barcode, &code);
  Serial.println("Server response:");
  serializeJsonPretty(response, Serial);
  if (response["success"]) {
    screen.printImportant("Entregando productos");
    JsonArray products = response["products"].as<JsonArray>();
    for (JsonVariant product : products) {
      keypad.Dispatch(product["position"].as<String>(), product["amount"].as<int>());
    }
    screen.print("Gracias por tu preferencia! :)");
  } else {
    Serial.println("Products unavailable");
    Serial.println("ERROR CODE: " + String(code));
    if ( code == 422  ) {
      String output;
      serializeJson(response, output);
      Serial.println(output);
      screen.printImportant(response["errors"]["error_translated"]);
    } else {
      screen.printImportant("Intente de nuevo");
    }
    buzzer.errorBeep();
  }
  delay(2000);
}

void ReadBardcode() {
  delay(1);
  Usb.Task();
  if(scanCodeReady) {
    Serial.println(scanCode);
    HandleBarcode(scanCode);
    scanCode = "";
    scanCodeReady = false;
  }
}
