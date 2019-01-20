#include <SPI.h>
#include <SD.h>

#include "Arduino.h"
#include "CSVLogFile.h"

CSVLogFile::CSVLogFile(int cs, int outputWrite, int outputError)
{
   _cs = cs;
   _outputWrite = outputWrite;
   _outputError = outputError;
   _useLogging = false;
}

void CSVLogFile::begin(int baudrate=9600, bool useLogging)
{
   _useLogging = useLogging;
   Serial.begin(baudrate);
   pinMode(_outputWrite, OUTPUT);
   pinMode(_outputError, OUTPUT);
   
   SerialLog("Initializing SD card...");
   if (!SD.begin(_cs)) {
       SerialLog("Card failed, or not present");
       error();
   } else {
       writing();
       SerialLog("card initialized.");
   };
}

void CSVLogFile::writeLog(String data,String filename)
{
    File dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
       SerialLog(data);
       writing();

       dataFile.println(data);
       _onWritingCallback();
       dataFile.close();
    }
    else {
      SerialLog("error opening datalog.txt");
      error();
   }
}


void CSVLogFile::setOnWritingCallback(void *onWritingCallback())
{
  _onWritingCallback = onWritingCallback;
  SerialLog("Call callback");
}

//private methods
void CSVLogFile::error()
{
    digitalWrite(_outputError, HIGH);
    digitalWrite(_outputWrite, LOW);
}

void CSVLogFile::writing()
{
   digitalWrite(_outputError, LOW);
   digitalWrite(_outputWrite, HIGH);
}

void CSVLogFile::SerialLog(String msg)
{
  if (_useLogging)
  {
    Serial.println("CSVLogFile - " + String(msg));
  }
}

