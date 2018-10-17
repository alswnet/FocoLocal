#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "ALSW";
const char* password = "2526-4897";
const char* mqtt_server = "broker.mqtt-dashboard.com";

int Foco[] = {0, 4, 16};
int Boton[] = {2, 14, 12};
boolean EstadoFoco[] = {false, false, false};
boolean EstadoAnterior[] = {true, true, true};
int LedIndicador = 5;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(LedIndicador, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(Foco[i], OUTPUT);
    pinMode(Boton[i], INPUT);
  }
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LedIndicador, 1);
    delay(500);
    digitalWrite(LedIndicador, 0);
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje de[");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  if ((char)payload[0] == 'a' || (char)payload[0] == 'A') {
    EstadoFoco[0] = true;
  }
  else if ((char)payload[0] == 'b' || (char)payload[0] == 'B') {
    EstadoFoco[0] = false;
  }
  else if ((char)payload[0] == 'c' || (char)payload[0] == 'C') {
    EstadoFoco[1] = true;
  }
  else if ((char)payload[0] == 'd' || (char)payload[0] == 'D') {
    EstadoFoco[1] = false;
  }
  else if ((char)payload[0] == 'e' || (char)payload[0] == 'E') {
    EstadoFoco[2] = true;
  }
  else if ((char)payload[0] == 'f' || (char)payload[0] == 'F') {
    EstadoFoco[2] = false;
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266ClientALSW")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("ALSWout", "hello world");
      // ... and resubscribe
      client.subscribe("ALSWin");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  ActualizarBotones();
  ActualizarFoco();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

 long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "Foco ALSW F0-%d F1-%d, F2-%d",EstadoFoco[0],EstadoFoco[1],EstadoFoco[2]);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("ALSWout", msg);
  }

  digitalWrite(LedIndicador, 1);
  delay(250);
  digitalWrite(LedIndicador, 0);
  delay(250);
}

void ActualizarFoco() {
  for (int i = 0; i < 3; i++) {
    if ( EstadoFoco[i] != EstadoAnterior[i]) {
      EstadoAnterior[i] = EstadoFoco[i];
      if (EstadoFoco[i]) {
        digitalWrite(Foco[i] , 1);
        Serial.print("Encender " );
      }
      else {
        digitalWrite(Foco[i] , 0);
        Serial.print("Apagar " );
      }
      Serial.println(i);
    }
  }
}

void ActualizarBotones() {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(Boton[i]) == 0) {
      EstadoFoco[i] = !EstadoFoco[i];
      delay(50);
      while (digitalRead(Boton[i]) == 0) {
      }
    }
  }
}
