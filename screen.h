#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

class Screen {
  private:
    uint8_t lcdColumns = 16;
    uint8_t lcdRows = 2;
    unsigned long updateInterval = 5000;
    unsigned long lastUpdate = 0;
    bool shouldShowWelcomeMessage = true;
    bool needUpdate = false;
    bool showCustomMessage = false;
    bool welcomeMessageShown = false;
    bool persistantActivated = false;
    String currentMessage;
    void drawMessage(String);
  public:
    Screen();
    void init();
    void print(String);
    void printImportant(String);
    void welcomeMessage();
    void printPersistant(String);
    void deactivatePersistant();
    void draw();
};

Screen::Screen() {

}

void Screen::init() {
  lcd.init();
  lcd.backlight();
}

void Screen::print(String text) {
  currentMessage = text;
  needUpdate = true;
  welcomeMessageShown = false;
}

void Screen::printImportant(String text) {
  print(text);
  draw();
}

void Screen::welcomeMessage() {
  shouldShowWelcomeMessage = true;
  needUpdate = true; 
  currentMessage = "Bienvenido a Warelan VM";
  welcomeMessageShown = true;
}

void Screen::printPersistant(String text) {
  lcd.clear();
  lcd.setCursor(0,0);
  drawMessage(text);
  persistantActivated = true;
}

void Screen::draw() {
  shouldShowWelcomeMessage = (millis() - lastUpdate > updateInterval);

  if (shouldShowWelcomeMessage && !needUpdate && !welcomeMessageShown) {
    welcomeMessage();
  }

  if (needUpdate && !persistantActivated) {
    lcd.clear();
    lcd.setCursor(0,0);
    drawMessage(currentMessage);
    lastUpdate = millis();
    needUpdate = false;
  }
}

void Screen::deactivatePersistant() {
  persistantActivated = false;
}

void Screen::drawMessage(String message) {
  if (message.length() > lcdColumns){
    String firstRow = message.substring(0, lcdColumns);
    String secondRow = message.substring(lcdColumns, message.length());
    if (firstRow.charAt(firstRow.length() - 1) != ' ') {
      int lastSpace = firstRow.lastIndexOf(' ');
      secondRow = firstRow.substring(lastSpace + 1, firstRow.length()) + secondRow;
      firstRow = firstRow.substring(0, lastSpace);
    } 
    lcd.print(firstRow);
    lcd.setCursor(0,1);
    lcd.print(secondRow);
  }  else {
    lcd.print(message);
  }
}
