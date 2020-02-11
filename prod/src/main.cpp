#include <Arduino.h>
#include <Wire.h>
#include "robot.h"

LSM9DS1 imu;
Robot robot(&imu, 100L);

void setup() {
  robot.robot_setup(); 
  Wire.begin();
  if(imu.begin() == false)
  {
    Serial.print("ErrorTeensy");
    while(1);
  }
  imu.calibrate();
}

void loop() {
  robot.robot_loop();
  Serial.println("hi");
}
