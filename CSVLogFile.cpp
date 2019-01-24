
#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <SPI.h>
#include <SD.h>
#include "CSVLogFile.h"

CSVLogFile::CSVLogFile(int csPin, int writeLedPin, int errorLedPin, int logButtonPin)
{
   _cs = csPin;
   _writeLedPin = writeLedPin;
   _errorLedPin = errorLedPin;
   _logButtonPin = logButtonPin; 
   _debug = false;
   _buttonState = 0;
   _needNewFileName = true;
   _fileCount = 0;
   _newFile = true;
}

void CSVLogFile::begin(String csvHeader, int baudrate=9600, bool useDebug=false)
{
   _debug = useDebug;
   _csvHeader = csvHeader;
   
   pinMode(_writeLedPin, OUTPUT);
   pinMode(_errorLedPin, OUTPUT);
   pinMode(_logButtonPin, INPUT);

   Serial.begin(baudrate);
   debug("initializing SD card...");

   if (!SD.begin(_cs)) {
       debug("card failed, or not present");
       error();
   } else {
       ready();
       debug("card initialized.");
   };
}

void CSVLogFile::writeData(String data)
{
    _buttonState = digitalRead(_logButtonPin); 
    if(_buttonState == HIGH ){
        setFileName();
        File dataFile = SD.open(_filename, FILE_WRITE);
        if (dataFile) {
          debug(data);
          doWrite();
          if (_newFile){
             dataFile.println(_csvHeader);
             _newFile = false;
          };
          
          dataFile.println(data);
          dataFile.close();
        }
        else {
          error();
      }
    }
    else{
      stop();
    }
}

void CSVLogFile::onWriteEvent(void *doWriteEvent())
{
  _doWriteEvent = doWriteEvent;
  debug("call callback");
}

//private methods
void CSVLogFile::doWrite()
{
    ready();
    debug("write to "+String(_filename));
    if (_doWriteEvent)
    {
      _doWriteEvent();
      debug("doWriteEvent is attached");
    }
    else
    {
      debug("doWriteEvent is not attached");
    }
}

void CSVLogFile::stop()
{
    debug("writing stop");
    digitalWrite(_writeLedPin, LOW);
    _needNewFileName = true;
}

void CSVLogFile::error()
{
    debug("error occured");
    digitalWrite(_errorLedPin, HIGH);
    digitalWrite(_writeLedPin, LOW);
}

void CSVLogFile::ready()
{
   debug("ready to write");
   digitalWrite(_errorLedPin, LOW);
   digitalWrite(_writeLedPin, HIGH);
}


void CSVLogFile::setFileName()
{
  if (_needNewFileName){
     int n = 0;  
     snprintf(_filename, sizeof(_filename), "data%04d.csv", n); 
     while(SD.exists(_filename)) {
       n++;
       snprintf(_filename, sizeof(_filename), "data%04d.csv", n); 
     };
     _newFile = true;
     _needNewFileName = false;
     _fileCount = n + 1;//begins to count on 0
     debug("filecount: " +String(_fileCount));
  };
}


void CSVLogFile::debug(String msg)
{
  if (_debug)
  {
    Serial.println("CSVLogFile - " + String(msg));
  }
}

