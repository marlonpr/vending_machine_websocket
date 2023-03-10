String splitString(String str, char separator) {
  // Split string by separator and join into a string
  String result = "";
  for (int i = 0; i < str.length(); i++) {
    if (str.charAt(i) != separator) {
      result += str.charAt(i);
    }
  }
  return result;
}

String GetMacAddress() {
  return splitString(WiFi.macAddress(), ':');
}

bool HasStoredWifi() {
  return (flash_ssid != "No ssid" && flash_pass != "No pass");
}

bool ConnectedToWifi() {
  return WiFi.status() == WL_CONNECTED;
}