/*
 * CSVLogFile.h = Library to log datastring to a SD card
 * Create by Ralph Maas, 20-01-2019
 */
#ifndef CSVLogFile_h
#define CSVLogFile_h

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
 
class CSVLogFile
{
  public:
    CSVLogFile(int csPin, int writeLedPin, int errorLedPin, int logButtonPin);
    void begin(int baudrate=9600, String initalFileName, bool useDebug=false);
    void writeData(String data);
    void onWriteEvent(void *doWriteEvent());
    void start()
  private:
     int _cs;
     int _writeLedPin;
     int _errorLedPin;
     int _logButtonPin;
     int _buttonState; 
     String _filename;
     bool _debug;
     
     void error();
     void ready();
     void stop();
     void debug(String msg);
     void (*_doWriteEvent)();
};
#endif
