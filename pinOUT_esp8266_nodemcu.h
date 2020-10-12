/* ************************************************************************
 * Created by    : Rahul Kumar Nonia
 * File name     : pinOUT_esp8266_nodemcu.h
 * Created on    : Saturday 10 October 2020 11:41:34 AM IST
 * Last modified : Monday 12 October 2020 08:34:38 AM IST
 * Description   : 
 * ***********************************************************************/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#define PIN_WIRE_SDA (4)
#define PIN_WIRE_SCL (5)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif
#define LED_BUILTIN_AUX 16

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

#include "../generic/common.h"

#endif /* Pins_Arduino_h */
