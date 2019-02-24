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
  void run();
  void onErrorEvent(void *error_event());
  void onPauseEvent(void *pending_event());
  void onWriteEvent(void *write_event());
 
private:
     unsigned long prev_write_mills;
     unsigned long prev_pendiing_mills;
     
     
	   uint8_t chip_select_pin;
     uint8_t log_pause_button_pin;
     uint8_t state;
	   int postfix__filenumber;
     bool in_debug_mode;
     bool start_a_new_file;
     char filename[16];
     String log_data;
     bool open_file_and_write_data;
     void statemachine_run();
     void set_new_filename();
     void do_state_init();
     void do_state_error();
     void do_state_pending();
     void do_state_check();
     void do_state_write();
     void write_debug_msg(String msg);
     void (*do_error_event)();
     void (*do_pending_event)();
     void (*do_write_event)();
     void write_event();
     void pending_event();
     void error_event();
};
#endif
