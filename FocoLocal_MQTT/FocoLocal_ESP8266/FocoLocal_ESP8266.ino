#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <MQTT.h>

#define NoWifi 0
#define Wifi 1
#define MqttWifi 2

int Foco[] = {0, 4, 13};
int Boton[] = {2, 14, 12};
boolean EstadoFoco[] = {false, false, false};
boolean EstadoAnterior[] = {true, true, true};
long IntervaloLed = 0;
long TiempoFoco = 0;
long TiempoMqttReconectar = 0;
long TiempoMensaje = 0;
boolean EstadoLed = false;
boolean ConectarMqtt = false;
boolean ConectarWifi = false;
int EstadoWifi = NoWifi;

const char* ssid =  "ALSW";
const char* password = "2526-4897";
//const char* ssid =  "ALSW";
//const char* password = "2526-4897";
const String TopicoFocoMensaje[4] = {"/ALSW/foco/mensaje/1", "/ALSW/foco/mensaje/2", "/ALSW/foco/mensaje/3"};
const String TopicoFocoEstado[4] = {"/ALSW/foco/estado/1", "/ALSW/foco/estado/2", "/ALSW/foco/estado/3"};

const char* BrokerMQTT = "broker.shiftr.io";
int PuertoMQTT = 80;
const char* ClienteIDMQTT = "FocoALSW";
const char* UsuarioMQTT = "chepecarlos";
const char* ContrasenaMQTT = "secretoespecial";

const char* usuario = "chepecarlos";
const char* pass = "secretoespecial";
const char* mqtt_server = "broker.shiftr.io";
char msg[50];

WiFiClient ESP_Cliente;

MQTTClient client;

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(Foco[i], OUTPUT);
    pinMode(Boton[i], INPUT);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Iniciando :) ");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Condigurar MQTT");
  client.begin("broker.shiftr.io", ESP_Cliente);
  client.onMessage(PreguntarMqtt);

  ActualizarWifi();
}

void loop() {
  ActualizarFoco();
  ActualizarBotones();
  ActualizarWifi();
  ActualizarMQTT();
  EnviarMQTT();
  ActualizarLed();
}

void ActualizarLed() {
  switch (EstadoWifi) {
    case NoWifi:
      IntervaloLed = 1000;
      break;
    case Wifi:
      IntervaloLed = 500;
      break;
    case MqttWifi:
      IntervaloLed = 50;
      break;
  }
  if (TiempoFoco + IntervaloLed < millis()) {
    TiempoFoco = millis();
    EstadoLed  = !EstadoLed;
    if (EstadoLed) {
      digitalWrite(LED_BUILTIN, 1);
    } else  {
      digitalWrite(LED_BUILTIN, 0);
    }
  }
}

void EnviarMQTT() {
  if (TiempoMensaje + 3000 < millis()) {
    TiempoMensaje = millis();
    //snprintf (msg, 75, "%l", EstadoFoco);
    Serial.print("Estado");
    for (int i = 0; i < 3; i++) {
      if (EstadoFoco[i]) {
        msg[0] = '1';
      }
      else {
        msg[0] = '0';
      }
      Serial.print(" F");
      Serial.print(i + 1);
      Serial.print("-");
      Serial.print(msg);
      client.publish(TopicoFocoEstado[i], msg);
    }
    Serial.println();
  }
}

void ActualizarMQTT() {
  /*if (!ConectarMqtt ) {
    client.begin("broker.shiftr.io", ESP_Cliente);
    client.onMessage(PreguntarMqtt);
    ConectarMqtt = true;
    }
  */
  client.loop();
  delay(10);

  if (!client.connected()) {
    if (WiFi.status() == WL_CONNECTED) {
      if (TiempoMqttReconectar + 500 <  millis()) {
        TiempoMqttReconectar =  millis();
        Serial.print("Activando mqtt");
        if (!client.connect(ClienteIDMQTT, UsuarioMQTT, ContrasenaMQTT)) {//Cambiar por un nombre unico para no tener poblemas
          Serial.println("... Error");
          return;
        }
        Serial.println("... Conectado");
        Serial.println("Subcribiendo a:");
        for (int i = 0; i < 3; i++) {
          Serial.println(TopicoFocoMensaje[i]);
          client.subscribe(TopicoFocoMensaje[i]);
        }
        EstadoWifi = MqttWifi;
      }
    }
  }
}

int SinWifi = 0;
void ActualizarWifi() {
  //Configurando Wifi
  /* if (!ConectarWifi) {
     ConectarWifi = true;
     WiFi.mode(WIFI_STA);
     WiFi.begin(ssid, password);
    }
  */

  if (WiFi.status() != WL_CONNECTED) {
    EstadoWifi = NoWifi;
    Serial.print("No Wifi ");
    Serial.println(SinWifi++);
    delay(500);
    return;
  }
  else if (EstadoWifi != Wifi) {
    EstadoWifi = Wifi;
    SinWifi = 0;
  }

}

void ActualizarBotones() {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(Boton[i]) == 0) {
      EstadoFoco[i] = !EstadoFoco[i];
      while (digitalRead(Boton[i]) == 0) {
        delay(300);
      }
      delay(300);
    }
  }
}

void PreguntarMqtt(String & topic, String & payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  int FocoMensaje = -1;
  for (int i = 0; i < 3; i++) {
    if (topic.compareTo(TopicoFocoMensaje[i]) == 0) {
      Serial.print("Topico Foco");
      Serial.println(i + 1);
      FocoMensaje = i;
    }
  }

  if (FocoMensaje > -1 ) {
    int ValorEnviado = payload.toInt();
    if (ValorEnviado) {
      EstadoFoco[FocoMensaje] = true;
    } else {
      EstadoFoco[FocoMensaje] = false;
    }
  } else {
    Serial.println("No Topico Conocido");
  }

}

void ActualizarFoco() {
  for (int i = 0; i < 3; i++) {
    if (EstadoFoco[i]) {
      digitalWrite(Foco[i], 1);
    }
    else {
      digitalWrite(Foco[i], 0);
    }
  }
}
