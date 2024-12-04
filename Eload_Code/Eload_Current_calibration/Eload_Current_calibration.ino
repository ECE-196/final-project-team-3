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

  // output pins
  pinMode(12, OUTPUT);  // fuse blown
  pinMode(11, OUTPUT);  // over temp
  pinMode(10, OUTPUT);  // over power
  pinMode(9, OUTPUT);   // over current
  pinMode(8, OUTPUT);   // over voltage
  pinMode(2, OUTPUT);   // turn off load

  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.setTextSize(1);            
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
}

void loop() {

//CURRENT MEASUREMENT
  int numReadings = 500; 
  long Currentsum = 0;            
  for (int i = 0; i < numReadings; i++) {
    Currentsum += analogRead(0);
    delay(3);
  }
  float current = (Currentsum / (float)numReadings);
  

  //DISPLAY
display.clearDisplay();


  display.setCursor(0, 22);        
  display.print(("Current: "));
  display.print(current);
 

  display.display();
}