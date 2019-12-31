#include <Arduino.h>
#include <Wire.h>
#include "imu.h"


// All the information 
// is contained in this 
// one module
LSM9DS1 imu;
uint64_t serial_timer;
SpeedDiagnostics speed(imu);


void setup() {
  Serial.begin(9600);
  serial_timer = millis();
  speed.setup();

  Wire.begin();

  if(imu.begin() == false) {
    while(1) {
      Serial.print("Error\n\r");
      delay(1000);
    }
  }
  imu.calibrate();
}



void loop() {
  speed.loop();
  delay(1000);
}

