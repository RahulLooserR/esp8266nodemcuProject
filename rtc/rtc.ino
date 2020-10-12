
#include "globals.h"


AsyncWebServer server(80);
bool flag = false;


void setup()
{
  Serial.begin(115200);
	//wifiSetup();
	hotspotSetup();
 setupDisplay();
 setupClock();
  
}

void loop()
{
	MDNS.update();
  updateClock();
  updateDisplay();
  //delay(1000);
}
