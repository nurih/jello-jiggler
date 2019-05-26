#include <ESP8266WiFi.h>
#include <Servo.h>
#include "arduinosecret.h"
#include <WiFiClientSecure.h>

#define LED_ON LOW
#define LED_OFF HIGH

const int SERVO_PIN = 15;
// 0.1 Seconds per 60 degrees
// 48 degrees angle
const int periodMillis = int(100.0 * (48.0 / 40.0));
unsigned long timeMark = 0;
bool isOn = false;
Servo servo;

WiFiClientSecure client;


void setup()
{
  Serial.begin(115200);

  Serial.println();

  servo.attach(SERVO_PIN);
  servo.write(0);

  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LED_OFF);

  WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASSWORD);

  Serial.print("Connecting Wifi: ");

  while (WiFi.status() != WL_CONNECTED)
  {
    servo.write(8);
    delay(200);
    Serial.print(".");
    servo.write(0);
  }

  Serial.println();

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("GATEWAY: ");
  Serial.println(WiFi.gatewayIP());
  
  Serial.print("Expected SSL fingerprint SHA1");
  Serial.println(FUNCTION_HOST_SHA1_FINGERPRINT);
  client.setFingerprint(FUNCTION_HOST_SHA1_FINGERPRINT);
}


void loop()
{

  Serial.printf("[?", FUNCTION_HOST);

  if (client.connect(FUNCTION_HOST, 443))
  {
    Serial.println("  :-)]");

    Serial.printf(">>> GET %s\n", FUNCTION_PATH_AND_QUERY);

    client.print(String("GET ") + FUNCTION_PATH_AND_QUERY + " HTTP/1.1\r\n" +
                 "Host: " + FUNCTION_HOST + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n");

    Serial.println("--------");

    while (client.connected())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        Serial.println(String('<<< ') + line);

        if (line.startsWith("jiggle="))
        {
          isOn = true;
        }
      }
    }
    client.stop();
    Serial.println("<.");

    if (isOn == true)
    {
      Serial.println("*** Jiggle! ***");
      jiggle(6);
    }
  }
  else
  {
    Serial.print(client.status());
    Serial.println(" :-(]");
    client.stop();
  }

  delay(6000);
  isOn = false;
}

void jiggle(int count)
{

  for (int i = 0; i < count; i++)
  {

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
