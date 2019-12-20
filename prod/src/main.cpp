#include "Arduino.h"

#define PWM_SCALAR    5.12
#define PWM_ZERO      1535
#define PWM_RES       12
#define PWM_INIT      250

int8_t MAX_THROTTLE = 100;

#define LF_IND        0
#define LB_IND        1
#define RF_IND        2
#define RB_IND        3

#define LF_PIN        16
#define LB_PIN        5
#define RF_PIN        4
#define RB_PIN        7

#define MSG_SIZE      11



int motors[4];
int8_t throttle[4];
uint8_t buffer[11];

int8_t lin;
int8_t ang;

int convert(int8_t throttle_);
int serial_listen(uint8_t message[MSG_SIZE], int8_t* lin, int8_t* ang);
int callback_velocity(int8_t throttle[4], int8_t lin, int8_t ang);
int motor_write(int motors[4], int8_t throttle[4]);


void setup() {
  lin = 0;
  ang = 0;


  motors[LF_IND] = LF_PIN;
  motors[LB_IND] = LB_PIN;
  motors[RB_IND] = RB_PIN;
  motors[RF_IND] = RF_PIN;

  memset(throttle, 0, 4);


  for(int i = 0; i < 4; ++i) {
    analogWriteFrequency(motors[i], PWM_INIT);
    analogWrite(motors[i], PWM_ZERO);
  }

  analogWriteResolution(PWM_RES);
  Serial.begin(9600);

  pinMode(33, OUTPUT);
  digitalWrite(33, HIGH);
  delay(1000);
  digitalWrite(33, LOW);
}



void loop() {
  serial_listen(buffer, &lin, &ang);
  callback_velocity(throttle, lin, ang);
  motor_write(motors, throttle);

  delay(100);
}

int convert(int8_t throttle_) {
  if(throttle_ > MAX_THROTTLE)
    throttle_ = MAX_THROTTLE;
  else if(throttle_ < -MAX_THROTTLE)
    throttle_ = -MAX_THROTTLE;

  return PWM_ZERO + (throttle_ * PWM_SCALAR);
}


int serial_listen(uint8_t message[MSG_SIZE], int8_t* lin_, int8_t* ang_) {
  if(Serial.available()) {
    digitalWrite(33, HIGH);
    Serial.readBytes((char*)message, 11);

    *lin_ = *(int*)(&message[1]);
    *ang_ = *(int*)(&message[5]);

    int buffer_temp[2];
    buffer_temp[0] = *lin_;
    buffer_temp[1] = *ang_;
    Serial.write((char*)buffer_temp, 8);

    // Serial.write(message, 11);
    

  } else {
    *lin_ = 0;
    *ang_ = 0;
  }
  return 1;
}


int callback_velocity(int8_t throttle[4], int8_t lin, int8_t ang) {
  throttle[LF_IND] = -(lin + ang);
  throttle[LB_IND] = -(lin + ang);
  throttle[RF_IND] = (lin - ang);
  throttle[RB_IND] = (lin - ang);
  return 1;
}

int motor_write(int motors[4], int8_t throttle[4]) {
  for(int i = 0; i < 4; ++i) {
    analogWrite(motors[i], convert(throttle[i]));
  }
  return 1;
}

