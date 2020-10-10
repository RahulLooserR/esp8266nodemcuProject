/********** Web page hosted when in wifi mode ************************/
// HTML web page to handle 3 input fields (input1, input2, input3)
const char wifi_page[] = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Home Appliance Status</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    inputLed1: <input type="text" name="led1">
    <input type="submit" value="LED1">
  </form><br>
  <form action="/get">
    inputLed2: <input type="text" name="led2">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    inputLed3: <input type="text" name="led3">
    <input type="submit" value="Submit">
  </form><br>

  
  <form action="/get">
    LED1: <input type="button" name="led1" onclick = "toggleCheck(this)" >
  </form><br>
  <form action="/get">
    LED2: <input type="checkbox" name="led2" onclick = "toggleCheck(ledpin2)">
  </form><br>
    <form action="/get">
    LED3: <input type="checkbox" name="led3" onclick = "toggleCheck(ledpin3)">
  </form><br>

  <script>
  function toggleCheck(e){
    
  }
  </script>
     
</body></html>)rawliteral";

/**********************************************************************/

const char *PARAM_LED1 = "led1";
const char *PARAM_LED2 = "led2";
const char *PARAM_LED3 = "led3";

/******************* wifi mode initial setup **************************/
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
  Serial.println(WiFi.localIP());

  String dns = getDNS();
  if(MDNS.begin(dns)){
    Serial.println("DNS has started, Please connect to " + dns + ".local");
    MDNS.addService("http", "tcp", 80);
  }
  else{
    Serial.println("DNS not responding !!");
  }
  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/html", wifi_page );
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

   // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
//    if (request->hasParam(PARAM_LED1)) {
//      inputMessage = request->getParam(PARAM_LED1)->value();
//      inputParam = PARAM_LED1;
//    }
  if (request->hasParam(PARAM_LED1)) {
      inputMessage = request->getParam(PARAM_LED1)->value();
      inputParam = PARAM_LED1;
//      request.send()
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_LED2)) {
      inputMessage = request->getParam(PARAM_LED2)->value();
      inputParam = PARAM_LED2;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_LED3)) {
      inputMessage = request->getParam(PARAM_LED3)->value();
      inputParam = PARAM_LED3;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });
  
  server.onNotFound(notFound);
  server.begin();
}
/**************************************************************************************/
