#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <AceButton.h>
#include <IRremote.h>

//#define DEBUG   //If you comment this line, the PRINT & PRINTLN lines are defined as blank.
#ifdef DEBUG    //Macros are usually in all capital letters.
   #define PRINT(...)    Serial.print(__VA_ARGS__)     //PRINT is a macro, debug print
   #define PRINTLN(...)  Serial.println(__VA_ARGS__)   //PRINTLN is a macro, debug print with new line
#else
   #define PRINT(...)     //now defines a blank line
   #define PRINTLN(...)   //now defines a blank line
#endif

#define AUTH "qZA0lHbizqxH6e_2TWeBYygtLLsptjum"                 // Enter Auth Token.  
#define WIFI_SSID "WIFI-L00seR_Home"             //Enter Wifi Name
#define WIFI_PASS "home@421"         //Enter wifi Password

#define RP_1         D5            // relay pin 1  (GPIO5, D1 on board)
#define SP_1         D2             // switch pin 1 (GPIO4, D2 on board)
#define WIFI_LED     D0              // wifi led indicator (GPIO16, D0 on board)
#define IR_RECV_PIN  D3
#define REMOTE_BTN_1 0x11D9D827      // TV remote button 1

#define VP_1    V1                // virtual pin button 1

using namespace ace_button;

BlynkTimer timer1;

int toggleState_1 = 0; //Define integer to remember the toggle state for relay 1
//int wifiFlag = 0;

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

ButtonConfig config1;
AceButton button1(&config1);
void handleEvent1(AceButton*, uint8_t, uint8_t);

void ir_remote(){
  if (irrecv.decode(&results)) {

      Serial.println(results.value, HEX);
      switch(results.value){
          case REMOTE_BTN_1:  relayOnOff(1); Blynk.virtualWrite(VP_1, toggleState_1); break;
//          //case 0x80BF3BC4:  all_SwitchOff(); break;
          default : break;         
        }   
        irrecv.resume();   
  } 
}

void relayOnOff(int relay){

	switch(relay){
		case 1: 
			if(toggleState_1 == 1){
				digitalWrite(RP_1, LOW); // turn on relay 1
				toggleState_1 = 0;
				PRINTLN("Device1 ON");
			}
			else{
				digitalWrite(RP_1, HIGH); // turn off relay 1
				toggleState_1 = 1;
				PRINTLN("Device1 OFF");
			}
			delay(100);
			break;
		default : break;      
	}  
}

// When App button is pushed - switch the state
BLYNK_WRITE(VP_1) {
	toggleState_1 = param.asInt();
	digitalWrite(RP_1, toggleState_1);
}

BLYNK_CONNECTED() {
	// Request the latest state from the server
	Blynk.syncVirtual(VP_1);
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

	bool isconnected = Blynk.connected();
	if (isconnected == false) {
		//wifiFlag = 1;
		digitalWrite(WIFI_LED, LOW); //Turn off WiFi LED
	}
	if (isconnected == true) {
		//wifiFlag = 0;    
		digitalWrite(WIFI_LED, HIGH); //Turn on WiFi LED
	}
}

void button1Handler(AceButton*, uint8_t, uint8_t);

void setup()
{
	Serial.begin(9600);
	Serial.println("\n Device restarted"); 
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	while (WiFi.status() != WL_CONNECTED) 
	{
		delay(500);
		PRINT(". ");
	}

	//Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS);
	PRINT("\nWifi connected: ");
	PRINTLN(WiFi.localIP());

  irrecv.enableIRIn();
	pinMode(RP_1, OUTPUT);
	pinMode(WIFI_LED, OUTPUT);
	pinMode(SP_1, INPUT_PULLUP);



	//During Starting all Relays should TURN OFF
	digitalWrite(RP_1, toggleState_1);
	config1.setEventHandler(button1Handler);
	button1.init(SP_1);

	timer1.setInterval(3000, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
	Blynk.config(AUTH);
	delay(1000);

	Blynk.virtualWrite(VP_1, toggleState_1);
  Serial.println("Value of V1 and V2 " + String(V1) + " " + String(V2));
	PRINTLN("ok: setup successful");
}


void loop()
{
	button1.check();
  ir_remote();

	if (WiFi.status() == WL_CONNECTED)
	{
		Blynk.run();
		//Serial.println("blynk.run");
	}
	timer1.run(); // Initiates SimpleTimer
	//  Serial.print("*");
	//  delay(1000);
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
	Serial.println("EVENT1");
	switch (eventType) {
		case AceButton::kEventPressed:
			Serial.println("kEventPressed");
			toggleState_1 = 0;
			Blynk.virtualWrite(VP_1, toggleState_1);   // Update Button Widget
			digitalWrite(RP_1, LOW);
			break;
		case AceButton::kEventReleased:
			Serial.println("kEventReleased");
			toggleState_1 = 1;
			Blynk.virtualWrite(VP_1, toggleState_1);   // Update Button Widget
			digitalWrite(RP_1, HIGH);
			break;
	}
}
