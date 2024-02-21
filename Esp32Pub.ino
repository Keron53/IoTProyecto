#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *******************************/

#define WIFI_SSID       "EPP_EXT"
#define WIFI_PASS       "cafe2021@"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "Keso"            
#define AIO_KEY         "aio_BztM63xmDbKFTJcNnw4MlY0DEBSs"

/************************* MQTT Topics ***************************************/

#define MQTT_PUBLISH_BTN   "Keso/feeds/boton"
#define MQTT_PUBLISH_MSG   "Keso/feeds/mensaje"
#define MQTT_PUBLISH_LVL   "Keso/feeds/nivel"
#define MQTT_PUBLISH_P1   "Keso/feeds/prueba1"
#define MQTT_PUBLISH_P2   "Keso/feeds/prueba2"

/************************* WiFi Client Setup *********************************/

WiFiClient client;

/************************* Adafruit.io MQTT Setup ******************************/

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish publisher_btn = Adafruit_MQTT_Publish(&mqtt, MQTT_PUBLISH_BTN);
Adafruit_MQTT_Publish publisher_msg = Adafruit_MQTT_Publish(&mqtt, MQTT_PUBLISH_MSG);
Adafruit_MQTT_Publish publisher_lvl = Adafruit_MQTT_Publish(&mqtt, MQTT_PUBLISH_LVL);
Adafruit_MQTT_Publish publisher_p1 = Adafruit_MQTT_Publish(&mqtt, MQTT_PUBLISH_P1);
Adafruit_MQTT_Publish publisher_p2 = Adafruit_MQTT_Publish(&mqtt, MQTT_PUBLISH_P2);

#define btn 27
#define btn2 26
#define btn3 25

void setup() {
  Serial.begin(115200);
  pinMode(btn, INPUT_PULLDOWN);
  pinMode(btn2, INPUT_PULLDOWN);
  pinMode(btn3, INPUT_PULLDOWN);
  delay(10);

  Serial.println(F("Adafruit MQTT ejemplo"));

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print(F("Conectando a WiFi"));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F("Conectado al WiFi"));

  Serial.println(F("Conectando a Adafruit IO..."));
  if (!mqtt.connected()) {
    connectToAdafruitIO();
  }
}

void loop() {
  if (digitalRead(btn) == HIGH){
    publishMessage();
  }
  if (digitalRead(btn2) == HIGH){
  publishP1();
  }
  if (digitalRead(btn3) == HIGH){
  publishP2();
  }
}

void connectToAdafruitIO() {
  int8_t retries = 3;
  while ((mqtt.connect() != 0) && (retries-- > 0)) {
    Serial.println(F("Error al conectarse a Adafruit IO, intentando de nuevo..."));
    delay(5000);
  }
}

void publishMessage() {
  if (!publisher_btn.publish("ON")) {
    Serial.println(F("Error al publicar estado"));
  } else {
    Serial.println(F("Estado publicado: ON"));
    delay(3000);
    publisher_btn.publish("OFF");
  }
}

void publishP1() {
  for (int i = 40; i <= 160; i += 10) {
    publisher_p1.publish(i);
    delay(1500); // Esperar un poco antes de la siguiente iteración
  }
    if (!publisher_msg.publish("Alerta ritmo cardiaco")) {
    Serial.println(F("Error al publicar estado"));
  } else {
    Serial.println(F("Estado publicado: Ritmo cardiaco"));
  }
    if (!publisher_lvl.publish("3")) {
    Serial.println(F("Error al publicar estado"));
  } else {
    Serial.println(F("Estado publicado: nivel 3"));
  }
    if (!publisher_btn.publish("ON")) {
    Serial.println(F("Error al publicar estado"));
  } else {
    Serial.println(F("Estado publicado: ON"));
  }
  for (int i = 160; i <= 200; i += 10) {
    publisher_p1.publish(i);
    delay(1500); // Esperar un poco antes de la siguiente iteración
  }
  delay(2000);
    for (int i = 200; i >= 160; i -= 10) {
    publisher_p1.publish(i);
    delay(1500); // Esperar un poco antes de la siguiente iteración
  }
    if (!publisher_btn.publish("OFF")) {
    Serial.println(F("Error al publicar estado"));
  } else {
    Serial.println(F("Estado publicado: OFF"));
  }
  for (int i = 160; i >= 40; i -= 10) {
  publisher_p1.publish(i);
  delay(1500); // Esperar un poco antes de la siguiente iteración
  }


}

void publishP2() {
    for (int i = 0; i <= 10; i += 2) {
    publisher_p2.publish(i);
    if (i==6){
          if (!publisher_msg.publish("Posible lluvia")) {
          Serial.println(F("Error al publicar estado"));
        } else {
          Serial.println(F("Estado publicado: Posible lluvia"));
        }
        if (!publisher_lvl.publish("2")) {
          Serial.println(F("Error al publicar estado"));
        } else {
          Serial.println(F("Estado publicado: 2"));
        }
        if (!publisher_btn.publish("ON")) {
          Serial.println(F("Error al publicar estado"));
        } else {
          Serial.println(F("Estado publicado: ON"));
        }
        delay(3500);
                if (!publisher_btn.publish("OFF")) {
          Serial.println(F("Error al publicar estado"));
        } else {
          Serial.println(F("Estado publicado: OFF"));
        }
    }
    if (i==10){
        if (!publisher_msg.publish("lluvia detectada")) {
          Serial.println(F("Error al publicar estado"));
        } else {
          Serial.println(F("Estado publicado: lluvia detectada"));
        }
        if (!publisher_lvl.publish("3")) {
          Serial.println(F("Error al publicar estado"));
        } else {
          Serial.println(F("Estado publicado: 3"));
        }
        if (!publisher_btn.publish("ON")) {
          Serial.println(F("Error al publicar estado"));
        } else {
          Serial.println(F("Estado publicado: ON"));
        }
    }
    delay(1500); // Esperar un poco antes de la siguiente iteración
  }
  delay(2000);

  for (int i = 10; i >= 0; i -= 2) {
  publisher_p2.publish(i);
  delay(1500); // Esperar un poco antes de la siguiente iteración
  }
  if (!publisher_btn.publish("OFF")) {
     Serial.println(F("Error al publicar estado"));
  } else {
   Serial.println(F("OFF"));
  }
}
