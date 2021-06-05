#include "utility.h"

void setup()
{
	//serialSetup();
	wifiSetup();
	pinSetup();
	turnOffRelays();
	btnHandlerSetup();
	blynkSetup();
	//PRINTLN("ok: setup successful");
}


void loop()
{
	for(int i = 0; i < N; i++){
		buttons[i]->check();
	}
	ir_remote();

	if (WiFi.status() == WL_CONNECTED)
	{
		Blynk.run();
	}
	timer1.run(); // Initiates SimpleTimer
}
