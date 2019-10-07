#include <Arduino.h>
#include <Wire.h>

#define TRIGGER     2
#define ECHO        3
#define LED         4

// long trigg_time;
// int distance;
//
// void receiveEvent();
//
// void setup() {
//   Wire.begin(8); // Address is 8 on the bus
//   Wire.onRequest(receiveEvent); //register callback
//
//   pinMode(TRIGGER, OUTPUT); // Sets the TRIGGER as an Output
//   pinMode(ECHO, INPUT);  // Sets the ECHO as an Input
//   pinMode(LED, OUTPUT);
// }
//
//
// void loop() {
//   digitalWrite(TRIGGER, LOW);
//   delayMicroseconds(2);
//
//   digitalWrite(TRIGGER, HIGH);
//   delayMicroseconds(10);
//
//   digitalWrite(TRIGGER, LOW);
//   trigg_time = pulseIn(ECHO, HIGH);
//   distance = trigg_time * 0.034 / 2; // centimeters
// [>   if(distance < 50) <]
//   //   digitalWrite(LED, HIGH);
//   // else
//     // [> digitalWrite(LED, LOW <]);
// }
//
// void receiveEvent() {
//   digitalWrite(LED, HIGH);
//   Wire.write("her", 4); // 4 bytes
//   delay(10);
//   digitalWrite(LED, LOW);
// }
void recieveEvent(int);
void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  Wire.setSDA(18);
  Wire.setSCL(19);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(recieveEvent); // register event
}

void loop() {
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void recieveEvent(int i) {
  digitalWrite(LED, LOW);
}
