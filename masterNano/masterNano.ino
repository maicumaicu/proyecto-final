#include <SoftwareSerial.h>0

SoftwareSerial BTserial(2, 3); // RX | TX

char c = ' ';

void setup() 

{

    Serial.begin(9600);

    Serial.println("__________Engineers Garage______________");

    Serial.println("---------------Master------------------");

    // HC-05 default serial speed for AT mode is 38400

    BTserial.begin(38400);  

}

void loop()

{

    // Keep reading from HC-05 and send to Arduino Serial Monitor

    if (BTserial.available())

    {  

        c = BTserial.read();

        Serial.write(c);

    }

    // Keep reading from Arduino Serial Monitor and send to HC-05

    if (Serial.available())

    {

        c =  Serial.read();

        BTserial.write(c);  

   }

}
