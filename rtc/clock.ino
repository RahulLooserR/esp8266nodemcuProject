void setupClock()
{
   rtcObject.Begin();     //Starts I2C
 
  //RtcDateTime currentTime = RtcDateTime(yy, mon, dd, hh, mm, ss); //define date and time object
  //rtcObject.SetDateTime(currentTime);
}

void updateClock()
{
  RtcDateTime currentTime = rtcObject.GetDateTime();    //get the time from the RTC
 
  char str[20];   //declare a string as an array of chars

  hh = currentTime.Hour();
  mm = currentTime.Minute();
  ss = currentTime.Second();
 
  sprintf(str, "%d/%d/%d %d:%d:%d",     //%d allows to print an integer to the string
          currentTime.Year(),   //get year method
          currentTime.Month(),  //get month method
          currentTime.Day(),    //get day method
          hh,   //get hour method
          mm, //get minute method
          ss  //get second method
         );
 
  //Serial.println(str); //print the string to the serial port
 temperature = rtcObject.GetTemperature();
 temp_in_c = temperature.AsFloatDegC();
}
