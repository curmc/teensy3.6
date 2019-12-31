#include <Arduino.h>
#include <SPI.h>

int prevSw1 = HIGH;
int incomingByte = 0;
String charsIn = "";
char printout[20];
 
void setup() {

    pinMode(33, OUTPUT);
    Serial.begin(9600);
    digitalWrite(33, HIGH);

}
 
void loop() {

  Serial.println("hi");
  delay(1000);

}
