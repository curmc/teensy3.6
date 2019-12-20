#include "Arduino.h"

#define PIN 23
#define LED 22
#define PWM_SCALAR 5.12
#define PWM_ZERO   1535

#define MOTOR_SCALAR 1.7534
#define MOTOR_SHIFT (-8.3535);

#define PWM_RES    12
#define PWM_INIT   250
#define MAX        100

int convert(int8_t throttle_) {
  throttle_ = MOTOR_SCALAR * throttle_ + MOTOR_SHIFT;

  int throttle;
  if(throttle_ < -MAX)
    throttle = -MAX;
  else if(throttle_ > MAX)
    throttle = MAX;
  else
    throttle = throttle_;
  return PWM_ZERO + PWM_SCALAR * throttle;
}

int vel = 30;
long counter = 0;
int led_state = HIGH;

void setup() {
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  
  // Used to show that an input was given
  pinMode(LED, OUTPUT);
  analogWriteFrequency(PIN, PWM_INIT);

  digitalWrite(LED, HIGH);

  analogWriteResolution(PWM_RES);
}



void loop() {
  // counter++;
  // // 10 seconds at 16MHZ
  // if(counter > 2000000){
  //   counter = 0;
  //   vel++;
  //   if(led_state == HIGH){
  //     digitalWrite(LED, LOW);
  //     led_state = LOW;
  //   }
  //   else{
  //     digitalWrite(LED, HIGH);
  //     led_state = HIGH;
  //   }
  // }
  analogWrite(PIN, convert(vel));
}

