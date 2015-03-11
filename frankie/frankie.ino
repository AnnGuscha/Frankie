int dirL;
int dirR;
int spL;
int spR;
char inputBuffer[4];

int backDelay = 300;
int backTurnDelay = 500;
int turnDelay =200;

double diff = 40;
double dist = 50;

double mini = 20;
double maxi = 60;

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

//возможные действия
enum Action
{
  _forward,  //движение вперёд
  _back,       //движение назад
  _backL,     //движение назад, после которого последует поворот влево задним ходом
  _backR,     //движение назад, после которого последует поворот вправо задним ходом
  
  _backTurnLeft,    //поворот влево задним ходом
  _backTurnRight,  //поворот вправо задним ходом
  _forwardLeft,       //поворот влево одной осью
  _forwardRight     //поворот вправо одной осью
};

// определяем порты, к которым подключены моторчики
MOTOR MOTORL = { 10, 11 };
MOTOR MOTORR = { 12, 13 };

BUMPER bumperPin = {4,3,2};
BUMPER bumper = {0,0,0};
SENSOR sens = {.0,.0,.0};

const int buttonPin = 2;

unsigned long timeEndAction;
Action currentAction = _forward;

double sum, distsArr[7];
int i, number = 7;

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
  
  forwardL(100);
  forwardR(100);
}

void loop()
{
  bumper.l = digitalRead(bumperPin.l);
  bumper.c = digitalRead(bumperPin.c);
  bumper.r = digitalRead(bumperPin.r);
  
  switch(currentAction) {
      case _forward:
          Serial.println("_forward");
          if (bumper.c == HIGH || bumper.l == HIGH && bumper.r == HIGH)
              currentAction = _back;
          else if (bumper.l == HIGH)
              currentAction = _backR;
          else if (bumper.r == HIGH)
              currentAction = _backL;
          
          if (currentAction != _forward) {
              timeEndAction = millis() + backDelay;
              backL(100);
              backR(100);
          }
          else {
              //получаем расстояние с центрального датчика
              sum = 0;
              for(i=0; i<number; ++i) {
                  distsArr[i] = read_gp2y0a_range(analogRead(A0));
                  sum += distsArr[i];
              }
              sens.c = (sum - distsArr[getNumMax_dist()] - distsArr[getNumMin_dist()]) / (number-2);
              
              //получаем расстояние с левого датчика
              sum = 0;
              for(i=0; i<number; ++i) {
                  distsArr[i] = read_gp2y0a_range(analogRead(A1));
                  sum += distsArr[i];
              }
              sens.l = (sum - distsArr[getNumMax_dist()] - distsArr[getNumMin_dist()]) / (number-2);
              
              Serial.println(sens.l);
              Serial.println(sens.c);
              Serial.println();
              
              if (sens.c * 1.5 > sens.l || sens.c < mini ) {
                  currentAction = _forwardRight;
                  timeEndAction = millis() + turnDelay;
                  releaseR();
                  forwardL(100);
              }
              //if (sens.c  * 3.5 < sens.l || sens.c > maxi) {
                if ( sens.c > maxi) {
                  currentAction = _forwardLeft;
                  timeEndAction = millis() + turnDelay;
                  releaseL();
                  forwardR(100);
              }
          }
          break;
      
      case _backL:
          Serial.println("_backL");
          if (millis() > timeEndAction) {
              currentAction = _backTurnLeft;
              timeEndAction = millis() + backTurnDelay;
              forwardR(100);
              //releaseR();
              backL(100);
          }
          break;
      
      case _backR:
          Serial.println("_backR");
          if (millis() > timeEndAction) {
              currentAction = _backTurnRight;
              timeEndAction = millis() + backTurnDelay;
              forwardL(100);
             // releaseL();
              backR(100);
          }
          break;
      
      case _backTurnLeft:
      case _backTurnRight:
          Serial.println("_backTurn");
          if (millis() > timeEndAction) {
              currentAction = _forward;
              forwardL(100);
              forwardR(100);
          }
          break;
      
      case _back:
          Serial.println("_back");
          if (millis() > timeEndAction) {
              currentAction = _forward;
              forwardL(100);
              forwardR(100);
          }
      
      case _forwardRight:
          Serial.println("_forwardRight");
          if (bumper.c == HIGH || bumper.l == HIGH && bumper.r == HIGH)
              currentAction = _back;
          else if (bumper.l == HIGH)
              currentAction = _backR;
          else if (bumper.r == HIGH)
              currentAction = _backL;
          
          if (currentAction != _forwardRight) {
              timeEndAction = millis() + backDelay;
              backL(100);
              backR(100);
          }
          else {
              if (millis() > timeEndAction) {
                  currentAction = _forward;
                  forwardL(100);
                  forwardR(100);
              }
          }
          break;
      
      case _forwardLeft:
          Serial.println("_forwardLeft");
          if (bumper.c == HIGH || bumper.l == HIGH && bumper.r == HIGH)
              currentAction = _back;
          else if (bumper.l == HIGH)
              currentAction = _backR;
          else if (bumper.r == HIGH)
              currentAction = _backL;
          
          if (currentAction != _forwardLeft) {
              timeEndAction = millis() + backDelay;
              backL(100);
              backR(100);
          }
          else {
              if (millis() > timeEndAction) {
                  currentAction = _forward;
                  forwardL(100);
                  forwardR(100);
              }
          }
  }
  
  /*
  sum = 0;
  for(i=0; i<number; ++i) {
      distsArr[i] = read_gp2y0a_range(analogRead(A1));
      sum += distsArr[i];
  }
  sens.l = (sum - distsArr[getNumMax_dist()] - distsArr[getNumMin_dist()]) / (number-2);
  
  sum = 0;
  for(i=0; i<number; ++i) {
      distsArr[i] = read_gp2y0a_range(analogRead(A0));
      sum += distsArr[i];
  }
  sens.c = (sum - distsArr[getNumMax_dist()] - distsArr[getNumMin_dist()]) / (number-2);
  
  sum = 0;
  for(i=0; i<number; ++i) {
      distsArr[i] = read_gp2y0a_range(analogRead(A2));
      sum += distsArr[i];
  }
  sens.r = (sum - distsArr[getNumMax_dist()] - distsArr[getNumMin_dist()]) / (number-2);

  */
  //Serial.println(sens.l);
  //Serial.println(sens.c);
  //Serial.println(sens.r);
  //Serial.println();
  //delay(100);
  
/*
  if (sens.l - sens.r > diff) {
      Serial.println("Left");
      forwardL(100);
      releaseR();  
      delay(turnDelay);
      forwardL(100); 
      forwardR(100);
      
  }else {
      if(sens.r - sens.l > diff) {
          Serial.println("Right");
          forwardR(1000);
          releaseL();  
          delay(turnDelay);
          forwardL(100); 
          forwardR(100);
      
      }else{
          Serial.println("Center");
          forwardL(100);
          forwardR(100);
      }
  }
  delay(turnDelay);
  */
  // delay(500);
  // forwardL(100);
  // forwardR(100);
}

int getNumMax_dist()
{
    int num = 0;
    for(int i=1; i<number; ++i)
        if (distsArr[num] < distsArr[i])
            num = i;
    return num;
}

int getNumMin_dist()
{
    int num = 0;
    for(int i=1; i<number; ++i)
        if (distsArr[num] > distsArr[i])
            num = i;
    return num;
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

