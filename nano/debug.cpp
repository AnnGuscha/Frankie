#include <PID_v1.h>

#include <Arduino.h>
#include <Servo.h> 
#include "config.h"
#include "debug.h"
#include "setup.h"

//UART
char data; // this var is used for reading data from uart

void printDouble( double val, byte precision){
  // prints val with number of decimal places determine by precision
  // precision is a number from 0 to 6 indicating the desired decimial places
  // example: printDouble( 3.1415, 2); // prints 3.14 (two decimal places)

  Serial.print (int(val));  //prints the int part
  if( precision > 0) {
    Serial.print("."); // print the decimal point
    unsigned long frac;
    unsigned long mult = 1;
    byte padding = precision -1;
    while(precision--)
       mult *=10;
       
    if(val >= 0)
      frac = (val - int(val)) * mult;
    else
      frac = (int(val)- val ) * mult;
    unsigned long frac1 = frac;
    while( frac1 /= 10 )
      padding--;
    while(  padding--)
      Serial.print("0");
    Serial.print(frac,DEC) ;
  }
}

//Define Variables we'll be connecting to
//double Setpoint, Input, Output,command = 0;
//PID myPID(&Input, &Output, &Setpoint,.02,0,0, DIRECT);

void setup_debug()
{  
  robotInit();
  delay(1000);
  
  getServo();
  writeServo(-80);
  
}





/*


int ChanelA , ChanelB;
int speedrobot = 0;
#define LOOPTIME 100
unsigned long lastMilli = 0;
int speed_act = 0;                              // speed (actual value)
int PWM_val = 0;                                // (25% = 64; 50% = 127; 75% = 191; 100% = 255)
int voltage = 0;                                // in mV
int current = 0;                                // in mA
volatile long count = 0;                        // rev counter
float Kp =   .1;                                // PID proportional control Gain
float Kd =   .01; 
int speedRobo = 0;

int updatePid(int command, int targetValue, int currentValue)   {             // compute PWM value
float pidTerm = 0;                                                            // PID correction
int error=0;                                  
static int last_error=0;                             
  error = abs(targetValue) - abs(currentValue); 
  pidTerm = (Kp * error) + (Kd * (error - last_error));                            
  last_error = error;
  return constrain(command + int(pidTerm), 0, 255);
}

*/



void loop_debug()
{
  /*
  
   getSensors(sensor);
  // getSensors(sensor);
  // getSensors(sensor);
  
   angle = 0;
   if(sensor[CENTER]<90)
   {
     if(sensor[BLEFT]>sensor[BRIGHT]) angle = -80;
     else  angle = 80;
   }
   writeServo(angle);
  
  if(angle!=0) speedRobo = 2000;
  else speedRobo = 1000;
  
  
 // ChanelA = constrain(map(pulseIn(7,HIGH,20000),1220,1732,0,100),0,100);
  //ChanelB = constrain(map(pulseIn(8,HIGH,20000),1220,1732,0,100),0,100);
  //Kd= (float)ChanelB/(float)100.00;
  
  // PID
   if((millis()-lastMilli) >= LOOPTIME)   {                                    // enter tmed loop
    lastMilli = millis();
    speedrobot = getSpeed();
    PWM_val= updatePid(PWM_val, speedRobo, speedrobot);
    forward(PWM_val);
   }
 //
  
  Serial.println(speedrobot);//Serial.print("  ");//Serial.print(ChanelB);//Serial.print("  ");
  //printDouble(Kp,10);Serial.print("  "); printDouble(KD,10);Serial.print("  ");
  //printDouble(Kd,10);
  //Serial.println("  ");
  //forward(255);
 
 
  
  delay(10);
  */
}
