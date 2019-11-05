#include <Arduino.h>
#include "i2c_t3.h"

char buff[256];
uint8_t target_drivetrain;
uint8_t target_peripheral;
int count;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);

  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
  Wire.setDefaultTimeout(200000);

  memset(buff, 0, sizeof(buff));
  count = 0;

  Serial.begin(115200);
}

void loop() {
  uint8_t target = 0x66;

  digitalWrite(LED_BUILTIN, HIGH);
  sprintf(buff, "Hello There %d", count++);
  
  Wire.beginTransmission(target);
  Wire.write(buff, strlen(buff));
  Wire.endTransmission();

  if(Wire.getError())
    digitalWrite(33, HIGH);
  else
    digitalWrite(33, LOW);


  digitalWrite(LED_BUILTIN, LOW);


  delay(500);


  target = 0x77;
  digitalWrite(LED_BUILTIN, HIGH);
  sprintf(buff, "Hello There %d", count++);
  

  Wire.beginTransmission(target);
  Wire.write(buff, strlen(buff));
  Wire.endTransmission();

  if(Wire.getError())
    digitalWrite(33, HIGH);
  else
    digitalWrite(33, LOW);

  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
