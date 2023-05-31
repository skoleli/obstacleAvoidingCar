/* 
 * OBSTACLE AVOIDING CAR
 * SILA KOLELI
 * MAY 2023
*/

#include <Servo.h>

/* Defining pins */
// motor driver pins 
#define ENABLE_3_4 3
#define INPUT_4 4
#define INPUT_3 5
#define INPUT_2 7
#define INPUT_1 8
#define ENABLE_1_2 9
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

void setup(){
  // setting up motor driver 
  pinMode(ENABLE_3_4, OUTPUT);
  pinMode(INPUT_4, OUTPUT);
  pinMode(INPUT_3, OUTPUT);
  pinMode(INPUT_2, OUTPUT);
  pinMode(INPUT_1, OUTPUT);
  pinMode(ENABLE_1_2, OUTPUT);
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
  digitalWrite(INPUT_1, LOW);
  digitalWrite(INPUT_2, LOW);
  digitalWrite(INPUT_3, LOW);
  digitalWrite(INPUT_4, LOW);
  digitalWrite(ENABLE_1_2, HIGH);
  digitalWrite(ENABLE_3_4, HIGH);
  servo.write(forwardAngle);

  digitalWrite(LED, LOW);
}

void loop(){
  
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
  digitalWrite(INPUT_1, LOW);
  digitalWrite(INPUT_2, LOW);
  digitalWrite(INPUT_3, LOW);
  digitalWrite(INPUT_4, LOW);
}

void moveForward(){
  if(!goesForward){
    goesForward = true;
    digitalWrite(INPUT_1, HIGH);
    digitalWrite(INPUT_2, LOW);
    digitalWrite(INPUT_3, HIGH);
    digitalWrite(INPUT_4, LOW);
  }
}

void moveBackward(){
  goesForward = false;
  digitalWrite(INPUT_1, LOW);
  digitalWrite(INPUT_2, HIGH);
  digitalWrite(INPUT_3, LOW);
  digitalWrite(INPUT_4, HIGH);
}

void turnLeft(){
  digitalWrite(INPUT_1, HIGH); 
  digitalWrite(INPUT_2, LOW); 
  digitalWrite(INPUT_3, LOW); 
  digitalWrite(INPUT_4, HIGH);
}

void turnRight(){
  digitalWrite(INPUT_1, LOW); 
  digitalWrite(INPUT_2, HIGH); 
  digitalWrite(INPUT_3, HIGH); 
  digitalWrite(INPUT_4, LOW);
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

