// includes
#include "RGBdriver.h"
#include "TaskScheduler.h"
#include "pt.h"

// define LED Strip ports
#define CLK 2 // LED Strip Driver CLK pin        
#define DIO 3 // LED Strip Driver DIO pin

// define Analog input ports
#define RANGE_PIN A1 // set the input pin for the range sensor 
#define SOUND_PIN A3 // set the input pin for the sound sensor

// define digital input ports 
#define BODY_PIN 5 //
#define TEST_PIN 13 // set the test LED pin

// define digital output ports
#define SPOTLIGHT_PIN 6 

// create classes
// RGBDrvier class creation
RGBdriver LEDStrip(CLK,DIO);

// declare variables
int nRangeValue = 0;    // variable to store the value coming from the range sensor
int nSoundValue = 0;    // variable to store the value coming from the sound sensor

int n = 3;
int count = 0;
int bodystate = 0;

const int FilterParam = 10; // filter param for sensors signal


// initialization
void setup() {
  // declare the IO pins:
  pinMode(TEST_PIN, OUTPUT);
  pinMode(BODY_PIN, INPUT);
  
  Serial.begin(9600);       // use the serial port
  
  Sch.init();                // Initialize task scheduler
  Sch.addTask(Task_ChangeColor,0,500,1);  //从第 0 毫秒开始闪烁 LED，每隔 1s, LED 状态改变一次
  Sch.addTask(Task_ChangeColor2,0,500,1);  //从第 20 毫秒开始闪烁 LED，每隔 0.5s, LED 状态改变一次
  Sch.start();  // Start the task scheduler
  
  LEDStrip.begin(); 
  LEDStrip.SetColor(255, 255, 255); 
  LEDStrip.end();
}

// main loop
void loop() {
  int color;
  // read the values from the sensors:
  nRangeValue = (analogRead(RANGE_PIN) * 5 + nRangeValue * (FilterParam - 1)) / FilterParam;
  nSoundValue = (analogRead(SOUND_PIN) * 5 + nRangeValue * (FilterParam - 1)) / FilterParam;
  
    // check if data has been sent from the computer:
  if (Serial.available() > 0) {
    // set debug LED open
    digitalWrite(TEST_PIN, HIGH);
    // look for the next valid integer in the incoming serial stream:
    int red = Serial.parseInt();
    // do it again:
    int green = Serial.parseInt();
    // do it again:
    int blue = Serial.parseInt();
    // set the brightness of the LED:
        // look for the newline. That's the end of your
    // sentence:
    if (Serial.read() == '\n') {
      // constrain the values to 0 - 255 and invert
      // if you're using a common-cathode LED, just use "constrain(color, 0, 255);"
      red = constrain(red, 0, 255);
      green = constrain(green, 0, 255);
      blue = constrain(blue, 0, 255);
      
      // set the color
      LEDStrip.begin(); // begin
      LEDStrip.SetColor(red, green, blue); 
      LEDStrip.end();
      
      // print the three numbers in one string as hexadecimal:
      Serial.print("Set OK! (");
      Serial.print(red, HEX);
      Serial.print(green, HEX);
      Serial.print(blue, HEX);
      Serial.print(") (");
      Serial.print(red);
      Serial.print(", ");
      Serial.print(green);
      Serial.print(", ");
      Serial.print(blue);
      Serial.println(")");
    }
  }  
  
  /* 根据状态机改变状态
  case 
  //*/
  count += 1;
  delay(100);  
        // set the color
  color = constrain(GetRange(nRangeValue)-80,0,255);/*
  LEDStrip.begin(); // begin
    bodystate = digitalRead(BODY_PIN);
    if (bodystate = HIGH) {
      LEDStrip.SetColor(255, color/4, 0); 
    }
    else {
      LEDStrip.SetColor(0, 0, 0); 
    }
  LEDStrip.end();*/
  if (count > 5 ) { 
    count = 0;
    Serial.print("RangeValue: :");
    Serial.print(nRangeValue, DEC);
    Serial.print(" Distance :");
    Serial.println(GetRange(nRangeValue), DEC);
  }
}

int a = 0;
// get the distance(cm) from the range sensor(mV).
int GetRange(int rangeValue) {
  int var;
  var = constrain(rangeValue, 1300, 3600);
  var = var - 1190;
  var = 100800 / var;
  var = var + 20;
  if (var < 70) return -1;
  return var;  
}

void Task_ChangeColor(){
  Serial.println("task1 run");
}

void Task_ChangeColor2(){
  Serial.println("task2 run");
}
