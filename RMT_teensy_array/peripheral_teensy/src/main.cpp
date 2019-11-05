
#include <Arduino.h>
#include "i2c_t3.h"

int recieved;
char buff[256];

void recieveEvent(size_t count);
void requestEvent(void);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(33, OUTPUT);
  Wire.begin(I2C_SLAVE, 0x77, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);

  recieved = 0;

  memset(buff, 0, sizeof(buff));

  Wire.onReceive(recieveEvent);
  Wire.onRequest(requestEvent);

  Serial.begin(115200);
}

void loop() {
  if(recieved) {
    digitalWrite(LED_BUILTIN, HIGH);
    recieved = 0;
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void recieveEvent(size_t count) {
  Wire.read(buff, count);
  recieved = count;
}

void requestEvent(void) {
  Wire.write(buff, 256);
}
