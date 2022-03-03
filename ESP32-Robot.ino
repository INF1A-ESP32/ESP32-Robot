#include <SPI.h>
#include <Wire.h>
// Load the `ESP32 AnalogWrite` by Brian Taylor
#include <analogWrite.h>
// Load the settings and own modules
#include "settings.h"
#include "display.h"

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
  int sRight = analogRead(IRRight);
  int sLeft = analogRead(IRLeft);
  display.clearDisplay(); // clears display
  display.invertDisplay(false);
  display.setTextSize(2); // sets text size
  display.setTextColor(WHITE);
  display.setCursor(2, 6); // sets cursor
  display.print("R: ");
  display.print(sRight);    // displays text
  display.setTextSize(2);   // sets text size
  display.setCursor(2, 26); // sets cursor
  display.print("L: ");
  display.print(sLeft); // displays text
  display.display();
  int backCount = 0;
/*  boolean right = false;
  boolean left = false;
  if (sRight > 3600 && sRight < 3950)
  {
    right = true;
  }
  if (sLeft > 3600 && sLeft < 3950)
  {
    left = true;
  }
  if (right & left)
  {
    analogWrite(FrontRight, speedR);
    analogWrite(FrontLeft, speedL);
  }
  else if (right)
  {
    analogWrite(FrontLeft, 200);
  }
  else if (left)
  {
    analogWrite(FrontRight, 200);
  }
  else
  {
    analogWrite(FrontRight, LOW);
    analogWrite(FrontLeft, LOW);
    analogWrite(BackRight, LOW);
    analogWrite(BackLeft, LOW);
  }*/
  if (sRight > 2700 && sLeft > 2700)
  {
    backCount = 0;
    analogWrite(BackRight, LOW);
    analogWrite(BackLeft, LOW);
    analogWrite(FrontRight, 200);
    analogWrite(FrontLeft, 200);
  }
    if (sRight < 2700 && sLeft > 2700)
  {
    backCount = 0;
    analogWrite(FrontLeft, LOW);
    analogWrite(BackRight, LOW);
    analogWrite(FrontRight, 200);
    analogWrite(BackLeft, 200);
  }
    if (sRight > 2700 && sLeft < 2700)
  {
    backCount = 0;
    analogWrite(FrontRight, LOW);
    analogWrite(BackLeft, LOW);
    analogWrite(FrontLeft, 200);
    analogWrite(BackRight, 200);
  }
    if (sRight < 2700 && sLeft < 2700)
  {
    backCount+=1;
    analogWrite(FrontLeft, LOW);
    analogWrite(FrontRight, LOW);
    analogWrite(BackLeft, 200);
    analogWrite(BackRight, 200);
  }
  if (backCount > 10)
  {
    analogWrite(FrontLeft, LOW);
    analogWrite(FrontRight, LOW);
    analogWrite(BackLeft, LOW);
    analogWrite(BackRight, LOW);
  }
}
