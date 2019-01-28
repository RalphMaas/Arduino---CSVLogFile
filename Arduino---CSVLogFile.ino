// include the SD library:
#include <SPI.h>
#include <SD.h>
#include "CSVLogFile.h";


const int chipSelect = 10;
const int LED_ERROR = 2;
const int LED_WRITE = 3;
const int BUTTON_PIN = 4; 

// variables will change
float current_mA = 0;
float loadvoltage = 0;
String filename;
//int buttonState = 0; 

unsigned long previousMillis = 0;
unsigned long interval = 2500;

CSVLogFile logfile(chipSelect, BUTTON_PIN);

void setup() {
  //pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_ERROR,OUTPUT);
  pinMode(LED_WRITE,OUTPUT);
    
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.begin(9600);
  logfile.begin();
  logfile.onWriteEvent(onWriteEvent);
  logfile.onErrorEvent(onErrorEvent);
  logfile.onPauseEvent(onPauseEvent);
}


void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
        previousMillis = currentMillis;
        String dataString = String(currentMillis)+";"+String(loadvoltage)+";"+String(current_mA);
        logfile.logData(dataString,true);
        
        loadvoltage++;
        current_mA++;
  }
}

void onWriteEvent()
{
  PIND |= (1<<PIND3);//blink write led
  Serial.println("onWriteEvent fired");
}

void onErrorEvent()
{
  digitalWrite(LED_ERROR, HIGH);
}

void onPauseEvent()
{
  PIND |= (1<<PIND2);//blink stop/error led
}

