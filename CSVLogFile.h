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
    CSVLogFile(int csPin, int writeLedPin, int errorLedPin);
    void begin(int baudrate=9600, bool debug=false);
    void writeLog(String data,String filename);
    void onWriteEvent(void *doWriteEvent());
  private:
     int _cs;
     int _outputWrite;
     int _outputError;
     String _filename;
     bool _debug;
     
     void error();
     void ready();
     void debug(String msg);
     void (*_doWriteEvent)();
};
#endif
