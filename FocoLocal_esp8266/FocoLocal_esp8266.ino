#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "ALSW";
const char* password = "2526-4897";
const char* mqtt_server = "broker.mqtt-dashboard.com";

int Foco[] = {0, 4, 16};
int Boton[] = {2, 14, 12};
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
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("ALSWout", "hello world");
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
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("ALSWout", msg);
  }
  digitalWrite(LedIndicador, 1);
  delay(250);
  digitalWrite(LedIndicador, 0);
  delay(250);
}

void ActualizarBotones() {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(Boton[i]) == 0) {
      Serial.print("A");
    }
    else {
      Serial.print("B");
    }
  }
  Serial.println();

}
