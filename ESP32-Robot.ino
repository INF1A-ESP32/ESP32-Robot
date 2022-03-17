#include <SPI.h>
#include <Wire.h>
// Load the `ESP32 AnalogWrite` by Brian Taylor
#include <analogWrite.h>
// Load the settings and own modules
#include "settings.h"
#include "display.h"
#include "drive.h"

void setup()
{
  // Start the serial monitor
  Serial.begin(115200);
  // Register the wheels as output pins
  pinMode(FrontRight, OUTPUT);
  pinMode(BackRight, OUTPUT);
  pinMode(FrontLeft, OUTPUT);
  pinMode(BackLeft, OUTPUT);
  pinMode(IRLeft, INPUT);
  pinMode(IRRight, INPUT);
  // Stop the robot
  analogWrite(FrontRight, LOW);
  analogWrite(FrontLeft, LOW);
  analogWrite(BackRight, LOW);
  analogWrite(BackLeft, LOW);
  // Show groupname on display
  displayBootAnimation();
  delay(3000);
}

void loop()
{
  unsigned long currentMillis = millis();
  int sRight = analogRead(IRRight);
  int sLeft = analogRead(IRLeft);
  display.clearDisplay(); // clears display
  display.invertDisplay(false);
  display.setTextSize(2); // sets text size
  display.setTextColor(WHITE);
  display.setCursor(0, 6); // sets cursor
  display.print("R: ");
  display.print(sRight);    // displays text
  display.setCursor(0, 26);
  display.print("L: ");
  display.print(sLeft);
  display.display();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (vTurn == LOW){
      vTurn = v;
    }
    else {
      vTurn = LOW;
    }
  }
  if (sRight < 120 && sLeft < 120)//If on white surface, go straight
  {
    straight();
  }
  if (sRight < 120 && sLeft > 120)//If right is on white and left is on black, go right
  {
    off();
    delay(200);
    right();
  }
  if (sRight > 120 && sLeft < 120)//if right is on black and left is on white, go left
  { 
    off();
    delay(200);
    left();
  }
  if (sRight > 120 && sLeft > 120 && backCount < 10)//if on black surface go back.
  {
    off();
    back();
    delay(100);
    right();
  }
  if (backCount >= 9)
  //turn off after 1 second
  {
    off();
  }
}
