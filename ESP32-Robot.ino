/*
-------------------------------------
 Configure Timers
-------------------------------------
*/

// websocket status updates
unsigned long wsStatusMillis = 0;
const int wsStatusInterval = 5000;

// game prepair timer
unsigned long startGameMillis = 0;
const int startGameInterval = 15000;

// game play timer for testing
unsigned long gameMillis = 0;
const int gameInterval = 30000;

// reset game timers to prefent inaccurate messages
void resetGameTimers(){
  startGameMillis = 0;
  gameMillis = 0;
}

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
int robot_acceleration;

// Game settings/status
bool prepairGame;
bool playGame;
bool gameReady;
String gameName;

void setStatusDefaults(){
  robot_status = "ready";
  // Set a true/false string because else it cannot send over the websocket
  robot_is_driving = "false"; 
  robot_acceleration = 0;
}

void setGameDefaults(){
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

#include <SPI.h>
#include <Wire.h>
// Load the `ESP32 AnalogWrite` by Brian Taylor
#include <analogWrite.h>
// Load the settings and own modules
#include "settings.h"
#include "display.h"
#include "websocket.h"

void setup() {
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
  setGameDefaults();
  connectWebSocket();
  delay(3000);
}

int calcSpeed(char wheel, int power){
  if(wheel == 'l'){
    return floor(speedL/100*power);
  }
  return floor(speedR/100*power);
}

void loop() {
  // Keeps the websocket open and recieves commands.
  // If there is a delay inside this loop the websocket will be disconnected
  webSocket.loop(); 

  // Get the current time for the timers
  unsigned long currentMillis = millis(); 

  // Send every X seconds a status to the server with the websocket.
  if (currentMillis - wsStatusMillis >= wsStatusInterval && wsLoggedin) {
    if(wsConnected) wsStatusSend(); // Only send updates when the websocket is connected
    
    // Reset the timer
    wsStatusMillis = millis(); 
  }

  // When the game needs to be prepaired
  if(prepairGame){
    // If there is no time yet set the current time for the timers.
    if(startGameMillis==0) startGameMillis = millis();

    // For testing send after X seconds status that the game is prepaired.
    if (currentMillis - startGameMillis >= startGameInterval) {
      // Tell the server that the game is ready to start
      sendMessageWebSocket("{\"status\": true, \"game\": \""+gameName+"\"}");
      // Update the status variables so it can play the game on command.
      gameReady = true;
      prepairGame = false;
      robot_status = "ready";
    }
  }

  // When the game needs to be played
  if(playGame){
    analogWrite(FrontRight, 255);
    analogWrite(FrontLeft, 255);
    analogWrite(BackRight, 0);
    analogWrite(BackLeft, 0);
    // Update the statuses
    robot_status = "in_game";
    robot_acceleration = rand() % 100 + 1; // For testing does this return a ranom number
    robot_is_driving = "true"; // For testing is this not read from the sensors

    // If there is no time yet set the current time for the timers.
    if(gameMillis==0) gameMillis = millis();

    // For testing send after X seconds status that the game is finisched.
    if (currentMillis - gameMillis >= gameInterval &&robot_status == "in_game" ) {
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
  
  char* predictL = "UNKNOWN";
  char* predictR = "UNKNOWN";

  int sLeft = analogRead(39);
  int sRight = analogRead(34);

  int lWhiteThreshold = 90;
  int lgrayLightThreshold = 250;
  int lgrayDarkThreshold = 1800;

  int rWhiteThreshold = 60;
  int rgrayLightThreshold = 250;
  int rgrayDarkThreshold = 1800;

  if (sLeft < lWhiteThreshold) {
    predictL = "white";
  } else if (sLeft < lgrayLightThreshold) {
    predictL = "l gray";
  } else if (sLeft < lgrayDarkThreshold) {
    predictL = "d gray";
  } else {
    predictL = "black";
  }
  if (sRight < rWhiteThreshold) {
    predictR = "white";
  } else if (sRight < rgrayLightThreshold) {
    predictR = "l gray";
  } else if (sRight < rgrayDarkThreshold) {
    predictR = "d gray";
  } else {
    predictR = "black";
  }

  display.clearDisplay();  // clears display
  display.invertDisplay(false);
  display.setTextSize(2);  // sets text size
  display.setTextColor(WHITE);
  display.setCursor(2, 6);  // sets cursor
  display.print(sRight);

  display.setTextSize(2);    // sets text size
  display.setCursor(2, 26);  // sets cursor
  display.print("PL: ");
  display.print(predictL);  // displays text
  display.setCursor(2, 46);
  display.print("PR: ");
  display.print(predictR);  // displays text
  display.display();

  return;
}
