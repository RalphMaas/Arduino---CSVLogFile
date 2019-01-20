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
    CSVLogFile(int cs, int outputWrite, int outputError);
    void begin(int baudrate=9600, bool useLogging=false);
    void writeLog(String data,String filename);
    void setOnWritingCallback(void *onWritingCallback());
  private:
     int _cs;
     int _outputWrite;
     int _outputError;
     String _filename;
     bool _useLogging;
     
     void error();
     void writing();
     void SerialLog(String msg);
     void (*_onWritingCallback)();
};
#endif
