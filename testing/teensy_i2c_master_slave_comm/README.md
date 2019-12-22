# Teensy I2C Master/Slave
Testing I2C Master and slave with two Teensys. When button is pressed at the master, it will send a state, which the slave will interpret either to turn its onboard led on or off.

Pin configuration:

Teensy 3.6 Master:
-Push button at pin 27

-SCL at pin 19

-SDA at pin 18



Teensy 3.6 Slave:
-On-board LED at pin 13

-SCL at pin 19

-SDA at pin 18


Don't forget:

-4.7k pull up resistors

-Connect to same ground

-Use Arduino push button example for setting up 

  -10k resistor
  
