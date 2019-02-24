
#if (ARDUINO >=100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "LogRTC.h";
#include "Wire.h";

byte zero = 0x00; //workaround for issue #527
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

LogRTC :: LogRTC(uint8_t i2cAddress){
  address = i2cAddress;
}

void LogRTC :: set_date(byte newYear, byte newMonth, byte newDay, byte newDayofweek)
{
  read_datetime();
  year = newYear;
  month = newMonth;
  day= newDay;
  dayofweek = newDayofweek;
  write_datetime();
}


void LogRTC :: set_time(byte newHour, byte newMinute, byte newSecond)
{
  read_datetime();
  hour = newHour;
  minute = newMinute;
  second = newSecond;
  write_datetime();
}

String LogRTC :: get_date()
{
  read_datetime();
  char dateBuffer[10];
  sprintf(dateBuffer,"%02u-%02u-%04u",day,month,year);
  return dateBuffer;
}

String LogRTC :: get_datofweek()
{
  return daysOfTheWeek[dayofweek];
}


//private methods

String LogRTC :: get_time()
{
  read_datetime();
  char timeBuffer[8];
  sprintf(timeBuffer,"%02u:%02u:%02u",hour,minute,second);
  return timeBuffer;
}

String LogRTC :: get_datetime()
{
  read_datetime();
  char dateBuffer[20];
  sprintf(dateBuffer,"%02u-%02u-%04u %02u:%02u:%02u ",day,month,year,hour,minute,second);
  return dateBuffer;
}

byte LogRTC :: decToBcd(byte val)
{
  // Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}

byte LogRTC :: bcdToDec(byte val) 
{
  // Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

void LogRTC :: read_datetime()
{
  // Reset the register pointer
  Wire.beginTransmission(address);
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(address, 7);

  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  dayofweek = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  day = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read())+2000;;
}

void LogRTC :: write_datetime()
{
  
  Wire.beginTransmission(address);
  Wire.write(zero); //stop Oscillator

  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(dayofweek));
  Wire.write(decToBcd(day));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));

  Wire.write(zero); //start 

  Wire.endTransmission();
}

