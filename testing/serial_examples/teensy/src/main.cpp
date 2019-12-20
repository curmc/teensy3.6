#include <Arduino.h>
#include <SPI.h>

int prevSw1 = HIGH;
int incomingByte = 0;
String charsIn = "";
char printout[20];  //max char to print: 20
 
void setup() {
  
    //Setup Serial Port with baud rate of 9600
    Serial.begin(9600);
    
}
 
void loop() {
  Serial.println("Hello from Arduino");
    while (Serial.available()) {
      char charRead = Serial.read();
      charsIn.concat(charRead);
    }
  if(charsIn != ""){
    Serial.println("How are you, " + charsIn);
    charsIn.toCharArray(printout, 21);
    charsIn = "";
  }
}
