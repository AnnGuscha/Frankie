//prototype
int writeServo (int  );
double read_gp2d12_range(double value);//right
double read_gp2y0a_range(double value);//center
int haveObstacle(int ledPinInfra, int InputPin);
void robotInit();
void motorInit();
void forward(int value);
void neutral();
void back(int value);
void move(int value);
int getSpeed();
int getServo();
void getSensors(int arrayone[]);
boolean range (int value, int rangeMin,int rangeMax );
extern volatile unsigned long wheelCount;