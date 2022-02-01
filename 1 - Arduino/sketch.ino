const int LED_PIN_DHT = 5;
const int LED_PIN_MQTT = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(100);

  Serial.println("Inicializando...");

  pinMode(LED_PIN_DHT, OUTPUT);
  pinMode(LED_PIN_MQTT, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("LED vermelho...");
  delay(2000);
  digitalWrite(LED_PIN_DHT, HIGH);
  digitalWrite(LED_PIN_DHT, LOW);

  Serial.println("LED verde...");
  delay(2000);
  digitalWrite(LED_PIN_MQTT, HIGH);
  digitalWrite(LED_PIN_MQTT, LOW);

}