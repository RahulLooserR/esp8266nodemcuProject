#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
#define CLK D2
#define DIO D3

#define DELAY 1000

// creating display object
TM1637Display display(CLK, DIO);

// four segment of display
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };

void setupDisplay()
{
	display.setBrightness(0x0f);
	display.setSegments(data);
	delay(DELAY);
	display.clear();
	delay(DELAY);
	display.setSegments(data);
}

void updateDisplay()
{
	//display.clear();
	if(flag){
    castHoursMin();
    currentTime = RtcDateTime(yy, mon, dd, hrs, mins, ss); //define date and time object
  rtcObject.SetDateTime(currentTime);
		setDigit();
   flag = false;
	}
 else{
    setDigit();
 }
 
	if(ss % 2 == 0){
  data[1] = digitToSegment[getSecondDigit(hh)] | 0x80;
	}
 else{
  data[1] = digitToSegment[getSecondDigit(hh)];
 }

/* this segment display tempure in degree celsius
 *  eg: 32.54 will be displayed as  32:5C in 4 segment
 */
  if(ss >= 25 && ss <= 35){
    int int_temp = temp_in_c / 1; 
    int fractional_temp = (int)(temp_in_c * 10) % 10;
    data[0] = digitToSegment[getFirstDigit(int_temp)];
    data[1] = digitToSegment[getSecondDigit(int_temp)] | 0x80;
    data[2] = digitToSegment[getSecondDigit(fractional_temp)]; 
    data[3] = digitToSegment[12]; // display c
  }
  display.setSegments(data);
}
