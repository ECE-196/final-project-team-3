#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // input pins
  pinMode(3, INPUT);   // button for clear fault
  pinMode(5, INPUT);   // vdrive sense
  pinMode(A0, INPUT);  // current sense
  pinMode(A1, INPUT);  // temp sense
  pinMode(A2, INPUT);  // voltage sense
  pinMode(A3, INPUT);  // set volt limit
  pinMode(A6, INPUT);  // set current limit
  pinMode(A7, INPUT);  // set temp limit
  pinMode(13, INPUT);   // Detect faults

  // output pins
  pinMode(12, OUTPUT);  // fuse blown
  pinMode(11, OUTPUT);  // over temp
  pinMode(10, OUTPUT);  // over power
  pinMode(9, OUTPUT);   // over current
  pinMode(8, OUTPUT);   // over voltage
  pinMode(2, OUTPUT);   // turn off load
  pinMode(6, OUTPUT);   // OLED Reset
  delay(100);

  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  display.setTextColor(SSD1306_WHITE);

  delay (100);
  digitalWrite(6, LOW);
  delay(10);
  digitalWrite(6, HIGH);
  delay(100);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  display.setTextSize(1);  

  display.clearDisplay();
  display.setCursor(41, 20);            
  display.print("CAPUCHIN");
  display.setCursor(30, 40);    
  display.print("TECHNOLOGIES");
  display.display();
  delay(1500);
  display.clearDisplay();

  display.setCursor(34, 20);            
  display.print("ELECTRONIC");
  display.setCursor(53, 40);    
  display.print("LOAD");
  display.display();
  delay(1500);
  display.clearDisplay();
}


void loop() {

//CURRENT MEASUREMENT
  int numReadings = 300; 
  long Currentsum = 0;            
  for (int i = 0; i < numReadings; i++) {
    Currentsum += analogRead(0);
    delay(1);
  }
  float current = (0.14642*(Currentsum / (float)numReadings)+2.3);
  if (current < 2.35) {
    current = 0;
  }

// TEMPERATURE MEASUREMENT
  int tempSamples = 50; 
  long tempsum = 0; 
  for (int i = 0; i < tempSamples; i++) {
    tempsum += (analogRead(1)); 
    delay(1); 
  }
  float temp = 0.1955*((tempsum) / (float)tempSamples);

//VOLTAGE MEASUREMENT
  int numSamples = 200; 
  long voltsum = 0; 
  for (int i = 0; i < numSamples; i++) {
    voltsum += (analogRead(A2)); 
    delay(1); 
  }
  float voltage = 0.1962*((voltsum) / (float)numSamples)+0.31;
  if (voltage < 0.35) {
    voltage = 0;
  }

//POWER CALCULATION
  int power = current*voltage;

//SET LIMITS
  int temp_lim = map(analogRead(7), 0, 1023, 10, 100); 
  int current_lim = map(analogRead(6), 0, 1023, 0, 150); 
  int voltage_lim = map(analogRead(3), 0, 1023, 0, 100);
  int power_lim = 3500;

  if (temp > temp_lim) {
    digitalWrite(11, HIGH);
  } 
  if (current > current_lim) {
    digitalWrite(9, HIGH);
  }   
  if (voltage > voltage_lim) {
    digitalWrite(8, HIGH);
  } 
  if (digitalRead(5) == HIGH && analogRead(A0)) {
    digitalWrite(12, HIGH);
  } 
  if (power > power_lim) {
    digitalWrite(10, HIGH);
  }

//CHECK FOR FAULTS  
  if (digitalRead(13) == HIGH) {
    digitalWrite(2, HIGH);
  }

//CHECK CLEAR FAULT
  int D3val = digitalRead(3);
  if (D3val) {
    //clear all faults
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    //ENABLE LOAD
    digitalWrite(2, LOW);
  }

  //DISPLAY
  display.clearDisplay();
  display.setCursor(0, 7);            
  display.print(("Voltage: "));
  display.print(voltage);
  display.setCursor(85, 7); 
  display.print(("/ "));
  display.print(voltage_lim);
  display.print((" V")); 
  display.setCursor(0, 22);        
  display.print(("Current: "));
  display.print(current);
  display.setCursor(85, 22); 
  display.print(("/ "));
  display.print(current_lim);
  display.print((" A")); 
  display.setCursor(0, 37);          
  display.print(("Power:   "));
  display.print(power);
  display.setCursor(85, 37); 
  display.print(("/ "));
  display.print("2.5k");
  display.print(("W")); 
  display.setCursor(0, 52);          
  display.print(("Temp:    "));
  display.print(temp);
  display.setCursor(85, 52); 
  display.print(("/ "));
  display.print(temp_lim);
  display.print((" C"));
  display.display();
}
