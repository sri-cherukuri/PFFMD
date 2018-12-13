// Arduino driver
// Receives raw data from four S401 load cells and prints timestamp and data to serial
// by Sri Cherukuri

#include <Wire.h>

#define led 13

#define NAN "NaN"

#define INDEX_ADDRESS 0x7A
#define INDEX_ZERO_BAL 1031 
#define INDEX_200N 31003

#define MIDDLE_ADDRESS 0x7B
#define MIDDLE_ZERO_BAL 1035
#define MIDDLE_200N 31023

#define RING_ADDRESS 0x7C
#define RING_ZERO_BAL 1030
#define RING_200N 31043

#define PINKY_ADDRESS 0x7D
#define PINKY_ZERO_BAL 1041
#define PINKY_200N 31002

void setup() {
  Wire.begin();       // join i2c bus
  Serial.begin(9600); // start serial communication at 9600 bps

  while(!Serial) ;
}

unsigned long startMillis = 0;
bool start = false;
int reading = 0;

String outputString = "";
String inputString = "TF";

void loop() {
  if(inputString == "TO")
  {
    digitalWrite(led, HIGH);

    outputString = String((double)(millis() - startMillis)/1000, 3);
    outputString += "\t";

    Wire.requestFrom(INDEX_ADDRESS, 2);
    if(2 <= Wire.available())
    {
      reading = Wire.read();
      reading = reading << 8;
      reading |= Wire.read();
      outputString += String((((float)reading - INDEX_ZERO_BAL)*200/(INDEX_200N - INDEX_ZERO_BAL)), 3);
    }
    else
      outputString += NAN;
    outputString += "\t";

    Wire.requestFrom(MIDDLE_ADDRESS, 2);
    if(2 <= Wire.available())
    {
      reading = Wire.read();
      reading = reading << 8;
      reading |= Wire.read();
      outputString += String((((float)reading - MIDDLE_ZERO_BAL)*200/(MIDDLE_200N - MIDDLE_ZERO_BAL)), 3);

    }
    else
      outputString += NAN;
    outputString += "\t";

    Wire.requestFrom(RING_ADDRESS, 2);
    if(2 <= Wire.available())
    {
      reading = Wire.read();
      reading = reading << 8;
      reading |= Wire.read();
      outputString += String((((float)reading - RING_ZERO_BAL)*200/(RING_200N - RING_ZERO_BAL)), 3);
    }
    else
      outputString += NAN;
    outputString += "\t";

    Wire.requestFrom(PINKY_ADDRESS, 2);
    if(2 <= Wire.available())
    {
      reading = Wire.read();
      reading = reading << 8;
      reading |= Wire.read();
      outputString += String((((float)reading - PINKY_ZERO_BAL)*200/(PINKY_200N - PINKY_ZERO_BAL)), 3);
    }
    else
      outputString += NAN;

    outputString += "\n";
    Serial.print(outputString);
  }

  if(inputString == "TF")
  {
     digitalWrite(led, LOW);
  }
 
  // read SerialEvents
  if(Serial.available())
    inputString = "";
  
  while(Serial.available())
    inputString += (char)Serial.read();

  if(inputString == "TO" && !start)
  {
    startMillis = millis();
    start = true;

    //Serial.println("millis\tINDEX\tMIDDLE\tRING\tPINKY");
  }
  else if(inputString == "TF" && start)
    start = false;


}
