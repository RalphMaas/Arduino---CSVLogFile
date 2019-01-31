
#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <SPI.h>
#include <SD.h>
#include "CSVLogFile.h"

enum sd_state_enum { INIT, INIT_ERROR, PENDING, CHECKCARD, WRITE, ERROR };

CSVLogFile::CSVLogFile(uint8_t csPin, uint8_t buttonPin)
{
   _cs = csPin;
   _buttonPin = buttonPin;
   _new_file = true;
   _state = INIT;
}

void CSVLogFile::logData(String data, bool debug = false)
{
    _data = data;
    _use_debug = debug;
    _state_machine_run();
}

void CSVLogFile::begin()
{
  pinMode(_buttonPin,INPUT);
}


void CSVLogFile::onErrorEvent(void *doErrorEvent())
{
  _doErrorEvent = doErrorEvent;
}

void CSVLogFile::onPauseEvent(void *doPendingEvent())
{
  _doPendingEvent = doPendingEvent;
}

void CSVLogFile::onWriteEvent(void *doWriteEvent())
{
  _doWriteEvent = doWriteEvent;
}

//private methods
void CSVLogFile::_state_machine_run() 
{
  _debug("state = "+String(_state));
  
  switch(_state)
  {
    case INIT:
      _doInit();
      break;

    case INIT_ERROR:
      _doInitiError();
      break;

    case PENDING:
      _doPending();
      break;

    case CHECKCARD:
     _doCheckCard();
      break;

    case WRITE:
     _doWrite();
      break;

  }
}

void CSVLogFile::_doInit()
{
  SD.begin(_cs) ? _state = PENDING : _state = INIT_ERROR;
}

void CSVLogFile::_doInitiError()
{
   if(_doErrorEvent){ _doErrorEvent(); }
   _state = INIT;
}

void CSVLogFile::_doError()
{
   if(_doErrorEvent){ _doErrorEvent(); }
   _state = PENDING;
}

void CSVLogFile::_doPending()
{
   if( digitalRead(_buttonPin) == HIGH){
      _state = CHECKCARD;
   } else{
      _state = PENDING;
      _new_file =true;
      if(_doPendingEvent){ _doPendingEvent(); }
   }
}

void CSVLogFile:: _getNewFileName()
{
  int n = 0;  
  snprintf(_filename, sizeof(_filename), "data%04d.csv", n); 
  while(SD.exists(_filename)) {
    n++;
    snprintf(_filename, sizeof(_filename), "data%04d.csv", n); 
  };
  _new_file = false;
}
void CSVLogFile:: _doCheckCard()
{
 //check to access card
  if (SD.open("/")){
      if(_new_file)
      {
        _getNewFileName();
      }
      _debug("filename = "+String(_filename));
      _state = WRITE;
  }
  else
  {
    _debug("Error in doCheckCard");
    _state = INIT_ERROR;
  }
}

void CSVLogFile::_doWrite()
{
    File dataFile = SD.open(_filename, FILE_WRITE);
    if (dataFile) {
      if(_doWriteEvent){ 
        _debug("call write event");
        _doWriteEvent(); 
        }

      //    dataFile.println(_csvHeader);
      _debug(_data);
      dataFile.println(_data);
      dataFile.close();
      _state = PENDING;
    }
    else {
      _state = INIT_ERROR;
  }
}

void CSVLogFile::_debug(String msg)
{
  if (_use_debug)
  {
    Serial.println("CSVLogFile - " + String(msg));
  }
}

