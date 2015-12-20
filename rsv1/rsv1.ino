
#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11); 

int IRIn = 2;            // We will use an interrupt
int IROut= 3;            // Where the echoed command will be sent from

// Some but not all RS commands are defined
#define RSTurnRight       0x80
#define RSRightArmUp      0x81
#define RSRightArmOut     0x82
#define RSTiltBodyRight   0x83
#define RSRightArmDown    0x84
#define RSRightArmIn      0x85
#define RSWalkForward     0x86
#define RSWalkBackward    0x87
#define RSTurnLeft        0x88
#define RSLeftArmUp       0x89
#define RSLeftArmOut      0x8A
#define RSTiltBodyLeft    0x8B
#define RSLeftArmDown     0x8C
#define RSLeftArmIn       0x8D
#define RSStop            0x8E
#define RSWakeUp          0xB1
#define RSBurp            0xC2
#define RSRightHandStrike 0xC0
#define RSNoOp            0xEF
boolean RSEcho=true;      // Should Arduino Echo RS commands
boolean RSUsed=true;      // Has the last command been used
volatile int RSBit=9;     // Total bits of data
volatile int RSCommand;   // Single byte command from IR
int bitTime=516;          // Bit time (Theoretically 833 but 516)
                          // works for transmission and is faster
int last;                 // Previous command from IR

String inString = "";    // string to hold input
int inInt;

