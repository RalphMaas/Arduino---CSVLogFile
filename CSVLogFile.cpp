
#if (ARDUINO >=100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>
#include <SD.h>


#include "CSVLogFile.h";

enum sd_state_enum { STATE_INIT, STATE_ERROR, STATE_PENDING, STATE_CHECK, STATE_WRITE };

CSVLogFile::CSVLogFile(uint8_t csPin, uint8_t buttonPin)
{
	chip_select_pin = csPin;
	log_pause_button_pin = buttonPin;
	start_a_new_file = true;
	state = STATE_INIT;
	postfix__filenumber = 0;
	prev_write_mills = 0;
	prev_pendiing_mills = 0;
	log_data = "";
	in_debug_mode = false;
	open_file_and_write_data = true;
}

void CSVLogFile::logData(String data, bool debug = false)
{
	log_data = data;
	in_debug_mode = debug;
	open_file_and_write_data = true;
}

void CSVLogFile::begin()
{
	pinMode(log_pause_button_pin, INPUT);
}

void CSVLogFile::run()
{
	statemachine_run();
}

void CSVLogFile::onErrorEvent(void *error_event())
{
	do_error_event = error_event;
}

void CSVLogFile::onPauseEvent(void *pending_event())
{
	do_pending_event = pending_event;
}

void CSVLogFile::onWriteEvent(void *write_event())
{
	do_write_event = write_event;
}

//private methods
void CSVLogFile::statemachine_run()
{
	//write_debug_msg("state = "+String(state));

	switch (state)
	{
  	case STATE_INIT:
  		do_state_init();
  		break;
  
  	case STATE_ERROR:
  		do_state_error();
  		break;
  
  	case STATE_PENDING:
  		do_state_pending();
  		break;
  
  	case STATE_CHECK:
  		do_state_check();
  		break;
  
  	case STATE_WRITE:
  		do_state_write();
  		break;
	}
}

void CSVLogFile::do_state_init()
{
	SD.begin(chip_select_pin) ? state = STATE_PENDING : state = STATE_ERROR;
}

void CSVLogFile::do_state_error()
{
	error_event();
	state = STATE_INIT;
}

void CSVLogFile::do_state_pending()
{
	if (digitalRead(log_pause_button_pin) == HIGH) {
		state = STATE_CHECK;
		write_event();
	}
	else {
		state = STATE_PENDING;
		start_a_new_file = true;
		pending_event();
	}
}

void CSVLogFile::set_new_filename()
{
	snprintf(filename, sizeof(filename), "data%04d.csv", postfix__filenumber);
	while (SD.exists(filename)) {
		postfix__filenumber = postfix__filenumber + 1;
		snprintf(filename, sizeof(filename), "data%04d.csv", postfix__filenumber);
	};

	write_debug_msg("filename = " + String(filename));
	start_a_new_file = false;
}

void CSVLogFile::do_state_check()
{
	if (start_a_new_file)
	{
		set_new_filename();
	}
	state = STATE_WRITE;
}

void CSVLogFile::do_state_write()
{
	if (open_file_and_write_data)
	{
		File dataFile = SD.open(filename, FILE_WRITE);

		if (dataFile) {
			//    dataFile.println(_csvHeader);
			dataFile.println(log_data);
			dataFile.close();
			open_file_and_write_data = false;

			write_debug_msg(log_data);
			state = STATE_PENDING;
		}
		else {
			write_debug_msg(F("error in doWrite"));
			state = STATE_ERROR;
		}
	}
	else
	{
		state = STATE_PENDING;
	}
}

void CSVLogFile::write_event()
{
	unsigned long cur_mill = millis();
	if (cur_mill - prev_write_mills >= 500)
	{
		prev_write_mills = cur_mill;
		if (do_write_event) { do_write_event(); }
	}
}

void CSVLogFile::pending_event()
{
	unsigned long cur_mill = millis();
	if (cur_mill - prev_pendiing_mills >= 2000)
	{
		prev_pendiing_mills = cur_mill;
		if (do_pending_event) { do_pending_event(); }
	}
}

void CSVLogFile::error_event()
{
	if (do_error_event) { do_error_event(); }
}


void CSVLogFile::write_debug_msg(String msg)
{
	if (in_debug_mode)
	{
		Serial.println("CSVLogFile - " + String(msg));
	}
}

