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
int buttonState = 0; 

unsigned long previousMillis = 0;
unsigned long interval = 1000;

//File dataFile;

CSVLogFile logfile(chipSelect,LED_WRITE,LED_ERROR);

void setup() {
  //pinMode(LED_ERROR, OUTPUT);
  //pinMode(LED_WRITE, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
    
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

Serial.begin(9600);
//  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
 /* if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    digitalWrite(LED_ERROR, HIGH);
    digitalWrite(LED_WRITE, LOW);
    return;
  } else {
    digitalWrite(LED_ERROR, LOW);
    digitalWrite(LED_WRITE, HIGH);
  }
  //PIND |= (1<<PIND3);
*/
  logfile.begin(9600,true);
  logfile.setOnWritingCallback(onWritingToFile);
 // Serial.println("card initialized.");
  //filename = "log.csv";
}


void loop() {
  unsigned long currentMillis = millis();
  buttonState = digitalRead(BUTTON_PIN);
  
  if (currentMillis - previousMillis >= interval)
  {
        previousMillis = currentMillis;
        // make a string for assembling the data to log:
        String dataString = "";
    
        // read three sensors and append to the string:
        dataString = String(currentMillis)+";"+String(loadvoltage)+";"+String(current_mA);
       
        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.

        if (buttonState == HIGH) {
              // new filename
          logfile.writeLog(dataString,"log.csv");
          
       /*     File dataFile = SD.open(filename, FILE_WRITE);
            // if the file is available, write to it:
            if (dataFile) {
              digitalWrite(LED_ERROR, LOW);
              dataFile.println(dataString);
              PIND |= (1<<PIND3);
              dataFile.close();
    
              // print to the serial port too:
              Serial.println(dataString);
            }
            else {
              Serial.println("error opening datalog.txt");
              digitalWrite(LED_WRITE, LOW);
            }
            */
        } else {
          //logging off
          digitalWrite(LED_WRITE, LOW);
        }
        
      loadvoltage++;
      current_mA++;
  }
}

void onWritingToFile()
{
  PIND |= (1<<PIND3);
}

