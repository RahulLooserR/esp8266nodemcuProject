/* ************************************************************************
 * Created by    : Rahul Kumar Nonia
 * File name     : globals.h
 * Created on    : Saturday 10 October 2020 08:37:46 PM IST
 * Last modified : Saturday 10 October 2020 08:38:15 PM IST
 * Description   : 
 * ***********************************************************************/

#ifndef GOLBALS_H
#define GLOBALS_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>


/****************** i2c, RTC lib and variables ************************/
/* connect SDA -> D2 = 19
 *  SCL -> D1= 20
 */
#include <Wire.h>       //I2C library
#include <RtcDS3231.h>  //RTC library
RtcDS3231<TwoWire> rtcObject(Wire);
uint8_t hh = 0, mm = 0, ss = 0;    // time
uint8_t yy = 20, mon = 10, dd = 11;  // date

RtcDateTime currentTime;

// temperature reading
RtcTemperature temperature;
float temp_in_c;    // read in clockupdate()
/***********************************************************************/

const uint8_t digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
  };
  
static const uint8_t minusSegments = 0b01000000;

/********* clock and  data i/o for display *******/


String ssid = "Master_Esp8266_nodemcu";
String password = "esp8266_01";
String dns = "my_rtc_local";
String hours, minutes;
int hrs = 0, mins = 0;


#endif
