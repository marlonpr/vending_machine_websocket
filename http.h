#include <HTTPClient.h>

struct HttpResponse {
  int code;
  DynamicJsonDocument message;
};

class HttpRequest {
  private:
    String path;
    DynamicJsonDocument unparseResponse(HTTPClient*);
    HttpResponse post(DynamicJsonDocument);
  public:
    HttpRequest(String);
    void setPath(String);
    String getPath();
    DynamicJsonDocument validateBarcode(String, int*);
    void webSerial(String);
};

HttpRequest::HttpRequest(String _path) {
  path = _path;
};

String HttpRequest::getPath() {
  return path;
}

void HttpRequest::setPath(String _path) {
  path = _path;
}

HttpResponse HttpRequest::post(DynamicJsonDocument body) {
  HTTPClient http;
  http.setTimeout(15000);
  http.begin(path.c_str());
  http.addHeader("Content-Type", "application/json");
  String requestBody;
  serializeJson(body, requestBody);

  int responseCode = http.POST(requestBody);

  HttpResponse httpResponse = {responseCode, unparseResponse(&http) };
  http.end();
  return httpResponse;
}

DynamicJsonDocument HttpRequest::unparseResponse(HTTPClient *http) {
  DynamicJsonDocument jsonResponse(3072);
  
  String response = http->getString();

  deserializeJson(jsonResponse, response);
  
  return jsonResponse;
}

DynamicJsonDocument HttpRequest::validateBarcode(String barcode, int* code) {
  DynamicJsonDocument body(512);
  body["barcode"] = barcode;
  body["serial_number"] =  GetMacAddress();

  HttpResponse httpResponse = post(body);
  *code = httpResponse.code;
  return httpResponse.message;
}

void HttpRequest::webSerial(String message) {
  DynamicJsonDocument body(512);
  body["message"] = message;
  post(body);
}
