#include <Wire.h>

#define led 13 //on-board LED
int state;
void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led,LOW);
  Wire.begin(2); //Address is at 0x02    
  Wire.onReceive(receiveEvent); //If annything is received from the master, read the bits
}


void loop() { 
Serial.println(state);  // print the integer received
if (state == 42) digitalWrite(led,HIGH); //If "42" was received by the slave, 
if (state == 41) digitalWrite(led,LOW); //If "41" was received by the slave
delay(100);
}

void receiveEvent(int howMany)
{
  state = Wire.read(); //read the bits sent until next event 
}
