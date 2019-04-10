int Foco = 13;
boolean EstadoFoco = false;

void setup() {
  pinMode(Foco, OUTPUT);
  digitalWrite(Foco, EstadoFoco);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char Letra = Serial.read();
    if (Letra == 'H' || Letra == 'h') {
      EstadoFoco = true;
    }
    else if (Letra == 'L' || Letra == 'l') {
      EstadoFoco = false;
    }
    digitalWrite(Foco, EstadoFoco);
  }
  if (EstadoFoco) {
    Serial.println("H");
  }
  else {
    Serial.println("L");
  }
  delay(1000);
}
