// Basic bluetooth test sketch. HC-06_01_9600
// HC-06 ZS-040
//
//
//  Uses hardware serial to talk to the host computer and software serial for communication with the bluetooth module
//
//  Pins
//  BT VCC to Arduino 5V out.
//  BT GND to GND
//  BT RX to Arduino pin 3 (through a voltage divider)
//  BT TX to Arduino pin 2 (no need voltage divider)
//
//  When a command is entered in the serial monitor on the computer
//  the Arduino will relay it to the bluetooth module and display the result.
//
//  These HC-06 modules require capital letters and no line ending
//

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // RX | TX

void setup()
{
  Serial.begin(9600);
  Serial.println("Arduino with HC-06 is ready");

  // HC-06 default baud rate is 9600
  BTSerial.begin(9600);
  Serial.println("BTserial started at 9600");
  BTSerial.write('0');
}

void loop()
{
  BTSerial.write('0');
  delay(1000);
  BTSerial.write('1');
  delay(1000);
}
