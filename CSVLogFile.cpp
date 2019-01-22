
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
}

void CSVLogFile::begin(int baudrate=9600, String initalFileName, bool useDebug = false)
{
   _debug = useDebug;
   _filename = initalFileName;
  
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
        File dataFile = SD.open(_filename, FILE_WRITE);
        if (dataFile) {
          debug(data);

          if (_doWriteEvent)
          {
              _doWriteEvent();
              debug("doWriteEvent is attached");
          }
          else
          {
                debug("doWriteEvent is not attached");

          }
          ready();
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
void CSVLogFile::stop()
{
     debug("writing stop");
     digitalWrite(_writeLedPin, LOW);
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

void CSVLogFile::debug(String msg)
{
  if (_debug)
  {
    Serial.println("CSVLogFile - " + String(msg));
  }
}

