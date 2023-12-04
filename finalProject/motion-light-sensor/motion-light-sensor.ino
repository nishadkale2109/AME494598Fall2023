#define LILYGO_WATCH_2019_WITH_TOUCH
#include <LilyGoWatch.h>
TTGOClass *ttgo;

#include <Adafruit_NeoPixel.h>

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Rahil";
const char* password = "93971625";
const char* serverName = "http://34.204.77.33:8080/sendData"; 

const int pirPin = 26;
const int neoPixelPin = 25;
const int numPixels = 4;


String response;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  http.begin(serverName);
  
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  return payload;
}

void setup() {
  Serial.begin(115200);

  strip.begin();
  pinMode(pirPin, INPUT);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  Serial.println("Sample Motion Sensing");
  int motion = digitalRead(pirPin);

  if (motion == HIGH) {

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          strip.setPixelColor(0, strip.Color(i * 255, j * 255, k * 255));
          strip.setPixelColor(1, strip.Color(i * 255, j * 255, k * 255));
          strip.setPixelColor(2, strip.Color(i * 255, j * 255, k * 255));
          strip.setPixelColor(3, strip.Color(i * 255, j * 255, k * 255));

          strip.show();
          delay(500);
        }
      }
    }

    unsigned long currentTime = millis();

    String url = String(serverName) + "?motion_time=" + String(currentTime);
    Serial.println(url);

    String response = httpGETRequest(url.c_str());
    Serial.println(response);
  } else {
    strip.clear();
    strip.show();
  }

  delay(1000); 
}
