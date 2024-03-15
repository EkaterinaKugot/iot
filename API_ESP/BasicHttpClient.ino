#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;
DynamicJsonDocument doc(1024);

const char* SSID = "kate24"; 
const char* PASSWORD = "1357924680";
String LOCATION = "Tokyo";


void setup() {

  Serial.begin(9600);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID, PASSWORD);
}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;

    HTTPClient http;

    if (Serial.available() > 0){
      String inputNum = Serial.readStringUntil('\n');
      double value = inputNum.toDouble();

      if(http.begin(client, "http://192.168.19.243:8005/send_temperature")){
        http.addHeader("Content-Type", "application/json");

        StaticJsonDocument<200> TempDataJSON;
        TempDataJSON["value"] = value;
        TempDataJSON["location"] = LOCATION;
        TempDataJSON["device_id"] = 0;    
        TempDataJSON["timestamp"] = "2024-03-14 21:18:21.0";

        String TempDataString;
        serializeJson(TempDataJSON, TempDataString);
        Serial.println(TempDataString);
        int httpResponseCode = http.POST(TempDataString);
        if (httpResponseCode == 200) {
          Serial.println("[HTTP] Data sent successfully");
        } else {
          Serial.println("[HTTP] There was a problem sending data");
        }

        http.end();

      }else {
        Serial.println("[HTTP] Unable to connect to send data");
      }
      delay(1000);
    }
    
    if (http.begin(client, "http://192.168.19.243:8005/get_temperature/?location=" + LOCATION)) {  // HTTP
      int httpCode = http.GET();

      if (httpCode > 0) {
        Serial.printf("[HTTP] Receiving data, code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          deserializeJson(doc, payload);
          if (doc.size() == 1){
            Serial.println("No data, please enter a number\n");
          }else{
            deserializeJson(doc, payload);

            double average = calculateAverage(doc["Values"].as<JsonArray>());
            
            Serial.print("Array: [ ");
            for (JsonVariant value : doc["Values"].as<JsonArray>()){
              Serial.print(String(value.as<double>()) + " ");
            }
            Serial.print("]");

            Serial.println("  Average: " + String(average) + "\n");
          }
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.println("[HTTP] Unable to connect to receive data");
    }
    delay(5000);
  }
  delay(1000);

}

double calculateAverage(JsonArray arr) {
  double sum = 0.0;
  int count = 0;

  for (JsonVariant value : arr) {
    if (value.is<double>()) {
      sum += value.as<double>();
      count++;
    }else {
      Serial.println("[HTTP] JSON parsing error: Non-double value found in Values array");
    }
  }
  
  if (count > 0) {
    return sum / count;
  } else {
    Serial.println("[HTTP] JSON parsing error: No valid values found in Values array");
    return 0.0;
  }
}