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
unsigned long interval = 5000;

CSVLogFile logfile(chipSelect,LED_WRITE,LED_ERROR,BUTTON_PIN);

void setup() {
  //pinMode(BUTTON_PIN, INPUT);
    
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

Serial.begin(9600);
  logfile.begin(9600,"logging.csv", true);
  logfile.onWriteEvent(onWriteEvent);
}


void loop() {
  unsigned long currentMillis = millis();
  buttonState = digitalRead(BUTTON_PIN);
  
  if (currentMillis - previousMillis >= interval)
  {
        previousMillis = currentMillis;
        String dataString = String(currentMillis)+";"+String(loadvoltage)+";"+String(current_mA);
        logfile.writeData(dataString);
        loadvoltage++;
        current_mA++;
  }
}

void onWriteEvent()
{
  PIND |= (1<<PIND3);
 
}

