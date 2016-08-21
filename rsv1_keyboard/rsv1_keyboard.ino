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


//keyboard 4x4 i2c
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>
#define I2CADDR 0x20

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// Digitran keypad, bit numbers of PCF8574 i/o port
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574 );

void setup()
{
  pinMode(13, OUTPUT);
  BT.begin(9600);

  //keyboard 4x4 i2c
  Wire.begin( );
  kpd.begin( makeKeymap(keys) );
  Serial.begin(9600);
  Serial.println( "start" );
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
  
  char key = kpd.getKey();
  if (key){
    Serial.println(key);

    data = 0;
    switch (key) {
      case '0':
        //data = 209;//Power Off 1 209
        data = 212; //Dance Demo 1 212
        break;  
      case '1':
         data = 194;//Burp 1 194       
        break;
      case '2':
        data = 134;//Walk Forward 2 134
        break;
      case '3':
        data = 211;//Demo 1 2 211
        break;
      case '4':
        data = 136;//Turn Left 1 136
        break;
      case '5':
        data = 142;//Stop 1 142
        break;
      case '6':

        data = 128;//Turn Right 1 128
        break;
      case '7':
        data = 199;//Oops 1 199
        break;
      case '8':
        data = 135;//Walk Backwards 2 135
        break; 
      case '9':
        data = 198;//Bulldozer 1 198
        break;
      case 'A':
        data = 210;//Demo 1 1 210
        break;
      case 'B':
        data = 202;//Whistle  1 202
        break;
      case 'C':
        data = 196;//High 5 1 196        
        break;
      case 'D':
        data = 192;//Right Hand Strike 3 1 192
        break;
      case '*':
        data = 205;//Left Hand Strike 1 205
        break;
      case '#':
        data = 208;//All Demo 1 208
        break;
      default: 
        data = 0;
      break;
    }
    if(data>0) {
      digitalWrite(13, HIGH);
      irsend.sendRSV1(data);
      digitalWrite(13, LOW);
    }
  }
}

