#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;

char *ssid = "Ghar";
char *password = "nahimalum";
uint8_t pinLed = 16;

void setup() {
  pinMode(pinLed, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  
  Serial.print("Connecting ");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());

  server.on("/", [](){server.send(200, "text/plain", "HelloWorld!");});   // 200 : ok response
  server.on("/toggle", toggleLed);
  server.begin();
}

void loop() {
  server.handleClient();
}

void toggleLed(){
  digitalWrite(pinLed, !digitalRead(pinLed));
  server.send(204, ""); // 204: empty response
}
