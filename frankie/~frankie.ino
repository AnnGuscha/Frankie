int dirL;
int dirR;
int spL;
int spR;
char inputBuffer[4];

int backDelay = 400;
int backTurnDelay = 600;
int turnDelay =200;

double diff = 40;
double distC = 50;

struct MOTOR    // структура для хранения номеров pin-ов, к которым подключены моторчики
{
  int in1;      // INPUT1
  int in2;      // INPUT2
};

struct BUMPER    // структура для хранения номеров pin-ов, к которым подключены моторчики
{
  int l;      
  int c;   
  int r;  
};

struct SENSOR
{
  double l;
  double c;
  double r;
};
// определяем порты, к которым подключены моторчики
MOTOR MOTORL = { 10, 11 };
MOTOR MOTORR = { 12, 13 };

BUMPER bumperPin = {4,3,2};
BUMPER bumper = {0,0,0};
SENSOR sens = {.0,.0,.0};

const int buttonPin = 2;

void setup()
{
  Serial.begin(9600);     
  pinMode(MOTORL.in1, OUTPUT); // настраиваем выводы
  pinMode(MOTORL.in2, OUTPUT); // на ВЫВОД
  pinMode(MOTORR.in1, OUTPUT);
  pinMode(MOTORR.in2, OUTPUT);

  pinMode(bumper.l, INPUT);
  pinMode(bumper.c, INPUT);
  pinMode(bumper.r, INPUT);
  digitalWrite(bumper.l,LOW);
  digitalWrite(bumper.c,LOW);
  digitalWrite(bumper.r,LOW);
  releaseL();
  releaseR();
}

void loop()
{
   bumper.l = digitalRead(bumperPin.l);
   bumper.c = digitalRead(bumperPin.c);
   bumper.r = digitalRead(bumperPin.r);
   Serial.println(bumper.l);
   Serial.println(bumper.c);
   Serial.println(bumper.r);
   Serial.println();
   

    if (bumper.l == HIGH) {
      backL(100);
      backR(100);
      delay(backDelay);
      releaseL();
      backR(100);
      delay(backTurnDelay);
      releaseL();
      releaseR();
    }
  if (bumper.c == HIGH) {
    backL(100);
    backR(100);
    delay(backDelay);
    releaseL();
    releaseR();
  }
  if (bumper.r == HIGH) {
    backL(100);
    backR(100);
    delay(backDelay);
    releaseR();
    backL(100);
    delay(backTurnDelay);
    releaseL();
    releaseR();
  }
  
  sens.l = read_gp2y0a_range(analogRead(A1));
  sens.c = read_gp2y0a_range(analogRead(A0));
  sens.r = read_gp2y0a_range(analogRead(A2));
  
   //Serial.println(sens.l);
   //Serial.println(sens.c);
   //Serial.println(sens.r);
   //Serial.println();
   // delay(300);
    
  if(sens.l-sens.r > diff) {
     //   Serial.println("Left");
     
        forwardL(100);
        releaseR();  
        delay(turnDelay);
        forwardL(100); 
        forwardR(100);
      
  }else{
  if(sens.l-sens.r>diff){
  //  Serial.println("Right");  
  
        forwardR(1000);                                            
        releaseL();  
        delay(turnDelay);
        forwardL(100); 
        forwardR(100);
      
  }else{
   // Serial.println("Center");  
        forwardL(100);   
        forwardR(100);
        
  }
  }
  delay(turnDelay);
//   Serial.println(bumper.l);
//   Serial.println(bumper.c);
//   Serial.println(bumper.r);
//   Serial.println();
  // delay(500);
  // forwardL(100);   
  // forwardR(100);
}


double read_gp2y0a_range(double value)
{
  double val;
  val = (8762.88/(value/2 + 18.32)-12.06);
  if (val < 0) val = 150;
  if (val >150) val = 150;
  return val; 
} 

void forwardL(int pwm) // первый вперёд
{
  digitalWrite(MOTORL.in1, LOW);
  digitalWrite(MOTORL.in2, HIGH);
}

void forwardR(int pwm) // второй вперёд
{  
  digitalWrite(MOTORR.in1, LOW);
  digitalWrite(MOTORR.in2, HIGH);
}

void backL(int pwm) // первый назад
{
  digitalWrite(MOTORL.in1, HIGH);
  digitalWrite(MOTORL.in2, LOW);
  
}

void backR(int pwm) // второй назад
{
  digitalWrite(MOTORR.in1, HIGH);
  digitalWrite(MOTORR.in2, LOW);

}

void releaseL() // первый вперёд
{
  digitalWrite(MOTORL.in1, LOW);
  digitalWrite(MOTORL.in2, LOW);

}
void releaseR() // первый вперёд
{
  digitalWrite(MOTORR.in1, LOW);
  digitalWrite(MOTORR.in2, LOW);

}

