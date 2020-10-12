
void castHoursMin()
{
	hrs = hours.toInt();
	if(hrs > 23)
		hrs = 0;
	
	mins = minutes.toInt();
	if(mins > 59)
		mins = 0;
}

uint8_t getFirstDigit(int num)
{
	return (num/10);
}

uint8_t getSecondDigit(int num)
{
	return (num % 10);
}

void notFound(AsyncWebServerRequest *request) 
{
  request->send(404, "text/plain", "Not found");
}

void setDigit()
{
    data[0] = digitToSegment[getFirstDigit(hh)];
    data[1] = digitToSegment[getSecondDigit(hh)];
    data[2] = digitToSegment[getFirstDigit(mm)];
    data[3] = digitToSegment[getSecondDigit(mm)];
}
