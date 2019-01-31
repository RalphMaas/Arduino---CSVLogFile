
#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <SPI.h>
#include <SD.h>
#include "CSVLogFile.h"

enum sd_state_enum { INIT, IN_ERROR, PENDING, CHECKCARD, WRITE};

CSVLogFile::CSVLogFile(uint8_t csPin, uint8_t buttonPin)
{
   _cs = csPin;
   _buttonPin = buttonPin;
   _new_file = true;
   _state = INIT;
   _fileNumber = 0;
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

    case IN_ERROR:
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
  SD.begin(_cs) ? _state = PENDING : _state = IN_ERROR;
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

void CSVLogFile:: _setNewFileName()
{
  snprintf(_filename, sizeof(_filename), "data%04d.csv", _fileNumber); 
  while(SD.exists(_filename)) {
    _fileNumber = _fileNumber + 1;
    snprintf(_filename, sizeof(_filename), "data%04d.csv", _fileNumber); 
  };
 
  _debug("filename = "+String(_filename));
  _new_file = false;
}

void CSVLogFile:: _doCheckCard()
{
  if(_new_file)
  {
    _setNewFileName();
  }
  _state = WRITE;
    
}

void CSVLogFile::_doWrite()
{
    File dataFile = SD.open(_filename, FILE_WRITE);
    if (dataFile) {
      if(_doWriteEvent){ _doWriteEvent(); }

      //    dataFile.println(_csvHeader);
      dataFile.println(_data);
      dataFile.close();

      _debug(_data);
      _state = PENDING;
    }
    else {
       _debug("error in doWrite");
      _state = IN_ERROR;
  }
}

void CSVLogFile::_debug(String msg)
{
  if (_use_debug)
  {
    Serial.println("CSVLogFile - " + String(msg));
  }
}

