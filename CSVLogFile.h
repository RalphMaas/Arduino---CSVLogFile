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
  CSVLogFile(uint8_t csPin, uint8_t buttonPin);
  void logData(String data, bool debug = false);
  void begin();
  void onErrorEvent(void *doErrorEvent());
  void onPauseEvent(void *_doPendingEvent());
  void onWriteEvent(void *_doWriteEvent());
  private:
     uint8_t _cs;
     uint8_t _buttonPin;
     uint8_t _state;
     uint8_t _state_prev;
     int _fileNumber;
     bool _use_debug;
     bool _new_file;
     char _filename[16];
     String _data;

     void _state_machine_run();
     void _setNewFileName();
     void _doInit();
     void _doInitiError();
     void _doPending();
     void _doCheckCard();
     void _doWrite();
     void _doError();
     void _debug(String msg);
     void (*_doErrorEvent)();
     void (*_doPendingEvent)();
     void (*_doWriteEvent)();
};
#endif
