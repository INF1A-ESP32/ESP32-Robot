#include <SPI.h>
#include <Wire.h>
// Load the `ESP32 AnalogWrite` by Brian Taylor
#include <analogWrite.h>
// Load the settings and own modules
#include "settings.h"
#include "display.h"

void setup() {
  // Start the serial monitor
  Serial.begin(115200);
  // Register the wheels as output pins
  pinMode(rechtsVoor, OUTPUT);
  pinMode(rechtsAchter, OUTPUT);
  pinMode(linksVoor, OUTPUT);
  pinMode(linksAchter, OUTPUT);
  pinMode(IRlinks, INPUT);
  pinMode(IRrechts, INPUT);
  // Stop the robot
  analogWrite(rechtsVoor, LOW);
  analogWrite(linksVoor, LOW);
  analogWrite(rechtsAchter, LOW);
  analogWrite(linksAchter, LOW);
  // Show groupname on display
  displayBootAnimation();
  delay(3000);
}

void loop() {
  int sRight = analogRead(IRrechts);
  int sLeft = analogRead(IRlinks);
  display.clearDisplay();//clears display
  display.invertDisplay(false);
  display.setTextSize(2);//sets text size
  display.setTextColor(WHITE);
  display.setCursor(2, 6);//sets cursor
  display.print("R: ");
  display.print(sRight);// displays text
  display.setTextSize(2);//sets text size
  display.setCursor(2,26);//sets cursor
  display.print("L: ");
  display.print(sLeft); // displays text
  display.display();
  boolean right = false;
  boolean left = false;
  if(sRight > 3600 && sRight < 3950){
    right = true;
  }
  if(sLeft > 3600 && sLeft < 3950){
    left = true;
  }
  if (right & left){
     analogWrite(rechtsVoor, speedR);
     analogWrite(linksVoor, speedL);
  }else if(right){
     analogWrite(linksVoor, 200);
  }else if(left){
    analogWrite(rechtsVoor, 200);
  }else{
    analogWrite(rechtsVoor, LOW);
    analogWrite(linksVoor, LOW);
    analogWrite(rechtsAchter, LOW);
    analogWrite(linksAchter, LOW);
  }
