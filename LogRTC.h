/*
 * LogRTC.h = Library to log datastring to a SD card
 * Create by Ralph Maas, 23-02-2019
 */
#ifndef LogRTC_h
#define LogRTC_h

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Wire.h"


class LogRTC
{
  public:
    LogRTC(uint8_t i2cAddress);
    void set_date(byte newYear, byte newMonth, byte newDay, byte newDayofweek);
    void set_time(byte newHour, byte newMinute, byte newSecond);
    String get_date();
    String get_time();
    String get_datetime();
    String get_datofweek();
  private:
    uint8_t address;  

    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t dayofweek;
    uint16_t hour;
    uint16_t minute;
    uint16_t second;
    byte decToBcd(byte val);
    byte bcdToDec(byte val) ;
    void read_datetime();
    void write_datetime();
};
#endif

