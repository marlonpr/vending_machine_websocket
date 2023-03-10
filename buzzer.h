class Buzzer {
  private:
    int pin;
  public:
    Buzzer(int pin);
    void beep(int duration);
    void errorBeep();
};

Buzzer::Buzzer(int _pin) {
  pin = _pin;
  pinMode(pin, OUTPUT);
}

void Buzzer::beep(int duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
}

void Buzzer::errorBeep() {
  beep(300);
  delay(300);
  beep(300);
  delay(300);
  beep(300);
}