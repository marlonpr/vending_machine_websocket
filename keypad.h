class TransistorsKeypad {
  private:
    int pinByNumber[10] = {27,15,15,2,2,4,4,16,16,27};
    int relevatorPin = 26;
  public:
    void init();
    void Dispatch(String, int);
};

void TransistorsKeypad::init() {
  pinMode(relevatorPin, OUTPUT);
  for(int i = 0; i<10; i++){
    digitalWrite(pinByNumber[i],LOW);
    pinMode(pinByNumber[i],OUTPUT);
  }
}

void TransistorsKeypad::Dispatch(String position, int amount) {
  int digitCount = position.length(); // How many digits are in the position?
  bool relevatorState = LOW;
  for (int i = 0; i < amount; i++) {
    if (i > 0) {
      delay(12000);
    }

    for (int j = 0; j < digitCount; j++) {
      int digit = position.charAt(j) - '0';
      bool relevatorStateNeeded = ((digit % 2 == 0 || digit == 0) ? HIGH : LOW);
      if (relevatorState != relevatorStateNeeded) {
        digitalWrite(relevatorPin, relevatorStateNeeded);
        relevatorState = relevatorStateNeeded;
      }
      digitalWrite(pinByNumber[digit], HIGH);
      delay(300);
      digitalWrite(pinByNumber[digit], LOW);
      delay(300);
    }
  }
  digitalWrite(relevatorPin, LOW);
  relevatorState = LOW;
  delay(12000);
}


class RelaysKeypad {
  private:  
    int relays[10] = {27, 4, 32, 33, 26, 14, 12, 2, 15, 25};
    // int relays[10] = {27, 4, 12, 32, 2, 33, 15, 26, 25, 14 };
  public:
    void init();
    void Dispatch(String, int);
};

void RelaysKeypad::init() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(relays[i],LOW);
    pinMode(relays[i], OUTPUT);
  }
}

void RelaysKeypad::Dispatch(String position, int amount) {
  int digitCount = position.length();
  char digits[digitCount];
  for (int i = 0; i < sizeof(digits); i++) {
    digits[i] = position[i];
  }
  for (int i = 0; i < amount; i++) {
    if (i > 0) {
      delay(8000);
    }
    for (int j = 0; j < digitCount; j++) {
      int digit = int(digits[j]) - '0';
      digitalWrite(relays[digit], HIGH);
      delay(500);
      digitalWrite(relays[digit], LOW);
      delay(500);
    }
  }
  delay(8000);
}
