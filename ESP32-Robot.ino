#include <analogWrite.h>
#include "settings.h"

void setup() {
  // Start the serial monitor
  Serial.begin(115200);
  // Register the wheels as output pins
  pinMode(rechtsVoor, OUTPUT);
  pinMode(rechtsAchter, OUTPUT);
  pinMode(linksVoor, OUTPUT);
  pinMode(linksAchter, OUTPUT);
}

void loop() {
  // Enable the 2 front wheels
  analogWrite(rechtsVoor, speedR);
  analogWrite(linksVoor, speedL);
  analogWrite(rechtsAchter, LOW);
  analogWrite(linksAchter, LOW);
  // Drive for 5 seconds
  delay(5000);
  // Stop the car for 10 sec
  analogWrite(rechtsVoor, LOW);
  analogWrite(linksVoor, LOW);
  delay(10000);
}
