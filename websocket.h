/*
-------------------------------------
 Include external liberaries
-------------------------------------
*/
// ESP32 WiFi Library
#include <WiFi.h>
// Websocket Client
#include <WebSocketsClient.h>
// Arduino JSON
#include <ArduinoJson.h>

// Set the macAdress to a varibale
// When testing on diverent ESP32 change WiFi.macAddress() to
// "F0:08:D1:D1:72:A0"
String device_mac = WiFi.macAddress();

WebSocketsClient webSocket;  // websocket client class instance
StaticJsonDocument<400> doc; // Allocate a static JSON document

void sendMessageWebSocket(String wsData, char *wsMessage = "data") {
  String wsString = String(wsData);
  Serial.printf("[Websocket] send %s:", wsMessage);
  Serial.println(wsString.c_str());
  webSocket.sendTXT(wsString);
}

void wsStatusSend() {
  sendMessageWebSocket("{\"status\":\"" + robot_status +
                       "\", \"isDriving\": " + robot_is_driving +
                       ", \"acceleration\": " + String(robot_acceleration) +
                       "}");
}

bool validGame(String gameName) {
  return (gameName == "race" || gameName == "butler");
}

void webSocketEvent(WStype_t WStype, uint8_t *payload, size_t length) {
  // Try to parse JSON
  DeserializationError error = deserializeJson(doc, payload);
  // If there is no error set wsHasJson to true
  wsHasJson = (!error);

  const bool json_loggedin = doc["loggedin"];
  const String json_action = doc["action"];
  const String json_status = doc["status"];
  const String json_start = doc["start"];
  const String json_game = doc["game"];

  // switch case for the different types of websocket events
  switch (WStype) {
  case WStype_DISCONNECTED:
    Serial.println("[Websocket] Connection lost!!");
    wsConnected = false;
    setGameDefaults(); // Stop robot and reset defaults
    break;

  case WStype_CONNECTED:
    Serial.printf("[Websocket] Connected to websocket: %s:%i\n", webSocketHost,
                  webSocketPort);
    wsConnected = true;

    // Authenticate with the websocket client
    sendMessageWebSocket("{\"action\": \"login\", \"id\":\"" + device_mac +
                             "\"}",
                         "credentials");
    robot_status = "preparing";
    wsStatusSend();
    break;

  case WStype_TEXT:
    Serial.printf("[Websocket] get text: %s\n", payload);
    if (!wsHasJson) {
      // If there whas a error with parsing the json display it
      Serial.print("deserializeJson() failed with code ");
      Serial.println(error.c_str());
      // Send the server a request that they have send invalid JSON
      sendMessageWebSocket("{\"error\": \"INVALID_JSON\"}");
      break;
    }

    // Handle when login data is send
    if (json_loggedin) {
      Serial.println("[Websocket] Login Success");
      setGameDefaults();
      wsLoggedin = true;
      displayText("Ready");
      robot_status = "ready";
    } else if (!wsLoggedin) {
      Serial.println("[Websocket] Login Fail");
      wsLoggedin = false;
      displayText("Websocket", "LoginError");
      webSocket.disconnect();
    }

    // Prevent execution when not loggedin.
    if (!wsLoggedin)
      break;

    // Print the status and action from the json data
    Serial.printf("[Websocket] recieved data: game=%s\n", json_game);
    Serial.printf("[Websocket] recieved data: status=%s\n", json_status);
    Serial.printf("[Websocket] recieved data: action=%s\n", json_action);
    Serial.printf("[Websocket] recieved data: start=%s\n", json_start);

    // If the request has no action do nothing
    if (!json_action)
      break;

    if (json_action == "prepare") {
      // Start prepairing the game
      Serial.println("[Websocket] Start Prepairing game");
      robot_status = "preparing_game";
      prepairGame = true;
      gameName = json_game;
      displayText(gameName, "Voorbereid");
    } else if (json_action == "start") {
      // Start game
      Serial.println("[Websocket] Starting game");
      if (!gameReady) {
        sendMessageWebSocket("{\"error\": \"GAME_NOT_READY\"}");
        break;
      }
      displayText(json_game, "Spelen");
      prepairGame = false;
      playGame = true;
      robot_status = "in_game";
    } else if (json_action == "ended") {
      // Stop game
      Serial.println("[Websocket] Stopping game");
      displayText(gameName, "Stoppen");
      off(); // Stop the robot
      setGameDefaults();
    }
    break;

  case WStype_PING:
    // pong will be send automatically
    Serial.println("[Websocket] get Ping: response pong");
    break;

  case WStype_PONG:
    // answer to a ping we send
    Serial.println("[Websocket] gets pong from server");
    break;

  default:
    Serial.printf("[Websocket] has a unknown event: %s", WStype);
    break;
  }
}

void connectWebSocket() {
  bool wifiConnected = false;
  String hostname = "onze.robbie.local";
  WiFi.setHostname(hostname.c_str());
  while (!wifiConnected) {
    displayText("Verbinden met WIFI", 2, 6);
    WiFi.mode(WIFI_STA); // change the wifi mode to Station mode, which is the
                         // standard client mode
    WiFi.begin(ssid, password); // Connect to the WiFi
    // Wait until the device is connected
    int test = 0;
    while (WiFi.status() != WL_CONNECTED) {
      display.setTextSize(4);
      display.setCursor(test, 36);
      display.println("_");
      display.display();
      Serial.print(".");
      test++;
      if (test / 5 > 10)
        break;
      delay(100);
    }
    if (WiFi.status() == WL_CONNECTED) {
      wifiConnected = true;
    } else {
      Serial.println("Try again to connect to the wifi");
    }
  }
  displayText("Verbonden");
  delay(2000);

  // Print wifi infromation
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Mac Address: ");
  Serial.println(WiFi.macAddress());
  displayText("Verbinden", "websocket");
  delay(1000);
  // Start the connection to the websocket
  webSocket.begin(webSocketHost, webSocketPort);
  // Tell what will happen on a websocket event
  webSocket.onEvent(webSocketEvent);
  // if connection failed retry every 5s
  webSocket.setReconnectInterval(5000);
}
