/* 
 * OBSTACLE AVOIDING CAR
 * SILA KOLELI
 * MAY 2023
*/

#include <Servo.h>

/* Defining pins */
// motor driver pins 
#define MD_1_INPUT_4 4
#define MD_1_INPUT_3 5
#define MD_1_INPUT_2 7
#define MD_1_INPUT_1 8
#define ENABLE_MD1 9

#define MD_2_INPUT_4 13
#define MD_2_INPUT_3 3
#define MD_2_INPUT_2 0
#define MD_2_INPUT_1 1
#define ENABLE_MD2 2

// servo motor pin
#define SERVO 10
// pins for button and dip switch
#define BUTTON 11
#define DIPSWITCH 12
// pins for ultrasonic sensor
#define TRIG A1
#define ECHO A2
// led's pin
#define LED 6

#define FORWARD 0
#define LEFT 1
#define RIGHT 2
#define BACKWARD 3

Servo servo;
byte maxDist = 150; // maximum sensing distance, in cm (further objects are ignored)
byte stopDist = 50; // minimum distance, in cm (car will stop)
float timeOut = 2*(maxDist+10)/100/340*1000000; // max time for return signal

// values to determinate servo's angles.
int forwardAngle = 100;
int leftAngle = 190;
int rightAngle = 10;

int currD; // current distance
int turnDir; // direction to be turned

boolean goesForward = false;

void brake();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
int checkDirection();
int getDistance();

void setup(){
  // setting up motor driver 
  pinMode(ENABLE_MD1, OUTPUT);
  pinMode(MD_1_INPUT_4, OUTPUT);
  pinMode(MD_1_INPUT_3, OUTPUT);
  pinMode(MD_1_INPUT_2, OUTPUT);
  pinMode(MD_1_INPUT_1, OUTPUT);

  pinMode(ENABLE_MD2, OUTPUT);
  pinMode(MD_2_INPUT_4, OUTPUT);
  pinMode(MD_2_INPUT_3, OUTPUT);
  pinMode(MD_2_INPUT_2, OUTPUT);
  pinMode(MD_2_INPUT_1, OUTPUT);

  pinMode(LED, OUTPUT);

  // setting up servo pin
  //pinMode(SERVO, OUTPUT);
  servo.attach(SERVO);

  // setting up buttons
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(DIPSWITCH, INPUT_PULLUP);

  // setting up ultrasonic sensor
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // setting up initial state of motors
  digitalWrite(MD_1_INPUT_1, LOW);
  digitalWrite(MD_1_INPUT_2, LOW);
  digitalWrite(MD_1_INPUT_3, LOW);
  digitalWrite(MD_1_INPUT_4, LOW);
  digitalWrite(ENABLE_MD1, HIGH);
  digitalWrite(MD_2_INPUT_1, LOW);
  digitalWrite(MD_2_INPUT_2, LOW);
  digitalWrite(MD_2_INPUT_3, LOW);
  digitalWrite(MD_2_INPUT_4, LOW);
  digitalWrite(ENABLE_MD2, HIGH);
  servo.write(forwardAngle);

  digitalWrite(LED, LOW);
  Serial.begin(9600);
}

void loop(){
  Serial.println(digitalRead(DIPSWITCH));
  if (digitalRead(DIPSWITCH) == HIGH){ // dip switch aciksa
    digitalWrite(LED, HIGH);
    servo.write(forwardAngle);
    delay(500);
    currD = getDistance();
    if (currD <= stopDist){
      brake();
      moveBackward();
      delay(200);
      if (digitalRead(BUTTON) == HIGH){ // mikro switch basili ise
        brake();
        moveForward();
        delay(100);
      }
      brake();
      turnDir = checkDirection();
      if (turnDir == RIGHT){
        turnRight();
        delay(100);
        brake();
      }
      else if (turnDir == LEFT){
        turnLeft();
        delay(100);
        brake();
      }
    }
    else{
      moveForward();
    }
  }else {
    brake();
    digitalWrite(LED, LOW);
  }
}

void brake(){
  digitalWrite(MD_1_INPUT_1, LOW);
  digitalWrite(MD_1_INPUT_2, LOW);
  digitalWrite(MD_1_INPUT_3, LOW);
  digitalWrite(MD_1_INPUT_4, LOW);
  digitalWrite(MD_2_INPUT_1, LOW);
  digitalWrite(MD_2_INPUT_2, LOW);
  digitalWrite(MD_2_INPUT_3, LOW);
  digitalWrite(MD_2_INPUT_4, LOW);
}

void moveForward(){
  if(!goesForward){
    goesForward = true;
    digitalWrite(MD_1_INPUT_1, HIGH);
    digitalWrite(MD_1_INPUT_2, LOW);
    digitalWrite(MD_1_INPUT_3, HIGH);
    digitalWrite(MD_1_INPUT_4, LOW);
    digitalWrite(MD_2_INPUT_1, HIGH);
    digitalWrite(MD_2_INPUT_2, LOW);
    digitalWrite(MD_2_INPUT_3, HIGH);
    digitalWrite(MD_2_INPUT_4, LOW);
  }
}

void moveBackward(){
  goesForward = false;
  digitalWrite(MD_1_INPUT_1, LOW);
  digitalWrite(MD_1_INPUT_2, HIGH);
  digitalWrite(MD_1_INPUT_3, LOW);
  digitalWrite(MD_1_INPUT_4, HIGH);
  digitalWrite(MD_2_INPUT_1, LOW);
  digitalWrite(MD_2_INPUT_2, HIGH);
  digitalWrite(MD_2_INPUT_3, LOW);
  digitalWrite(MD_2_INPUT_4, HIGH);
}


void turnLeft(){
  digitalWrite(MD_1_INPUT_1, HIGH); 
  digitalWrite(MD_1_INPUT_2, LOW); 
  digitalWrite(MD_1_INPUT_3, LOW); 
  digitalWrite(MD_1_INPUT_4, HIGH);
  digitalWrite(MD_2_INPUT_1, HIGH);
  digitalWrite(MD_2_INPUT_2, LOW);
  digitalWrite(MD_2_INPUT_3, LOW);
  digitalWrite(MD_2_INPUT_4, HIGH);
}

void turnRight(){
  digitalWrite(MD_1_INPUT_1, LOW); 
  digitalWrite(MD_1_INPUT_2, HIGH); 
  digitalWrite(MD_1_INPUT_3, HIGH); 
  digitalWrite(MD_1_INPUT_4, LOW);
  digitalWrite(MD_2_INPUT_1, LOW);
  digitalWrite(MD_2_INPUT_2, HIGH);
  digitalWrite(MD_2_INPUT_3, HIGH);
  digitalWrite(MD_2_INPUT_4, LOW);
}

int checkDirection(){
  int dLeft = 0;
  int dRight = 0;
  int direction = FORWARD;
  servo.write(leftAngle);
  delay(500);
  dLeft = getDistance();
  servo.write(rightAngle);
  delay(500);
  dRight = getDistance();
  if (dLeft >= maxDist && dRight >= maxDist)
    direction = LEFT;
  else if (dLeft <= stopDist && dRight <= stopDist)
    direction = BACKWARD;
  else if (dLeft >= dRight)
    direction = LEFT;
  else if (dRight < dLeft)
    direction = RIGHT;
  return direction;
}

int getDistance(){
  unsigned long pulseTime;
  int d;
  digitalWrite(TRIG, HIGH);
  delay(1);
  digitalWrite(TRIG, LOW);
  pulseTime = pulseIn(ECHO, HIGH, timeOut);
  d = (float)pulseTime * 340 /2 /10000;
  return d;
}

