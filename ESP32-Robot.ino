/*
-------------------------------------
 Configure Timers
-------------------------------------
*/

// websocket status updates
unsigned long wsStatusMillis = 0;
const int wsStatusInterval = 5000;

/*
-------------------------------------
 Set game default settings
-------------------------------------
*/

// Websocket status
bool wsLoggedin = false;
bool wsConnected = false;
bool wsHasJson = false; // Tracks if the websocket request has json

// Robot status
String robot_status = "preparing";
String robot_is_driving;
float robot_acceleration;

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
#include <Adafruit_VL53L0X.h>
#include <analogWrite.h>
// Load the settings and own modules

#include "display.h"
#include "settings.h"
#include "drive.h"
#include "websocket.h"
#include "butler.h"
#include "maze.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

/*
-----------------------------------
  MPU9250
-----------------------------------
*/
#include <MPU9250_asukiaaa.h>

#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

MPU9250_asukiaaa mySensor;

/*
-----------------------------------
  Setup
-----------------------------------
*/
void setup() {
  // Start the serial monitor
  Serial.begin(115200);
  // Wait until the serial monitor is ready
  while (!Serial)
    ;
  // Register the wheels as output pins
  pinMode(FrontRight, OUTPUT);
  pinMode(BackRight, OUTPUT);
  pinMode(FrontLeft, OUTPUT);
  pinMode(BackLeft, OUTPUT);
  pinMode(IRLeft, INPUT);
  pinMode(IRRight, INPUT);
  // Stop the robot
  off();
  // Show groupname on display
  displayBootAnimation();
  setGameDefaults();
  connectWebSocket();
#ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
#endif
  delay(3000);
  while (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
  }
  mySensor.beginAccel();
  if (gameName == "butler") {
    FrontRight = 17;
    BackRight = 16;
    FrontLeft = 18;
    BackLeft = 5;
  }
  if (gameName == "butler") {
    speedL = 233;
    speedR = 214;
  }
}

void loop() {
  // Keeps the websocket open and recieves commands.
  // If there is a delay inside this loop the websocket will be disconnected
  webSocket.loop();

  // Get the current time for the timers
  unsigned long currentMillis = millis();

  // Send every X seconds a status to the server with the websocket.
  if (currentMillis - wsStatusMillis >= 5000 && wsLoggedin) {
    if (wsConnected) {
      if (robot_status == "ready")
        displayText("Ready");
      if (mySensor.accelUpdate() == 0) {
        robot_acceleration = mySensor.accelX();
      } else {
        robot_acceleration = 0;
        Serial.println("Cannot read acceleration sensor");
      }
      wsStatusSend(); // Only send updates when the websocket is connected
    }
    // Reset the timer
    wsStatusMillis = millis();
  }

  // gets de lidar distance data
  VL53L0X_RangingMeasurementData_t measure;
  int distance = measure.RangeMilliMeter;
  lox.rangingTest(&measure,
                  false); // pass in 'true' to get debug data printout!

  int sRight = analogRead(IRRight);
  int sLeft = analogRead(IRLeft);
  display.clearDisplay(); // clears display
  display.invertDisplay(false);
  display.setTextSize(2); // sets text size
  display.setTextColor(WHITE);

  // When the game needs to be prepaired
  if (prepairGame) {
    if (validGame(gameName)) {
      // When the game is race the robot is direct ready
      sendMessageWebSocket("{\"status\": true, \"game\": \"" + gameName +
                           "\"}");
      robot_status = "ready";
      prepairGame = false;
      gameReady = true;
    }
  }
  if (playGame) {
    robot_status = "in_game";
    robot_is_driving = "true";

    if (gameName == "race") {
      if (currentMillis - previousMillis >= 150) {
        previousMillis = currentMillis;
        if (vTurn == 0) {
          vTurn = v;
        } else {
          vTurn = 0;
        }
      }
      if (sRight < 120 && sLeft < 120) {
        // If on white surface, go straight
        straight();
      }

      if (sRight < 120 && sLeft > 120) {
        // If right is on white and left is on black, go right
        right();
      }
      if (sRight > 120 && sLeft < 120) {
        // if right is on black and left is on white, go left
        left();
      }
      if (sRight > 120 && sLeft > 120 && backCount < 10) {
        // if on black surface go back.
        back();
        delay(100);
      }
      if (backCount >= 9) {
        // turn off after 1 second
        off();
      }
      if (sRight < 2 && sLeft < 2) {
        gameOver = true;
      }
    } else if (gameName == "butler") {
      display.clearDisplay();  // clears display
      display.setCursor(0, 6); // sets cursor
      display.print("Distance: ");
      display.print(distance); // displays text
      display.display();

      if (measure.RangeStatus != 4) { // phase failures have incorrect data
        Serial.print("Distance (mm): ");
        Serial.println(distance);
      } else {
        Serial.println(" out of range ");
      }

      if (distance > 220) {
        straight();
      }

      if (distance < 220) {
        // search for the "gate"
        off();

        delay(800);

        analogWrite(FrontRight, 0);
        analogWrite(BackLeft, 0);
        analogWrite(FrontLeft, 165);
        analogWrite(BackRight, 165);

        delay(500);

        analogWrite(FrontLeft, 0);
        analogWrite(BackRight, 0);
        analogWrite(FrontRight, 165);
        analogWrite(BackLeft, 165);

        delay(400);

        off();

        delay(1000);

        back();

        delay(100);
      }

      if (sRight > 2100 && sLeft > 2100) {
        // if robot on black surface, finish
        off();
      }
    } else if (gameName == "maze"){
      playMaze();
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
      // Update the status message
      robot_status = "ready";
    }
  }
}
