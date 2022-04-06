/*
-------------------------------------
 Configure Timers
-------------------------------------
*/

// websocket status updates
unsigned long wsStatusMillis = 0;
const int wsStatusInterval = 5000;

// reset game timers to prefent inaccurate messages
void resetGameTimers() {
}

/*
-------------------------------------
 Set game default settings
-------------------------------------
*/

// Websocket status
bool wsLoggedin = false;
bool wsConnected = false;
bool wsHasJson = false;  // Tracks if the websocket request has json

// Robot status
String robot_status = "preparing";
String robot_is_driving;
int robot_acceleration;

// Game settings/status
bool prepairGame;
bool playGame;
bool gameReady;
bool gameOver;
String gameName;

void setStatusDefaults() {
  robot_status = "ready";
  // Set a true/false string because else it cannot send over the websocket
  robot_is_driving = "false";
  robot_acceleration = 0;
}

void setGameDefaults() {
  prepairGame = false;
  playGame = false;
  gameReady = false;
  gameName = "";
  setStatusDefaults();
}

/*
-------------------------------------
 Include external liberaries
-------------------------------------
*/

#include <Wire.h>
// Load the `ESP32 AnalogWrite` by Brian Taylor
#include <analogWrite.h>
#include "Adafruit_VL53L0X.h"
// Load the settings and own modules
#include "display.h"
#include "settings.h"
#include "drive.h"
#include "websocket.h"
#include "butler.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  // Start the serial monitor
  Serial.begin(115200);
  // Register the wheels as output pins
  pinMode(FrontRight, OUTPUT);
  pinMode(BackRight, OUTPUT);
  pinMode(FrontLeft, OUTPUT);
  pinMode(BackLeft, OUTPUT);
  // Stop the robot
  off();
  // Show groupname on display
  displayBootAnimation();
  setGameDefaults();
  connectWebSocket();
  delay(3000);
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power lidar
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  // Keeps the websocket open and recieves commands.
  // If there is a delay inside this loop the websocket will be disconnected
  webSocket.loop();

  // Get the current time for the timers
  unsigned long currentMillis = millis();

  // Send every X seconds a status to the server with the websocket.
  if (currentMillis - wsStatusMillis >= 5000 && wsLoggedin) {
    if (wsConnected)
      wsStatusSend();  // Only send updates when the websocket is connected

    // Reset the timer
    wsStatusMillis = millis();
  }

  //gets de lidar distance data
  VL53L0X_RangingMeasurementData_t measure;
  int distance = measure.RangeMilliMeter; 
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout! 

  int sRight = analogRead(IRRight);
  int sLeft = analogRead(IRLeft);
  display.clearDisplay();  // clears display
  display.invertDisplay(false);
  display.setTextSize(2);  // sets text size
  display.setTextColor(WHITE);

  // When the game needs to be prepaired
  if (prepairGame) {
    if (gameName == "race") {
      // When the game is race the robot is direct ready
      sendMessageWebSocket("{\"status\": true, \"game\": \"" + gameName + "\"}");
      robot_status = "ready";
      prepairGame = false;
      gameReady = true;
    }
    if (gameName == "butler") {
      sendMessageWebSocket("{\"status\": true, \"game\": \"" + gameName + "\"}");
      robot_status = "ready";
      prepairGame = false;
      gameReady = true;
    }
  }

  if (playGame) {
    if (gameName == "race") {
      robot_status = "in_game";
      robot_is_driving = "true";

      display.setCursor(0, 6);  // sets cursor
      display.print("R: ");
      display.print(sRight);  // displays text
      display.setCursor(0, 26);
      display.print("L: ");
      display.print(sLeft);
      display.display();

      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (vTurn == LOW) {
          vTurn = v;
        } else {
          vTurn = LOW;
        }
      }
      if (sRight < 120 && sLeft < 120) {
        // If on white surface, go straight
        straight();
      }
      // websocket status updates
      unsigned long wsStatusMillis = 0;
      const int wsStatusInterval = 5000;
     
      if (sRight < 120 && sLeft > 120) {
        // If right is on white and left is on black, go right
        // off();
        // delay(200);
        right();
      }
      if (sRight > 120 && sLeft < 120) {
        // if right is on black and left is on white, go left
        // off();
        // delay(200);
        left();
      }
      if (sRight > 120 && sLeft > 120 && backCount < 10) {
        // if on black surface go back.
        // off();
        back();
        delay(100);
        // left();
      }
      if (backCount >= 9) {
        // turn off after 1 second
        off();
      }
      if (sRight <2 && sLeft < 2) {
        gameOver = true;
      }
    }else if (gameName == "maze"){
      
    }else if (gameName == "butler"){
        robot_status = "in_game";
        robot_is_driving = "true";
          
          display.clearDisplay();  // clears display
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

    if (gameOver) {
      robot_is_driving = "false";
      // Stop the game
      setGameDefaults();
      // Update the status messages
      robot_status = "finished";
      robot_acceleration = 0;
      robot_is_driving = "false";
      analogWrite(FrontRight, 0);
      analogWrite(FrontLeft, 0);
      analogWrite(BackRight, 0);
      analogWrite(BackLeft, 0);
      // Send a update to the websocket
      wsStatusSend();
      // Reset the timers
      resetGameTimers();
      // Update the status message
      robot_status = "ready";
    }
  }
}
