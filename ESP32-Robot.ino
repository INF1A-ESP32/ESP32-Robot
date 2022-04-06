#include <Wire.h>
// Load the `ESP32 AnalogWrite` by Brian Taylor
#include <analogWrite.h>
#include "Adafruit_VL53L0X.h"
// Load the settings and own modules
#include "settings.h"
#include "display.h"
#include "drive.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup()
{
  // Start the serial monitor
  Serial.begin(115200);
  // Register the wheels as output pins
  pinMode(FrontRight, OUTPUT);
  pinMode(BackRight, OUTPUT);
  pinMode(FrontLeft, OUTPUT);
  pinMode(BackLeft, OUTPUT);
  // Stop the robot
  analogWrite(FrontRight, LOW);
  analogWrite(FrontLeft, LOW);
  analogWrite(BackRight, LOW);
  analogWrite(BackLeft, LOW);
  // Show groupname on display
  displayBootAnimation();
  delay(3000);
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}

void loop()
{
  unsigned long currentMillis = millis();

  VL53L0X_RangingMeasurementData_t measure;
  int distance = measure.RangeMilliMeter; 
  int sRight = analogRead(IRRight);
  int sLeft = analogRead(IRLeft);
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout! 

  display.clearDisplay(); // clears display
  display.invertDisplay(false);
  display.setTextSize(2); // sets text size
  display.setTextColor(WHITE);
  display.setCursor(0, 6); // sets cursor
  display.print("Distance: ");
  display.print(distance);    // displays text
  display.display();
  
    
      if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      Serial.print("Distance (mm): "); Serial.println(distance); 
      } else {
      Serial.println(" out of range ");
      }

       if (distance > 220)
        {
          straight();
        }

       if (distance < 220)//search
       {
                  off();

                  delay(800);
      
                  analogWrite(FrontRight, LOW);
                  analogWrite(BackLeft, LOW);
                  analogWrite(FrontLeft, 165);
                  analogWrite(BackRight, 165);  
    
                  delay(500);
    
                  analogWrite(FrontLeft, LOW);
                  analogWrite(BackRight, LOW);
                  analogWrite(FrontRight, 165);
                  analogWrite(BackLeft, 165);
    
                  delay(400);   

                  off();

                  delay(1000);

                  back();

                  delay(100);
       }

         if (sRight > 2100 && sLeft > 2100) {// if robot on black surface, finish
          off();
        }
         
}
