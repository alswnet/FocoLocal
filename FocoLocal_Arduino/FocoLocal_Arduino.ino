int Led[] = {13, 12, 11};

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(Led[i] , OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char Letra = Serial.read();
    if (Letra == 'a') {
      digitalWrite(Led[0], 1);
    }
    else if (Letra == 'b') {
      digitalWrite(Led[0], 0);
    }
    else if (Letra == 'c') {
      digitalWrite(Led[1], 1);
    }
    else if (Letra == 'd') {
      digitalWrite(Led[1], 0);
    }
    else if (Letra == 'e') {
      digitalWrite(Led[2], 1);
    }
    else if (Letra == 'f') {
      digitalWrite(Led[2], 0);
    }
  }
}
