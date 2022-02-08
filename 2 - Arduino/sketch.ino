#include <WiFi.h>
#include <DHTesp.h>
#include "PubSubClient.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.emqx.io";
const char clientId[50] = "clientId-nasser";
const char* TOPIC = "IFSCsope/nasser";

WiFiClient espClient;
DHTesp dhtSensor;
PubSubClient client(espClient);

const int DHT_PIN = 15;
const int LED_PIN_DHT = 5;
const int LED_PIN_MQTT = 2;

int seq = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(100);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(LED_PIN_DHT, OUTPUT);
  pinMode(LED_PIN_MQTT, OUTPUT);

  Serial.println();  
  Serial.print("Conectando a ");
  Serial.println(ssid);

  wifiConnect();

  Serial.println("");
  Serial.println("WiFi Conectado!");
  Serial.print("Endereco IP: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, 1883);

}

void mqttReconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão com MQTT...");
    if (client.connect(clientId)) {
      Serial.print(clientId);
      Serial.println(" conectado");
    } else {
      Serial.print("A conexão falhou, código = ");
      Serial.print(client.state());
      Serial.println(" reconectando em 5 segundos");
      delay(5000);
    }
  }
}

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    WiFi.reconnect();
  }
}

void loop() {

  if (!client.connected()) {
    mqttReconnect();
  }
  delay(2000);
  digitalWrite(LED_PIN_DHT, HIGH);
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  digitalWrite(LED_PIN_DHT, LOW);
  Serial.println("Temp: " + String(data.temperature, 2) + " C");
  Serial.println("Humi: " + String(data.humidity, 1) + " %");

  delay(2000);
  digitalWrite(LED_PIN_MQTT, HIGH);
  seq++;
  String resp = respRecord(seq, data.temperature, data.humidity);
  int len = resp.length() + 1;
  char msg[len];
  resp.toCharArray(msg, len);
  Serial.print("Enviando mensagem para MQTT = [");
  Serial.print(len);
  Serial.print("] [ ");
  Serial.print(msg);
  Serial.print(" ]");
  client.publish(TOPIC, (byte *)msg, len);
  digitalWrite(LED_PIN_MQTT, LOW);
  delay(2000);
  client.loop();
}

String respRecord(int seq, float temp, float humi) {
  String ptr = "";
  ptr += "(";
  ptr += seq;
  ptr += ",";
  ptr += "temp";
  ptr += ",";
  ptr += humi;
  ptr += ")";

  return ptr;
}
