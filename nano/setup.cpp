#include <Arduino.h>
//#include <MsTimer2.h>
#include <Servo.h> 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"
#include "realese.h"
#include "setup.h"



// MOTOR

char motorDir = STOP; // 1 forward 0 stop -1 back
void motorInit()
{
  pinMode(MOTOR_DIR,OUTPUT);
  pinMode(MOTOR_EN,OUTPUT);
}
void forward(int value)
{
  motorDir=FORWARD;
  digitalWrite(MOTOR_DIR,HIGH);
  value=value>250?255:value;
  value=constrain(value,0,255);
  analogWrite(MOTOR_EN,value);
}
void neutral()
{
  motorDir=STOP;
  analogWrite(MOTOR_EN,0);
}
void back(int value)
{ 
  motorDir=BACK;
  digitalWrite(MOTOR_DIR,LOW);
  value=value>250?255:value;
  value=constrain(value,0,255);
  analogWrite(MOTOR_EN,value);
}


// speed of robot
unsigned long tPrev = 0; // time of previev interrupt it use for "drebezg"
unsigned long tCurr = 0; // time current interrupt it use for "drebezg"
int state = HIGH; // for led 

// interrupt
volatile unsigned long wheelCount = 0;
void motorTick() 
{
  //tCurr = micros();
  //if((tCurr - tPrev)>MIN_INT_TIC)
  //{
    //tPrev = tCurr; 
    wheelCount++;
  //}
  //led blink
  //state=!state;
  //digitalWrite(LED,state); 
}

unsigned long LastTime = 0;
unsigned long NowTime = 0;

int getSpeed() // mm/s
{
  NowTime = micros();
  int value = (int)((float)wheelCount*(float)SREDUKTOR/(float)((float)NowTime - (float)LastTime));
  //float value = wheelCount;
  LastTime = NowTime;
  wheelCount = 0;
  return value;
}

//interrupt init
void speedInit()
{
  attachInterrupt(0,motorTick,RISING);
}


// SERVO
Servo wheelServoR;

void getServoInit()
{
  pinMode(SERVOANALOGPIN,INPUT);
  digitalWrite(SERVOANALOGPIN,HIGH);
}
int getServo() //return servo
{
  //wheelServoR.detach();
  //return analogRead(7);
  int value = map(analogRead(SERVOANALOGPIN),SERVOMINVOLTAGE,SERVOMAXVOLTAGE,-SERVO_MAX_ANGLE,SERVO_MAX_ANGLE);
  //wheelServoR.attach(SERVO_PIN);
  return value;
}

// this function check value it return the value in diapazon from -30 to +30  
int checkServo( int value)
{
  value = value>(SERVO_MAX_ANGLE)?SERVO_MAX_ANGLE:value;
  value = value<(-SERVO_MAX_ANGLE)?-SERVO_MAX_ANGLE:value;
  return value;
}


// depends of global class wheelServo 
// this function write value to servo without waiting time
// this function use checkServo so the value could be any integer. 
int writeServo (int value )
{
  if (wheelServoR.attached() == false)
  {
    wheelServoR.attach(SERVO_PIN);
  }
  value = checkServo(value);
  wheelServoR.write(STCALC(value));  
  return value;
}



// return value in sm it used for convert from right analod sharp sensor to sm
double read_gp2d12_range(double value)
{
  double val;

  val = 8044.0721/(value*2 - 129.0100)-0.9581;
  if (val < 0) val = 80;
  if (val >80) val = 80;
  //val = (8762.88/(value/4 + 18.32)-12.06)/2;
  return val; 
}
// return value in sm it used for convert from center analod sharp sensor to sm
double read_gp2y0a_range(double value)
{
  double val;
  val = (8762.88/(value/2 + 18.32)-12.06);
  if (val < 0) val = 150;
  if (val >150) val = 150;

  return val; 
} 










