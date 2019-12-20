/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : main
 * @created     : Friday Nov 22, 2019 16:38:20 MST
 */

#include <Arduino.h>

void setup() {
  Serial.begin(9600);

  pinMode(33, OUTPUT);
  digitalWrite(33, HIGH);
  delay(1000);
  digitalWrite(33, LOW);
}

char buffer[256];

void loop() {
  while(Serial.available()){
    digitalWrite(33, HIGH);
    Serial.readBytes(buffer, 11);
  
    int* a = (int*)&buffer;
    *a++ = 5;
    *a++ = 6;

    char* b = (char*)a;
    *b++ = '\0';

    int size = b - buffer;

    Serial.write(buffer, size);

    delay(100);
  }
  digitalWrite(33, LOW);
  delay(300);
}
