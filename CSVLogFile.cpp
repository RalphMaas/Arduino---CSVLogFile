
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
       debug(F("card failed, or not present"));
       error();
   } else {
       ready();
       debug(F("card initialized."));
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
  debug(F("call callback"));
}


void CSVLogFile::onStopEvent(void *doStopEvent())
{
  _doStopEvent = doStopEvent;
  debug(F("call callback"));
}

int CSVLogFile::fileCount()
{
  return _fileCount;
}

//private methods
void CSVLogFile::doWrite()
{
    ready();
    debug(F("write to file :"));
    debug(String(_filename));
    if (_doWriteEvent)
    {
      _doWriteEvent();
    }
}

void CSVLogFile::stop()
{
    debug(F("writing stop"));
    digitalWrite(_writeLedPin, LOW);
    digitalWrite(_errorLedPin, HIGH);
    _needNewFileName = true;
    delay(100);
    if (_doStopEvent)
    {
      _doStopEvent();
    }

}

void CSVLogFile::error()
{
    debug(F("error occured"));
    digitalWrite(_errorLedPin, HIGH);
    digitalWrite(_writeLedPin, LOW);
}

void CSVLogFile::ready()
{
   debug(F("ready to write"));
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
  };
}


void CSVLogFile::debug(String msg)
{
  if (_debug)
  {
    Serial.println("CSVLogFile - " + String(msg));
  }
}

