#include <PID_v1.h>

//#define DEBUG_MODE
#include <Arduino.h>
#include <Servo.h>
#include <PID_v1.h>
//#include <MsTimer2.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#ifdef DEBUG_MODE
  #include "debug.h"
#else 
  #include "realese.h"
#endif

void setup()
{
  #ifdef DEBUG_MODE
    setup_debug();
  #else
    setup_realese();
  #endif
}
void loop()
{
  #ifdef DEBUG_MODE
    loop_debug();
  #else
    loop_realese();
  #endif
}
