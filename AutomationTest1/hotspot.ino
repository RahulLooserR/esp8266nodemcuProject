/*********************************************************************/
// HTML web page to handle input fields (ssid, password of your wifi)
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
/*********************************************************************/
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "password";

/********************* setting up hotspot ****************************/
void hotspotSetup()
{
  delay(1000);
  Serial.println();
  Serial.print("Configuring access point…");
  WiFi.softAP(getDNS(), getDNS());
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
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
/*********************************************************************************/
