#include <Arduino.h>
#include <WiFi.h>
#include <WifiMulti.h>
#include <HTTPClient.h>

#define RED 21
#define GREEN 0
#define BLUE 4


int blue_led_value = 0;
char led_increasing = 1;

hw_timer_t *led_fade_timer = NULL;
WiFiMulti wifiMulti;

void IRAM_ATTR onFadeTimer() {
  if(led_increasing == 1) {
    blue_led_value += 4;
  } else {
    blue_led_value -= 4;
  }
  if(blue_led_value == 252) led_increasing = 0;
  if(blue_led_value == 0) led_increasing = 1;
  analogWrite(BLUE, blue_led_value);
}

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  led_fade_timer = timerBegin(1000000);
  timerAttachInterrupt(led_fade_timer, &onFadeTimer);
  timerAlarm(led_fade_timer, 10000, true, 0);

  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();

  wifiMulti.addAP("Hoot", "nutmegroberts");
  wifiMulti.addAP("NSA", "13371952");
  
  while(wifiMulti.run() != WL_CONNECTED) {
    Serial.println("[SETUP] Waiting for WiFi connection...");
    delay(1000);
  }
  Serial.println("[SETUP] WiFi Connected!\n");

  if(wifiMulti.run() == WL_CONNECTED) {
    HTTPClient http;
    
    Serial.println("[HTTP] Begin request...");
    http.begin("http://example.com/index.html");

    Serial.println("[HTTP] GET request...");
    int http_code = http.GET();

    if(http_code > 0) {
      Serial.printf("[HTTP] Response Code: %d\n\n", http_code);
      timerStop(led_fade_timer);

      if(http_code == HTTP_CODE_OK) {
        digitalWrite(GREEN, HIGH);
      } else {
        digitalWrite(RED, HIGH);
      }
    } else {
      Serial.printf("[HTTP] GET failed with error: %s\n", http.errorToString(http_code).c_str());
      digitalWrite(RED, HIGH);
    }

    http.end();
  }
}

void loop() {
  analogWrite(BLUE, LOW);
}
