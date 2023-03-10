#include <WebSocketClient.h>

class WebSocket {
  private:
    WebSocketClient webSocketClient;
    WiFiClient client;
  public:
    WebSocket();
    void init();
    void data();
};

WebSocket::WebSocket(){}

void WebSocket::init() {
  char host[] = "192.168.1.13";
  char path[] = "/connectVending";

  if (client.connect(host, 5555)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
  }

  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
  }
}

void WebSocket::data() {
  String data;

  webSocketClient.sendData(GetMacAddress());
 
  webSocketClient.getData(data);
  if (data.length() > 0) {
    Serial.print("Received data: ");
    Serial.println(data);
    keypad.Dispatch(data, 1);
  }
  delay(3000);
}

