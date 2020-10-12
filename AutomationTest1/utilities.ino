/************ function to write string on eeprom on given address *******/
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
/**************************************************************************/

/************* function to read from eeprom *******************************/
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
/****************************************************************************/

/****** function to get MAC adress (used as hotspot name and password also***/
String getDNS()
{
 String mac = WiFi.macAddress();
 int len = mac.length();
 mac.toLowerCase();
 for(int i = 0; i < len; i++){
  if (mac[i] == ':'){
    for(int j = i; j < len; j++){
      mac[j] = mac[j+1];
    }
  }
 }
 return mac;
}
/****************************************************************************/

/****************** handling server request not found ************************/
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
/*****************************************************************************/
