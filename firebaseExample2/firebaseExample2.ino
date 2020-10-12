#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "automationtest-689f5.firebaseio.com"
#define FIREBASE_AUTH "YGCmXaNTn20pjhbl3TN6nDUNqrQQa5dOsoSBHJJQ"
#define WIFI_SSID "Ghar"
#define WIFI_PASSWORD "nahimalum"

void setup() {
  Serial.begin(115200);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.stream("/LED");
  
}

int n = 0;

void loop() {
  // get value 
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  if (Firebase.failed()) {
      Serial.println("streaming error");
      Serial.println(Firebase.error());
    }
    
    if (Firebase.available()) {
       FirebaseObject event = Firebase.readEvent();
       String eventType = event.getString("type");
       eventType.toLowerCase();
       
       Serial.print("event: ");
       Serial.println(eventType);
    
  if (eventType == "put"){
    Serial.print("data");
    Serial.println(event.getString("data"));
        Serial.print("path");

    Serial.println(event.getString("path"));
    
  }
}}
