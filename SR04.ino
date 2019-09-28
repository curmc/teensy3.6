#define trigPin 11
#define echoPin 10

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

/**
 * Prints the distance of the sensor in centimeters.
 * NOTE: the range on the HC-SR04 is between 2cm-4cm.
 */
void printDistance() {
  long duration, distance;
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.01715; // 343*(100/1)*(1/(1*10^6)) / 2
  Serial.print(distance);
  Serial.println(" cm");
}

void loop() {
  printDistance();
  delay(500);
}
