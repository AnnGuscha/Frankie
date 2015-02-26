//servo (rotate frontal wheels) 
#define SERVO_PIN 6
#define MAX_MS_SERVOTIK 1000
#define SERVO_OFFSET 87
#define SERVO_MAX_ANGLE 80 
#define STCALC(a) (SERVO_OFFSET+(a)) 

// motor (back wheels are powered)
#define MOTOR_EN 5
#define MOTOR_DIR 4

#define FORWARD 1
#define STOP 0
#define BACK -1

//rotate_sensor
#define ROTATE_PIN 2

// pinien 15
// spur 68
// wheel diametr 59 mm
#define MIN_INT_TIC 70 // for normal interrupt work if the sped is big put this to 70
#define SREDUKTOR  32*1000000  ////10.21*1000000 //distanse mm
#define MINSPEED 100 // mm / s
#define MAXSPEED 4000 // mm / s

// button
#define BUTTON 12
 
// led
#define LED 13

// distanse (analog sensors)
#define RIGHT 3
#define CENTER 0
#define LEFT 4
#define BRIGHT 2
#define BLEFT 1

//Servo analog output
#define SERVOANALOGPIN 7
#define SERVOMINVOLTAGE 47
#define SERVOMAXVOLTAGE 574




// serial
#define SERIAL_SRPEED 57600

