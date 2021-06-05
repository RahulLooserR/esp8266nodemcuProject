#include "utility.h"

void ir_remote()
{
	if (irrecv.decode(&results))
	{
		//PRINTLN(results.value, HEX);
		for (int i = 0; i < N; i++)
		{
			if (remoteBtns[i] == results.value)
			{
				relayOnOff(i);
				Blynk.virtualWrite(virtualPins[i], toggleState[i]);
			}
		}
		irrecv.resume();
	}
}

void relayOnOff(int relay_no)
{
	if (toggleState[relay_no] == 1)
	{
		digitalWrite(relayPins[relay_no], LOW);
		toggleState[relay_no] = 0;
	}
	else
	{
		digitalWrite(relayPins[relay_no], HIGH);
		toggleState[relay_no] = 1;
	}
}

BLYNK_WRITE_DEFAULT()
{
	int pin = request.pin;
	toggleState[pin] = param.asInt();
	digitalWrite(relayPins[pin], toggleState[pin]);
}

BLYNK_CONNECTED()
{
	// Request the latest state from the server
	for (int i = 0; i < N; i++)
	{
		Blynk.syncVirtual(virtualPins[i]);
	}
}

void checkBlynkStatus()
{ // called every 3 seconds by SimpleTimer

	bool isconnected = Blynk.connected();
	if (isconnected == false)
	{
		digitalWrite(WIFI_LED, HIGH); //Turn off WiFi LED
	}
	if (isconnected == true)
	{
		digitalWrite(WIFI_LED, LOW); //Turn on WiFi LED
	}
}

void btnHandler(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
	uint8_t id = button->getId();

		switch (eventType)
		{
		case AceButton::kEventPressed:
			toggleState[id] = 0;
			Blynk.virtualWrite(virtualPins[id], toggleState[id]); // Update Button Widget
			digitalWrite(relayPins[id], LOW);
			break;

		case AceButton::kEventReleased:
			toggleState[id] = 1;
			Blynk.virtualWrite(virtualPins[id], toggleState[id]); // Update Button Widget
			digitalWrite(relayPins[id], HIGH);
			break;
		}
}

void serialSetup()
{
	Serial.begin(9600);
	//PRINTLN("\n Device restarted");
}

void wifiSetup()
{
	int time = 40;
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		PRINT(". ");
		if(time == 40) break;
	}
}

void pinSetup()
{
	irrecv.enableIRIn();
	pinMode(WIFI_LED, OUTPUT);

	for (int i = 0; i < N; i++)
	{
		pinMode(relayPins[i], OUTPUT);
		pinMode(switchPins[i], INPUT_PULLUP);
	}
}

void turnOffRelays()
{
	for (int i = 0; i < N; i++)
	{
		digitalWrite(relayPins[i], toggleState[i]);
	}
}

void btnHandlerSetup()
{
	for (int i = 0; i < N; i++)
	{
		/* AceButton(uint8_t pin = 0, uint8_t defaultReleasedState = HIGH, uint8_t id = 0);*/
		// configs[i] = new ButtonConfig;
		// configs[i]->setEventHandler(btnHandler);
		buttons[i] = new AceButton;
		buttons[i]->init(switchPins[i], HIGH, i);
		buttons[i]->setEventHandler(btnHandler);
	
	}
}

void blynkSetup()
{
	timer1.setInterval(3000, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
	Blynk.config(AUTH);
	delay(1000);

	for (int i = 0; i < N; i++)
	{
		Blynk.virtualWrite(virtualPins[i], toggleState[i]);
	}
}

// void readSwitchPins()
// {
// 	for(int i = 0; i < N; i++){
// 		if()
// 	}
// }
