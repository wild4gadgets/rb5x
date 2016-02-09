// RB5X Control Script; receives commands from the Raspberry Pi and converts them into commands for the Sabertooth 2X12 controller

#include <SoftwareSerial.h>
#include <SabertoothSimplified.h>
#define FWD_SPEED_L 64
#define FWD_SPEED_R 70
#define REV_SPEED_L -64
#define REV_SPEED_R -70
#define STOP 48
#define FWD 49
#define REV 50
#define RIGHT 1
#define LEFT 2
#define CW 51
#define CCW 52
#define DEBUG 0
SoftwareSerial SWSerial(NOT_A_PIN, 18); // RX on no pin (unused), TX on pin 11 (to S1).
SabertoothSimplified ST(SWSerial); // Use SWSerial as the serial port.
int incomingbyte = 0;
int state = STOP;
int state_change = 0;
void setup()
{
  SWSerial.begin(9600); // serial connection to Sabertooth
  Serial.begin(9600);   // serial connection to Raspberry Pi (via USB)
}

void loop()
{
      
   // Start loop looking for commands from the RPi
   // Command structure
   // 0 = STOP = 48
   // 1 = FWD = 49
   // 2 = REV = 50
   // 3 = CW = 51
   // 4 = CCW = 52
 
   if (Serial.available() > 0) {
   // read the incoming byte
   incomingbyte=Serial.read();
  
     if (state != incomingbyte) {
       state = incomingbyte;
       state_change = 1;
     }
   #ifdef DEBUG 
   Serial.println(incomingbyte, DEC);
   #endif
   }  
   if (state_change)
     switch(state)
     {
       case FWD:
         #ifdef DEBUG
         Serial.println("FWD");
         #endif
         move(FWD,50);
         break;
       case REV:
         #ifdef DEBUG
         Serial.println("REV");
         #endif
         move(REV,50);
         break;
     case CW:
         #ifdef DEBUG
         Serial.println("CW");
         #endif
         turn(CW,50);
         break;
      case CCW:
       #ifdef DEBUG
       Serial.println("CCW");
       #endif
       turn(CCW,50);
       break;
      case STOP:
       #ifdef DEBUG
       Serial.println("STOP");
       #endif
       move(STOP,50);
       break;
      default:
       break;
   }
   
    
}
  
void move(int direction, long msec)
{
    state_change=0;
    if (direction==FWD)
    {
      ST.motor(LEFT, FWD_SPEED_L);
      ST.motor(RIGHT, FWD_SPEED_R);
    }
    else if (direction==REV)
    {
       ST.motor(LEFT,REV_SPEED_L);
       ST.motor(RIGHT,REV_SPEED_R);
    }
    else // must be stop
    {
      ST.motor(LEFT,0);
      ST.motor(RIGHT,0);
    }
    delay(msec);
    return;
}


void turn(int direction, long msec)
{
  state_change=0;
  if (direction==CW)
  {
       ST.motor(RIGHT,REV_SPEED_R);
       ST.motor(LEFT,FWD_SPEED_L);
  }
  else if (direction==CCW)
  {
       ST.motor(RIGHT,FWD_SPEED_R);
       ST.motor(LEFT,REV_SPEED_L);
  }
   delay(msec);
   return;
}
