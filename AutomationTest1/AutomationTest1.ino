#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>

/************ GLOBAL variables *******************************/
const int ModeAddr = 0;  // first addres of eeprom
const int ssidAddr = 1;
const int passAddr = 101;
const int hotspotPin = 15;   // pin D8

String ssid;       // wifi ssid
String password;   // wifi password

//ESP8266WebServer server(80);
AsyncWebServer server(80);
/*************************************************************/

/************* interrupt handler ***********************/
// interrupt handler for setting mode to hotspot mode
void ICACHE_RAM_ATTR intrHandler()
{
  //    Serial.println("INTERRUPT CALLED");
  EEPROM.write(ModeAddr, 0);
  EEPROM.commit();
  ESP.restart();
}
/********************************************************/

/**** setup function to handle wifimode and hotspot mode setup ********/
void setup()
{
  pinMode(hotspotPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(hotspotPin), intrHandler, RISING);
  EEPROM.begin(512);
  Serial.begin(115200);
  int result = EEPROM.read(ModeAddr);
  if(result == 1){
    Serial.println("");
    Serial.println("Entered in wifi mode");
    delay(500);
    wifiSetup();
  }
  else if(result == 0){
    Serial.println("Entered in hotspot mode");
    hotspotSetup();
  }
}
/************************************************************************/

/********************************** void loop *******************************/
void loop()
{
  MDNS.update();
}
/****************************************************************************/
