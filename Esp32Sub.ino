#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "EPP_EXT"
#define WLAN_PASS       "cafe2021@"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Keso"            
#define AIO_KEY         "aio_BztM63xmDbKFTJcNnw4MlY0DEBSs"

/************ Global State (you don't need to change this!) ******************/

WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/boton");
Adafruit_MQTT_Subscribe nivel = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/nivel");
Adafruit_MQTT_Subscribe mensaje = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/mensaje");

/*************************** Sketch Code ************************************/
void MQTT_connect();

String Mensaje = "Aviso";
int NumNivel = 1;
int pinMotor = 16;
int LEDVERDE = 17;
int LEDAMARILLO = 18;
int LEDROJO = 19;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(pinMotor, OUTPUT);
  pinMode(LEDVERDE, OUTPUT);
  pinMode(LEDAMARILLO, OUTPUT);
  pinMode(LEDROJO, OUTPUT);
  Wire.begin();
  lcd.begin();
  lcd.backlight();

  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff & slider feed.
  mqtt.subscribe(&onoffbutton);
  mqtt.subscribe(&nivel);
  mqtt.subscribe(&mensaje);
}

uint32_t x=0;

void loop() {

  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("On-Off button: "));
      Serial.println((char *)onoffbutton.lastread);
      
      if (strcmp((char *)onoffbutton.lastread, "ON") == 0) {
        lcd.print(Mensaje);
        if (NumNivel == 1) {
          digitalWrite(LEDVERDE, HIGH);
        }
        else if (NumNivel == 2){
          digitalWrite(LEDAMARILLO, HIGH);
        }
        else if (NumNivel == 3){
          digitalWrite(LEDROJO, HIGH);
        }
        vibrarMotor(NumNivel);
      }
      if (strcmp((char *)onoffbutton.lastread, "OFF") == 0) {
        digitalWrite(LEDVERDE, LOW); 
        digitalWrite(LEDAMARILLO, LOW); 
        digitalWrite(LEDROJO, LOW); 
        lcd.clear();
      }
    }
    
    // check if its the slider feed
    if (subscription == &nivel) {
      Serial.print(F("Nivel del mensaje: "));
      Serial.println((char *)nivel.lastread);
      uint16_t sliderval = atoi((char *)nivel.lastread);
      NumNivel= sliderval;
    }

    // check if its the mensaje feed
    if (subscription == &mensaje) {
      Serial.print(F("Mensaje: "));
      Serial.println((char *)mensaje.lastread);
      Mensaje = String((char *)mensaje.lastread);
    }
  }

  // ping the server to keep the mqtt connection alive
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

void vibrarMotor(int nivel){
        if(nivel == 1){
        digitalWrite(pinMotor, HIGH);
        delay(200);
        digitalWrite(pinMotor, LOW);
        }
        else if(nivel == 2){
          for (int i = 0; i < 2; i++) {
            digitalWrite(pinMotor, HIGH); // Encender el motor
              delay(200);
              digitalWrite(pinMotor, LOW); // Apagar el motor
              delay(200);
          }
  }
      else if(nivel == 3){
          for (int i = 0; i < 3; i++) {
            digitalWrite(pinMotor, HIGH); // Encender el motor
              delay(200);
              digitalWrite(pinMotor, LOW); // Apagar el motor
              delay(200);
          }
     }
}
