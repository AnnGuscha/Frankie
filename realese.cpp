
#include <Arduino.h>
#include <Servo.h> 
#include <PID_v1.h>
#include "config.h"
#include "realese.h"
#include "setup.h"
#include "logic.h"

//VARIABLEAS 
int sensor[5] = {
  0,0,0,0,0};
int angle = 0;
int speedR = 0;

#define CNTR 0
#define LWAL 2
#define RWAL 3




void setup_realese(){  
  robotInit();
 
}



int ChanelA , ChanelB;
int speedrobot = 0;
#define LOOPTIME 40
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








void loop_realese()
{  
  while(1)
  {    


    // button start
startt:
    if(digitalRead(BUTTON)==LOW)
    {

      //neutral();
      //delay(500);
      //goto startt;
    }

    getSensors(sensor);
    getSensors(sensor);
    getSensors(sensor);

    angle = 0;
    //if(sensor[LEFT]<40) angle = 5;
    //if(sensor[RIGHT]<40) angle = -5;
    if(sensor[CENTER]<90)
    {
      if(sensor[BLEFT]>sensor[BRIGHT]) angle = -80;
      else  angle = 80;
    }
     

    writeServo(angle);
    if((sensor[CENTER]>120)&&(range(getServo(),-2,2))) speedR =2500;     
    else
    {
    if((sensor[CENTER]>90)&&(sensor[CENTER]<120)&&(range(getServo(),-7,7))) speedR =1700; 
    else speedR =1200; 
    }
  
   // PID
   if((millis()-lastMilli) >= LOOPTIME)   {                                    // enter tmed loop
    lastMilli = millis();
    speedrobot = getSpeed();
    PWM_val= updatePid(PWM_val, speedR, speedrobot);
    forward(PWM_val);
   }
 //
  




    //contact

    if(sensor[RIGHT]<25) {
      mLeft();
      getSensors(sensor);
    }
    else

        if(sensor[LEFT]<29) {
        mRight();
        getSensors(sensor);
      }
      else

          if(sensor[CENTER]<25) 
        {
          if (sensor[BLEFT]>sensor[BRIGHT])
          {
            mLeft();
            getSensors(sensor);
          }
          else
          {
            mRight();
            getSensors(sensor);
          }
        }


    //print results
/*
    Serial.print(angle);
    Serial.print("  ");
    Serial.print(speedR);
    Serial.print("  ");
    Serial.print(sensor[BLEFT]);
    Serial.print("  ");
    Serial.print(sensor[LEFT]);
    Serial.print("  ");
    Serial.print(sensor[CENTER]);
    Serial.print("  ");
    Serial.print(sensor[RIGHT]);
    Serial.print("  ");
    Serial.println(sensor[BRIGHT]);
    delay(500);
*/
  }  
}
