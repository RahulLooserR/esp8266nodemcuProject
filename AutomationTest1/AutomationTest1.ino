#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>


const int ModeAddr = 0;  // first addres of eeprom
const int ssidAddr = 1;
const int passAddr = 101;
const int hotspotPin = 15;   // pin D8

const char *hotspotSSID = "AutomationTest1";
const char *hotspotPass = "password";
String ssid;       // wifi ssid
String password;   // wifi password
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "password";

//ESP8266WebServer server(80);
AsyncWebServer server(80);

// HTML web page to handle input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    ssid: <input type="text" name="ssid">
    password: <input type="text" name="password">
    <input type="submit" value="Submit">
  </form><br>
 
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void eepromWrite (String str, int addr)
{
  int i;
  Serial.print("Writing to eeprom....");
  for(i = 0; i < str.length(); i++){
    EEPROM.write(addr+i, str[i]);
    Serial.print(str[i]);
  }
  EEPROM.write(addr + i, '\0');
  EEPROM.commit();
}
String eepromRead (int addr)
{
  Serial.println("Reading from eeprom....");
  Serial.println("");
  char temp;
  String str;
  do{
    temp = char(EEPROM.read(addr));
    str = str + temp;
    addr++;
  }while(temp != '\0');
  Serial.println("");
  return str;
}

void wifiSetup()
{
  Serial.begin(115200);
  Serial.println("");
  ssid = eepromRead(ssidAddr);
  password = eepromRead(passAddr);
  Serial.println("ssid: " + ssid);
  Serial.println("password: " + password);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting ");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());

  //server.on("/", [](){server.send(200, "text/plain", "HelloWorld!");});   // 200 : ok response
  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/html", "Hello world !" );
  });
  server.on("/setHotspotMode", HTTP_GET, [] (AsyncWebServerRequest *request) {
    EEPROM.write(ModeAddr, 0);
    EEPROM.commit();
    Serial.println("Written 0 to ModeAddr");

    request->send(200, "text/html", "Hotspot mode set to 0" );
    server.end();
    WiFi.disconnect();
    Serial.println("Restarting esp....");
    delay(5000);
    ESP.restart();
  });
  server.begin();

  
}

void hotspotSetup()
{
  delay(1000);
  Serial.println();
  Serial.print("Configuring access point…");
  WiFi.softAP(hotspotSSID, hotspotPass);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  //server.on("/", handleRoot);
  //server.begin();
  Serial.println("HTTP server started"); 

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    //String inputMessage;
    String statusCode;

    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2) ) {
      ssid = request->getParam(PARAM_INPUT_1)->value();
      password = request->getParam(PARAM_INPUT_2)->value();
       
      // writing back in EEPROM to set wifi mode
      EEPROM.write(ModeAddr, 1);
      EEPROM.commit();
      Serial.println("Written 1 to ModeAddr");
      Serial.println("Saving ssid and password");
      eepromWrite(ssid, ssidAddr);
      eepromWrite(password, passAddr);
      Serial.println("ssid: " + ssid);
      Serial.println("password: " + password);
      Serial.println("Restarting esp ...");
      server.end();
      WiFi.softAPdisconnect(true);
      delay(5000);
      ESP.restart();
      
    }
    else {
      statusCode = "No message sent";
    }
    Serial.println(statusCode);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + ssid + ") with value: " + ssid +
                                     "<br><a href=\"/\">Return to Home Page</a> 2: " + password );
  });
  server.onNotFound(notFound);
  server.begin();

}

// interrupt handler for setting mode to hotspot mode
void ICACHE_RAM_ATTR intrHandler()
{
  //    Serial.println("INTERRUPT CALLED");
  EEPROM.write(ModeAddr, 0);
  EEPROM.commit();
  ESP.restart();
}

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

void loop()
{
  //handleRoot();
  //    Serial.println("PINMODE: " + String(digitalRead(hotspotPin)));
}

/*
void handleRoot()
{
  server.send(200, "text/html", "<h1>You are connected</h1>");
}
*/
