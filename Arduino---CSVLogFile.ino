// include the SD library:
#include <SPI.h>
#include <SD.h>
#include <Wire.h>



#include "CSVLogFile.h";
#include "LogRTC.h"

const int DS1307_ADDRESS = 0x68;
const int chipSelect = 10;
const int LED_ERROR = 2;
const int LED_WRITE = 3;
const int BUTTON_PIN = 6; 

// variables will change
float current_mA = 0;
float loadvoltage = 0;
String filename;
//int buttonState = 0; 

unsigned long previousMillis = 0;
unsigned long interval = 5000;


CSVLogFile logfile(chipSelect, BUTTON_PIN);
LogRTC LogRTC(0x68);

void setup() {
  
  pinMode(LED_ERROR,OUTPUT);
  pinMode(LED_WRITE,OUTPUT);

  Wire.begin();  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 
  logfile.begin();
  logfile.onWriteEvent(onWriteEvent);
  logfile.onErrorEvent(onErrorEvent);
  logfile.onPauseEvent(onPauseEvent);
}


void loop() {
 logfile.run();
  unsigned long currentMillis = millis();

  
  if (currentMillis - previousMillis >= interval)
  {

        previousMillis = currentMillis;
        //String dataString = String(currentMillis)+";"+String(loadvoltage)+";"+String(current_mA);
        String dataString = " ;"+String(loadvoltage)+";"+String(current_mA);
        logfile.logData(dataString,true);
        
        loadvoltage++;
        current_mA++;


        Serial.println(LogRTC.get_datetime());
        
  }
}

void onWriteEvent()
{
  digitalWrite(LED_ERROR, LOW);
  PIND |= (1<<PIND3);
}

void onErrorEvent()
{
  digitalWrite(LED_WRITE, LOW);
  digitalWrite(LED_ERROR, HIGH);
}

void onPauseEvent()
{
  digitalWrite(LED_ERROR, LOW);
  PIND |= (1<<PIND3);//blink stop/error led
}

