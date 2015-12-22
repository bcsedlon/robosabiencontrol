#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11); 
// creates a "virtual" serial port/UART
// connect BT module TX to D10
// connect BT module RX to D11
// connect BT Vcc to 5V, GND to GND


/*
Based on IRSend demo from ken Shirriffs library -
this sends simple commands to a RoboSapien V1 using
and IR Transmitter with Arduino
*/
 
#include <IRremote.h>
IRsend irsend;        // pin 3 as IR LED output
int data;
void setup()
{
  pinMode(13, OUTPUT);
  BT.begin(9600);
}
 
void loop() {
  while (BT.available()> 0) {
    data = BT.parseInt();
    if(data > 0) {
      BT.println(data);
      digitalWrite(13, HIGH);
      irsend.sendRSV1(data);
      //irsend.sendRSV1(data);
      digitalWrite(13, LOW);
    }    
  }
}
