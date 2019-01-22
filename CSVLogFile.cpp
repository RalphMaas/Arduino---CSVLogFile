
#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <SPI.h>
#include <SD.h>
#include "CSVLogFile.h"

CSVLogFile::CSVLogFile(int csPin, int writeLedPin, int errorLedPin)
{
   _cs = csPin;
   _writeLedPin = writeLedPin;
   _errorLedPin = errorLedPin;
   
   _Debug = false;
}

void CSVLogFile::begin(int baudrate=9600, bool debug)
{
   _Debug = debug;
   Serial.begin(baudrate);
   
   pinMode(_writeLedPin, OUTPUT);
   pinMode(_errorLedPin, OUTPUT);
   
   debug("Initializing SD card...");

   if (!SD.begin(_cs)) {
       debug("Card failed, or not present");
       error();
   } else {
       ready();
       debug("card initialized.");
   };
}

void CSVLogFile::writeLog(String data,String filename)
{
    File dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
       debug(data);

       if (doWriteEvent())
       {
           _doWriteEvent();
           debug("doWriteEvent is set");
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
      debug("error opening datalog.txt");
      error();
   }
}

void CSVLogFile::onWriteEvent(void *doWriteEvent())
{
  _doWriteEvent = doWriteEvent;
  debug("Call callback");
}

//private methods
void CSVLogFile::error()
{
    digitalWrite(_errorLedPin, HIGH);
    digitalWrite(_writeLedPin, LOW);
}

void CSVLogFile::ready()
{
   digitalWrite(_errorLedPin, LOW);
   digitalWrite(_writeLedPin, HIGH);
}

void CSVLogFile::debug(String msg)
{
  if (_useLogging)
  {
    Serial.println("CSVLogFile - " + String(msg));
  }
}

