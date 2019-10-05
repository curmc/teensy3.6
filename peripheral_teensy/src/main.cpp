#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(80);
  Wire.write(0);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(80, 1);
  while(Wire.read()) {
    
  }

  Serial.write("hell\n");
  delay(1000);
}
