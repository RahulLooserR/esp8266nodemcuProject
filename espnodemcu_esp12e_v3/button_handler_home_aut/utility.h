#ifndef UTILITY_H
#define UTILITY_H

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <AceButton.h>
#include <IRremote.h>

//#define BLYNK_PRINT Serial
#define DEBUG                                    //If you comment this line, the PRINT & PRINTLN lines are defined as blank.
#ifdef DEBUG                                     //Macros are usually in all capital letters.
#define PRINT(...) Serial.print(__VA_ARGS__)     //PRINT is a macro, debug print
#define PRINTLN(...) Serial.println(__VA_ARGS__) //PRINTLN is a macro, debug print with new line
#else
#define PRINT(...)   //now defines a blank line
#define PRINTLN(...) //now defines a blank line
#endif

#define AUTH "qZA0lHbizqxH6e_2TWeBYygtLLsptjum" // Enter Auth Token.
#define WIFI_SSID "WIFI-L00seR_Home"            //Enter Wifi Name
#define WIFI_PASS "home@421"                    //Enter wifi Password

#define WIFI_LED D4 // wifi led indicator (GPIO16, D0 on board)
#define IR_RECV_PIN D5

#define N    4  // Number of devies
using namespace ace_button;

BlynkTimer timer1;

/*                        */
const int relayPins[N] = {D0, D1, D3, D8};
/*                                Rx, tX */
const int switchPins[N] = {D6, D7, 3, 1};

/* remote button 0(0x11D930CF) 1 2 3 4 */
const int remoteBtns[N] = {0x11D9D827, 0x11D9F00F, 0x11D9C03F, 0x11D99867};
const int virtualPins[N+1] = {V0, V1, V2, V3, V4};
int toggleState[N] = {0}; //Define integer to remember the toggle state for relay 1

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

ButtonConfig *configs[N];
//ButtonConfig configs[N] = {ButtonConfig(), ButtonConfig(), ButtonConfig(), ButtonConfig()};
//AceButton button1(&config1);
AceButton *buttons[N];




void btnHandler(AceButton *, uint8_t, uint8_t);
void checkBlynkStatus();
void relayOnOff(int);
void ir_remote();

/* setup functions */
/********************/
void serialSetup();
void wifiSetup();
void pinSetup();
void turnOffRelays();
void btnHandlerSetup();
void blynkSetup();
/********************/

#endif