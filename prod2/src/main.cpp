#include <Arduino.h>
#include <Wire.h>
#include "robot.h"

LSM9DS1 imu;
Robot robot(&imu, 100L);

void setup() {
  Serial.begin(9600);
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);

  Wire.begin();
  Serial.println("Here");

  robot.robot_setup();
  if(imu.begin() == false)
  {
    while(1){
      Serial.println("ErrorTeensy");
    }
  }

  robot.calibrate_sensors();
  digitalWrite(33, HIGH);
}

void loop() {
  robot.update_sensors();
  robot.serial_listen();
}
