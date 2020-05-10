#include "DHT.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "mydef.h"

/*#define WIFI_SSID "name"
#define WIFI_PASS "pswd" 

#define MQTT_S "1.1.1.1"
#define MQTT_P 1883
#define MQTT_U "user"
#define MQTT_PASS "pass"*/

#define DHTPIN 2 // номер пина, к которому подсоединен датчик

// Раскомментируйте в соответствии с используемым датчиком

// Инициируем датчик

//DHT dht(DHTPIN, DHT22);

DHT dht(DHTPIN, DHT11);
WiFiClient espClient;
PubSubClient MqttClient(espClient);

void setup() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  MqttClient.setServer(MQTT_S, MQTT_P);

Serial.begin(9600);

dht.begin();

}

void loop() {

// Задержка 2 секунды между измерениями

delay(1000);

// чтение занимает 250 мСек, у старых датчиков данные обновляются не чаще чем 1 раз в 2 сек

// Считываем температуру
float t = dht.readTemperature();
float h = dht.readHumidity();

float* pt = &t;
float* ph = &h;
// Проверка удачно прошло ли считыванnие.

if (isnan(t) || isnan(h)) {
Serial.println("Не удается считать показания");
return;
}

if (Serial.available() > 0) {  //если есть доступные данные
        // считываем байт
        char incomingByte = Serial.read();
 
        // отсылаем то, что получили
        Serial.print("I received: ");
        Serial.println(incomingByte, DEC);
    }


Serial.print(*pt); Serial.println(" C");
Serial.print(*ph); Serial.println(" %");

if(WiFi.status() == WL_CONNECTED)
  {
  Serial.println(WiFi.localIP());
  MqttClient.connect("ESPClientTwo", MQTT_U, MQTT_PASS); 
  char x[4] = {0,0,0,0};
  dtostrf(t,4,2,x);  
  MqttClient.publish("sensor",x);
  }

}
