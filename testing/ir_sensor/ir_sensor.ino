int sensorpin = 0;                 // analog pin used to connect the sharp sensor
double val = 0;                 // variable to store the values from sensor(initially zero)

void setup()
{
  Serial.begin(9600);               // starts the serial monitor
}
 
void loop()
{
  val = analogRead(sensorpin);   // reads the value of the sharp sensor
  val =  0.0001936428309 * val *val + 0.06987226424 * val - 14.32575223; //conversion to mm
  val += 20.98909091;            //offset
  Serial.println(val);           // prints the value of the sensor to the serial monitor
  delay(100);                    //delay so that measurements can be more easily read (visually in monitor)
}
