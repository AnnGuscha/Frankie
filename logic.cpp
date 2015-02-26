#include <Arduino.h>
//#include <MsTimer2.h>
#include <Servo.h> 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"
#include "realese.h"
#include "setup.h"



void mLeft(void)
{
  
  neutral();
  delay(50);
  
  writeServo(SERVO_MAX_ANGLE);
  delay(120);
  back(150);
  delay(320);
  neutral();
  writeServo(-SERVO_MAX_ANGLE);
  delay(100);
  forward(200);
  delay(100); 
  
 
}
void mRight(void)
{
  neutral();
  delay(50);
  writeServo(-SERVO_MAX_ANGLE);
  delay(120);
  back(150);
  delay(320);
  neutral();
  writeServo(SERVO_MAX_ANGLE);
  delay(100);
  forward(200);  
  delay(100); 
}


