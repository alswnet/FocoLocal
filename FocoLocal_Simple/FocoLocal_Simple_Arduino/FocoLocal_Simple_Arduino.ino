int Foco = 13;
int Boton = 2;
int EstadoBotonAnterior = 0;
boolean EstadoFoco = false;

void setup() {
  pinMode(Foco, OUTPUT);
  pinMode(Boton, INPUT_PULLUP);
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

  int EstadoBoton = digitalRead( Boton );
  if (EstadoBoton  != EstadoBotonAnterior) {
    EstadoBotonAnterior = EstadoBoton;
    if (EstadoBoton) {
      EstadoFoco = false;
    } else {
      EstadoFoco = true;
    }
    digitalWrite(Foco, EstadoFoco);
  }

  if (EstadoFoco) {
    Serial.println("H");
  }
  else {
    Serial.println("L");
  }
  delay(500);
}
