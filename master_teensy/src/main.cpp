#include <Wire.h>

#define LED 12



void setup() {
  pinMode(LED, OUTPUT);
  Wire.setSDA(18);
  Wire.setSCL(19);
  Wire.begin();        // join i2c bus (address optional for master)


  Serial.begin(9600);  // start serial for output
  digitalWrite(LED, LOW);
}

void loop() {
  digitalWrite(LED, HIGH);
  Wire.beginTransmission(8);
  Wire.write(0);
  Wire.endTransmission();
  digitalWrite(LED, LOW);


  delay(500);
}
