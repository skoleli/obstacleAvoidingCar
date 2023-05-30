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

Servo servo;
byte maxDist = 150; // maximum sensing distance, in cm (further objects are ignored)
byte stopDist = 50; // minimum distance, in cm (car will stop)
float timeOut = 2*(maxDist+10)/100/340*1000000; // max time for return signal

void setup(){
  // setting up motor driver 
  pinMode(ENABLE_3_4, OUTPUT);
  pinMode(INPUT_4, OUTPUT);
  pinMode(INPUT_3, OUTPUT);
  pinMode(INPUT_2, OUTPUT);
  pinMode(INPUT_1, OUTPUT);
  pinMode(ENABLE_1_2, OUTPUT);

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
  servo.write(10);
}

void loop(){

}