/*********************************************************************/
// HTML web page to handle input fields (ssid, password of your wifi)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    hours: <input type="number" name="hh">
    minutes: <input type="number" name="mm">
    <input type="submit" value="Submit">
  </form><br>
 
</body></html>)rawliteral";
/*********************************************************************/
const char* PARAM_INPUT_1 = "hh";
const char* PARAM_INPUT_2 = "mm";


/********************* setting up hotspot ****************************/
void hotspotSetup()
{
  delay(1000);
  IPAddress ap_local_IP(192,168,80,1);
  IPAddress ap_gateway(192,168,80,1);
  IPAddress ap_subnet(255,255,255,0);
  
  Serial.println();
  Serial.print("Configuring access point…");

  WiFi.mode(WIFI_AP);
//  WiFi.softAPConfig(ap_local_IP, ap_gateway, ap_subnet);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println("HTTP server started"); 

   String dns = "mylocaldns";
  if(MDNS.begin(dns)){
    Serial.println("DNS has started, Please connect to " + dns + ".local");
    MDNS.addService("http", "tcp", 80);
  }
  else{
    Serial.println("DNS not responding !!");
  }


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });


  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    //String inputMessage;
    String statusCode;

    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2) ) {
      hours = request->getParam(PARAM_INPUT_1)->value();
      minutes = request->getParam(PARAM_INPUT_2)->value();
      Serial.println("hours: " + hours);
      Serial.println("minutes: " + minutes);
      flag = true;
      
    }
    else {
      statusCode = "No message sent";
    }
    Serial.println(statusCode);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + hours + ") with value: " + hours +
                                     "<br><a href=\"/\">Return to Home Page</a> 2: " + minutes );
  });
  server.onNotFound(notFound);
  server.begin();
}
/*********************************************************************************/
