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
int backCount = 0;

void strait()
//drive strait
{
  backCount = 0;
  analogWrite(BackRight, LOW);
  analogWrite(BackLeft, LOW);
  analogWrite(FrontRight, v);
  analogWrite(FrontLeft, v);
}

void left()
//drive left
{
  backCount = 0;
  analogWrite(FrontLeft, LOW);
  analogWrite(BackRight, LOW);
  analogWrite(FrontRight, v);
  analogWrite(BackLeft, v);
}

void right()
//drive right
{
  backCount = 0;
  analogWrite(FrontRight, LOW);
  analogWrite(BackLeft, LOW);
  analogWrite(FrontLeft, v);
  analogWrite(BackRight, v);
}

void back()
//drive back
{
  backCount++;
  analogWrite(FrontLeft, LOW);
  analogWrite(FrontRight, LOW);
  analogWrite(BackLeft, v);
  analogWrite(BackRight, v);
}

void off()
//turn off
{
  analogWrite(FrontLeft, LOW);
  analogWrite(FrontRight, LOW);
  analogWrite(BackLeft, LOW);
  analogWrite(BackRight, LOW);
}

void loop()
{
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
  if (sRight > 2700 && sLeft > 2700)
  {
    strait();
  }
  if (sRight < 2700 && sLeft > 2700)
  {
    left();
  }
  if (sRight > 2700 && sLeft < 2700)
  {
    right();
  }
  if (sRight < 2700 && sLeft < 2700 && backCount < 10)
  {
    back();
    delay(100);
  }
  if (backCount >= 10)
  //turn off after 1 second
  {
    off();
  }
}
