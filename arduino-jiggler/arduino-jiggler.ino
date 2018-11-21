#include <ESP8266WiFi.h>
#include <Servo.h>
#include "arduinosecret.h"

#define LED_ON LOW
#define LED_OFF HIGH

const int SERVO_PIN = 15;
// 0.1 Seconds per 60 degrees
// 48 degrees angle
const int periodMillis = int(100.0 * (48.0 / 40.0)) ;
unsigned long timeMark = 0;
bool isOn = false;
Servo servo;

void setup() {
  Serial.begin(115200);

  Serial.println();

  servo.attach(SERVO_PIN);
  servo.write(0);

  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LED_OFF);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting...");

  while (WiFi.status() != WL_CONNECTED)
  {
    servo.write(8);
    delay(200);
    Serial.print(".");
    servo.write(0);
  }

  Serial.println();

  Serial.print("Connected! IP address: ");

  Serial.println(WiFi.localIP());

}

void loop() {

  WiFiClient client;

  Serial.printf("\n[Connecting to %s ...\n", FUNCTION_HOST);

  if (client.connect(FUNCTION_HOST, 80))
  {
    Serial.println("connected]");

    Serial.printf(">>> GET %s\n", FUNCTION_PATH_AND_QUERY);

    client.print(String("GET ") + FUNCTION_PATH_AND_QUERY + " HTTP/1.1\r\n" +
                 "Host: " + FUNCTION_HOST + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n"
                );

    Serial.println("--------");

    while (client.connected())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        Serial.println(String('<<< ') + line);

        if (line.startsWith("jiggle=")) {
          isOn = true;
        }
      }
    }
    client.stop();
    Serial.println("*** Disconnected ***");

    if (isOn == true) {
      Serial.println("*** Jiggle! ***");
      jiggle(6);
    }
  }
  else
  {
    Serial.println("** Connection failed ***");
    client.stop();
  }

  delay(6000);
  isOn = false;
}

void jiggle(int count) {

  for (int i = 0; i < count; i++) {

    servo.write(48);

    digitalWrite(LED_BUILTIN, LED_ON);

    delay(periodMillis);

    servo.write(0);

    digitalWrite(LED_BUILTIN, LED_OFF);

    delay(periodMillis);
  }
}

//bool getBlink(int dutyCycleMillis) {
//  bool result = (millis() % (2 * dutyCycleMillis)) < dutyCycleMillis;
//  return result;
//}