void robotInit()
{

  analogReference(DEFAULT); //analog init  

  // init servo feedback
  getServoInit();


  //button init (set pul-up resistor)
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON,HIGH);

  // init morors and counter sensor
  motorInit();
  speedInit();

  // initialize the LED pin as an output:
  pinMode(LED,OUTPUT);     

  // init servo and set it to midle
  wheelServoR.attach(SERVO_PIN);  

  //init serial
  Serial.begin(SERIAL_SRPEED);

  // say START
  Serial.println("Start");
}


/// get sensors
int array[3][5];
void getSensors(int arrayone[])  // 6 milisekund
{


  int i1 = 0;
  array[0][0] = array[1][0];
  array[0][1] = array[1][1];    
  array[0][2] = array[1][2];
  array[0][3] = array[1][3];
  array[0][4] = array[1][4];

  array[1][0] = array[2][0];
  array[1][1] = array[2][1];
  array[1][2] = array[2][2];
  array[1][3] = array[2][3];
  array[1][4] = array[2][4];

  array[2][0] = analogRead(0);
  array[2][0] = read_gp2y0a_range(analogRead(0));
  array[2][1] = analogRead(1);
  array[2][1] = read_gp2y0a_range(analogRead(1));
  array[2][2] = analogRead(2);
  array[2][2] = read_gp2y0a_range(analogRead(2));
  array[2][3] = analogRead(3);
  array[2][3] = read_gp2y0a_range(analogRead(3));
  array[2][4] = analogRead(4);
  array[2][4] = read_gp2y0a_range(analogRead(4));

  for(i1 = 0;i1<5;i1++)
  {
    if((array[0][i1]>=array[1][i1])&&(array[0][i1]>=array[2][i1]))
    {
      if((array[1][i1]>array[2][i1])){
        arrayone[i1] = array[1][i1]; 
      }
      else      arrayone[i1] = array[2][i1]; 
    }

    if((array[1][i1]>=array[0][i1])&&(array[1][i1]>=array[2][i1]))
    {
      if((array[0][i1]>array[2][i1])){
        arrayone[i1] = array[0][i1]; 
      }
      else      arrayone[i1] = array[2][i1]; 
    }

    if((array[2][0]>=array[0][i1])&&(array[2][0]>=array[1][0]))
    {
      if((array[0][i1]>array[1][i1])){
        arrayone[i1] = array[0][i1]; 
      }
      else      arrayone[i1] = array[1][i1]; 
    }
  }

  /*
   analogRead(0);
   arrayone[0] = read_gp2y0a_range(analogRead(0));
   analogRead(1);
   arrayone[1] = read_gp2y0a_range(analogRead(1));
   analogRead(2);
   arrayone[2] = read_gp2y0a_range(analogRead(2));
   analogRead(3);
   arrayone[3] = read_gp2y0a_range(analogRead(3));
   analogRead(4);
   arrayone[4] = read_gp2y0a_range(analogRead(4));
   */
}

boolean range (int value, int rangeMin,int rangeMax )
{
  if ( (value>=rangeMin) && (value<=rangeMax) ) return HIGH;
  return LOW;    
}


/*

motorTimeTick = (tCurr - tPrev);
    tPrev = tCurr;

    // move
    if(speedControl)
    {
    if(gvalue>0)  
    {
      gvalue = constrain(gvalue,MINSPEED,MAXSPEED);
      if (motorDir!=FORWARD)    curSpeed=0;   

      //curSpeed = curSpeed + map((gvalue - getSpeed()),0,MAXSPEED,0,255);//regulator
      curSpeed = getSpeed()>gvalue?curSpeed-10:curSpeed+10;//regulator
      //Serial.println(curSpeed);
      curSpeed = constrain(curSpeed,0,255);
      forward(curSpeed);
    }
    if(gvalue<0) 
    {
      gvalue = abs(gvalue);
      gvalue = constrain(gvalue,MINSPEED,MAXSPEED);
      if (motorDir!=BACK)  curSpeed=0;     

      curSpeed = getSpeed()>gvalue?curSpeed-KP:curSpeed+KP;// rgulator

      curSpeed = constrain(curSpeed,0,255);
      back(curSpeed);
    } 
    if(gvalue==0) neutral();
    }

*/
