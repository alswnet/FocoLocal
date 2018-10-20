#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define NoConectado 0
#define Conectado 1
#define MqttNo 2
#define Mqtt 3

const char* ssid = "ALSW";
const char* password = "2526-4897";
const char* mqtt_server = "broker.mqtt-dashboard.com";

int Foco[] = {0, 4, 13};
int Boton[] = {2, 14, 12};
boolean EstadoFoco[] = {false, false, false};
boolean EstadoAnterior[] = {true, true, true};
int LedIndicador = 5;
int EstadoWifi = 0;
int EstadoWifiAnterior = -1;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
long LedTiempo = 0;
char msg[50];

void setup() {
  pinMode(LedIndicador, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(Foco[i], OUTPUT);
    pinMode(Boton[i], INPUT);
  }
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje de[");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  int FocoActual = -1;
  if ((char)payload[0] == 'a' || (char)payload[0] == 'A') {
    FocoActual = 0;
  }
  else if ((char)payload[0] == 'b' || (char)payload[0] == 'B') {
    FocoActual = 1;
  }
  else if ((char)payload[0] == 'c' || (char)payload[0] == 'C') {
    FocoActual = 2;
  }
  else if ((char)payload[0] == 'f' || (char)payload[0] == 'F') {
    if ((char)payload[1] == '1') {
      for (int i = 0; i < 3; i++) {
        EstadoFoco[i] = true;
      }
    }
    else if ((char)payload[1] == '0' ) {
      for (int i = 0; i < 3; i++) {
        EstadoFoco[i] = false;
      }
    }
  }
  else {
    return;
  }

  if ((char)payload[1] == '1') {
    EstadoFoco[FocoActual] = true;
  }
  else if ((char)payload[1] == '0' ) {
    EstadoFoco[FocoActual] = false;
  }
}

void loop() {
  ActualizarBotones();
  ActualizarFoco();
  ActualizarRed();
  ActualizarLed();
}

void ActualizarLed() {
  digitalWrite(LedIndicador, 1);
  delay(100);
  digitalWrite(LedIndicador, 0);
  delay(100);
  if (EstadoWifi != EstadoWifiAnterior) {
    EstadoWifiAnterior =  EstadoWifi;
    Serial.print("Estado Wifi ");
    Serial.println(EstadoWifi);
  }
  switch (EstadoWifi) {
    case NoConectado:
      digitalWrite(LedIndicador, 1);
      delay(1000);
      digitalWrite(LedIndicador, 0);
      delay(1000);
      break;
    case Conectado:
      digitalWrite(LedIndicador, 1);
      delay(100);
      digitalWrite(LedIndicador, 0);
      delay(500);
      break;
    case Mqtt:
      digitalWrite(LedIndicador, 1);
      delay(100);
      digitalWrite(LedIndicador, 0);
      delay(100);
      break;
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266ClientALSW")) {
      Serial.println("connected");
      client.subscribe("ALSWin");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void ActualizarRed() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect("ESP8266ClientALSW")) {
        EstadoWifi = Mqtt;
        Serial.println("connected");
        client.subscribe("ALSWin");
      } else {
        EstadoWifi = Conectado;
        Serial.print("failed, rc=");
        Serial.print(client.state());
      }
    }
  }
  else {
    EstadoWifi = NoConectado;
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 5000 && EstadoWifi == Mqtt ) {
    lastMsg = now;
    snprintf (msg, 75, "Foco ALSW F0-%d F1-%d, F2-%d", EstadoFoco[0], EstadoFoco[1], EstadoFoco[2]);
    Serial.print("Mensaje Publicado: ");
    Serial.println(msg);
    client.publish("ALSWout", msg);
  }
}


void ActualizarFoco() {
  for (int i = 0; i < 3; i++) {
    if ( EstadoFoco[i] != EstadoAnterior[i]) {
      EstadoAnterior[i] = EstadoFoco[i];
      Serial.print("Foco-");
      Serial.print(i);
      if (EstadoFoco[i]) {
        digitalWrite(Foco[i] , 1);
        Serial.print(" Encender " );
      }
      else {
        digitalWrite(Foco[i] , 0);
        Serial.print(" Apagar " );
      }
      Serial.println();
      return;
    }
  }
}

void ActualizarBotones() {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(Boton[i]) == 0) {
      EstadoFoco[i] = !EstadoFoco[i];
      while (digitalRead(Boton[i]) == 0) {
        delay(50);
      }
      delay(50);
    }
  }
}
