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

// Инициируем датчик

//DHT dht(DHTPIN, DHT22); // если тип датчика DHT22

DHT dht(DHTPIN, DHT11); // если тип датчика DHT22

WiFiClient espClient;
PubSubClient MqttClient(espClient);

void setup() { // различные установки
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  MqttClient.setServer(MQTT_S, MQTT_P);

Serial.begin(9600); // инициализация COM

dht.begin(); // инициализация опроса датчика
}

void loop() {

// Задержка 1 секунды между измерениями

delay(1000);

// чтение занимает 250 мСек, у старых датчиков данные обновляются не чаще чем 1 раз в 2 сек

// Считываем температуру
float t = dht.readTemperature();
float h = dht.readHumidity();

// Проверка удачно прошло ли считыванnие
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

Serial.print(t); Serial.println(" C");
Serial.print(h); Serial.println(" %");

if(WiFi.status() == WL_CONNECTED)
  {
  Serial.println(WiFi.localIP());
  MqttClient.connect("ESPClientTwo", MQTT_U, MQTT_PASS); 
  char x[4] = {0,0,0,0};
  
  /* перевод значения float в массив символов
  t- что переводим
  4 - длина получаемого символьного значения
  2 - количество символов после запятой
  x - куда записываем преобразованные данные*/
  dtostrf(t,4,2,x);  
  MqttClient.publish("sensor/temp",x); // отправка mqtt qos level 1
  dtostrf(h,4,2,x);  
  MqttClient.publish("sensor/hump",x); // отправка mqtt qos level 1
  
  }

}
