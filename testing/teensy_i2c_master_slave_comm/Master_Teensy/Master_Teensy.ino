#include <Wire.h>

#define button 27 //push-button at pin 27
int buttonState = 0;  

void setup() {
  pinMode(button, INPUT);  
  Wire.begin(1); //Address is at 0x01 
  Serial.begin(115200);
}

int LED_on = 42; 
int LED_off = 41;

void loop() {
  buttonState = digitalRead(button); //reads the state of the button (1 or 0)
  Serial.println(buttonState); //for debugging
  Wire.beginTransmission(2);
  if (buttonState){ 
  Wire.write(LED_on); //Sends "42" to the slave to indicate button is pressed  
  }
  else
  //Wire.beginTransmission(2);
  Wire.send(LED_off); //Sends "41" to the slave to indicate button is not pressed
  Wire.endTransmission();  
}