void setup()                    
{
  pinMode(IRIn, INPUT);     
  pinMode(IROut, OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(IROut,HIGH);
  attachInterrupt(0,RSReadCommand,RISING);
  last=RSNoOp;

  BT.begin(9600);
  // Send test message to other device
  BT.println("Arduino Robosapien Control");
  BT.println("0 for test");
  BT.println("1 for help");
}

// Receive a bit at a time.
void RSReadCommand() {
  delayMicroseconds(833+208);  // about 1 1/4 bit times
  int bit=digitalRead(IRIn); 
  if (RSBit==9) { // Must be start of new command
    RSCommand=0;
    RSBit=0;
    RSUsed=true;
  }
  if (RSBit<8) { 
    RSCommand<<=1;
    RSCommand|=bit;
  }
  RSBit++;
  if (RSBit==9) RSUsed=false;
}

// send the whole 8 bits
void RSSendCommand(int command) {
  digitalWrite(IROut,LOW);
  delayMicroseconds(8*bitTime);
  for (int i=0;i<8;i++) {
    digitalWrite(IROut,HIGH);  
    delayMicroseconds(bitTime);
    if ((command & 128) !=0) delayMicroseconds(3*bitTime);
    digitalWrite(IROut,LOW);
    delayMicroseconds(bitTime);
    command <<= 1;
  }
  digitalWrite(IROut,HIGH);
  delay(250); // Give a 1/4 sec before next
}

void loop() // run over and over
{
    while (BT.available())
    {
        char character = BT.read(); // Receive a single character from the software serial port
        inString.concat(character); // Add the received character to the receive buffer
        if (character == '\n')
        {
            
            inInt = inString.toInt(); 
           
            
            BT.print("Received string: ");
            BT.println(inString);            
            BT.print("Received int: ");
            BT.println(inInt);


            
            // Add your code to parse the received line here....
     
            
            if (inInt==0) {
              BT.println("Testing...");
              
              digitalWrite(13,HIGH);  // Turn on LED let us know we have control our wedge
              RSSendCommand(RSRightArmOut);
              RSSendCommand(RSTiltBodyRight);
              RSSendCommand(RSRightArmDown);
            
              RSSendCommand(RSLeftArmOut);
              RSSendCommand(RSTiltBodyLeft);
              RSSendCommand(RSLeftArmDown);
              digitalWrite(13,LOW);
              BT.println("Done.");
            } 
/*
             if (inInt==1) {
              printHelp();
             }
*/
             if (inInt > 0) {
                BT.print("IRSend: ");
                BT.println(inInt);
                digitalWrite(13,HIGH);
                RSSendCommand(inInt);
                digitalWrite(13,LOW);                
                        
             }


            // Clear receive buffer so we're ready to receive the next line
            inString = "";
        }
    }
}






void printHelp() {  
/*
       BT.println(
"codes\n"
"t for test\n"
"\t"        
"Button Controller Function  Presses Arduino Code\n"
"12  R Turn Right  1 128\n"
"1 R Right Arm Up  2 129\n"
"4 R Right Arm Out 2 130\n"
"5 R Tilt Body right 1 131\n"
"2 R Right Arm Down  2 132\n"
"3 R Right Arm In  2 133\n"
"11  R Walk Forward  2 134\n"
"15  R Walk Backwards  2 135\n"
"14  R Turn Left 1 136\n"
"6 R Left Arm Up 2 137\n"
"9 R Left Arm Out  2 138\n"
"10  R Tilt Body Right 1 139\n"
"7 R Left Arm Down 2 140\n"
"8 R Left Arm In 2 141\n"
"13  R Stop  1 142\n"
"19  R Master Command Program  1 144\n"
"20  R Program Play  1 145\n"
"16  R Right Sensor Program  1 146\n"
"18  R Left Sensor Program 1 147\n"
"17  R Sonic Sensor Program  1 148\n"
"12  G Right Turn Step 1 160\n"
"1 G Right Hand Thump  1 161\n"
"4 G Right Hand Throw  1 162\n"
"5 G Sleep 1 163\n"
"2 G Right Hand Pickup 1 164\n"
"3 G Lean Backward 1 165\n"
"11  G Forward Step  1 166\n"
"15  G Backward Step 1 167\n"
"14  G Left Turn Step  1 168\n"
"6 G Left Hand thump 1 169\n"
"9 G Left hand Throw 1 170\n"
"10  G Listen  1 171\n"
"7 G Left Hand Pickup  1 172\n"
"8 G Lean Forward  1 173\n"
"13  G Reset 1 174\n"
"19  G Master Command Program  1 176\n"
"20  G Wake Up 1 177\n"
"16  G Right Sensor Program Execute  1 178\n"
"18  G Left Sensor Program Execute 1 179\n"
"17  G Sonic Sensor Program Execute  1 180\n"
"12  O  Right Hand Strike 3 1 192\n"
"1 O  Right Hand Sweep  1 193\n"
"4 O  Burp  1 194\n"
"5 O  Right Hand Strike 1 195\n"
"2 O  High 5  1 196\n"
"3 O  Right Hand Strike 1 197\n"
"11  O  Bulldozer 1 198\n"
"15  O  Oops  1 199\n"
"14  O  Left Hand Strike 3  1 200\n"
"6 O  Left Hand Sweep 1 201\n"
"9 O  Whistle 1 202\n"
"10  O  Left Hand Strike  1 203\n"
"7 O  Talk Back 1 204\n"
"8 O  Left Hand Strike  1 205\n"
"13  O  Roar  1 206\n"
"19  O  All Demo  1 208\n"
"20  O  Power Off 1 209\n"
"16  O  Demo 1  1 210\n"
"18  O  Demo 2  1 211\n"
"17  O  Dance Demo  1 212");
    
*/
}
void loop2() 
{ 
  if (!RSUsed) {
    if (RSCommand==RSStop && last==RSStop) RSEcho=!RSEcho;
    last=RSCommand;
    if (!RSEcho){
      digitalWrite(10,HIGH);  // Turn on LED let us know 
                              //  we have control our wedge
      RSSendCommand(RSRightArmOut);
      RSSendCommand(RSTiltBodyRight);
      RSSendCommand(RSRightArmDown);

      RSSendCommand(RSLeftArmOut);
      RSSendCommand(RSTiltBodyLeft);
      RSSendCommand(RSLeftArmDown);

      RSSendCommand(RSWalkForward);

    } else {
      digitalWrite(10,LOW);   // No longer in control
      RSSendCommand(RSCommand);
    }
    RSUsed=true;
  }
}
