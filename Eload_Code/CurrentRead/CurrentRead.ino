void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);
}

void loop() {
  const int numReadings = 100; 
  long sum = 0;            
  for (int i = 0; i < numReadings; i++) {
    int analogValue = analogRead(A2);
    sum += analogValue;
    delay(1);
  }

  float averageValue = sum / (float)numReadings;
  float current = 0.288*averageValue+0.465;
}
