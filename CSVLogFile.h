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
#include <SD.h>
 
class CSVLogFile
{
  public:
    CSVLogFile(int csPin, int writeLedPin, int errorLedPin, int logButtonPin);
    void begin(String csvHeader, int baudrate=9600, bool useDebug=false);
    void writeData(String data);
    void onWriteEvent(void *doWriteEvent());
    void onStopEvent(void *doStopEvent());
    void start();
    int fileCount();
  private:
     int _cs;
     int _writeLedPin;
     int _errorLedPin;
     int _logButtonPin;
     int _buttonState; 
     bool _needNewFileName;
     char _filename[16];
     int _fileCount;
     String _csvHeader;
     bool _newFile;
     bool _debug;
     
     void error();
     void ready();
     void stop();
     void debug(String msg);
     void setFileName();
     void doWrite();
     
     void (*_doWriteEvent)();
     void (*_doStopEvent)();
};
#endif
