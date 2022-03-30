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

WebSocketsClient webSocket;   // websocket client class instance
StaticJsonDocument<400> doc;  // Allocate a static JSON document

void sendMessageWebSocket(String wsData, char* wsMessage = "data") {
  String wsString = String(wsData);
  Serial.printf("[Websocket] send %s:", wsMessage);
  Serial.println(wsString.c_str());
  webSocket.sendTXT(wsString);
}

void wsStatusSend() {
  sendMessageWebSocket("{\"status\":\"" + robot_status + "\", \"isDriving\": " + robot_is_driving + ", \"acceleration\": " + String(robot_acceleration) + "}");
}

bool validGame(String gameName){
  return (gameName == "race");
} 

String converter(uint8_t* str) {
  return String((char*)str);
}
void webSocketEvent(WStype_t WStype, uint8_t* payload, size_t length) {
  String raw_data = converter(payload).c_str();
  // Try to parse JSON
  DeserializationError error = deserializeJson(doc, raw_data);
  wsHasJson = false;

  if (!error) wsHasJson = true;

  const bool json_loggedin = doc["loggedin"];
  const String json_action = doc["action"];
  const String json_status = doc["status"];
  const String json_start = doc["start"];
  const String json_game = doc["game"];

  String device_mac = WiFi.macAddress();
  String login_msg = "{\"action\": \"login\", \"id\":\"" + device_mac + "\"}";
  // switch case for the different types of websocket events
  switch (WStype) {
    case WStype_DISCONNECTED:
      Serial.println("[Websocket] Connection lost!!");
      wsConnected = false;
      setGameDefaults(); // Stop robot and reset defaults
      break;

    case WStype_CONNECTED:
      Serial.printf("[Websocket] Connected to websocket: %s:%i\n", webSocketHost, webSocketPort);
      wsConnected = true;
      
      // Authenticate with the websocket client
      sendMessageWebSocket(login_msg, "credentials");
      delay(1000);
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
        delay(2000);
        robot_status = "ready";
        prepairGame = false; 
        playGame = false;
        gameReady = false;
        gameName = "";
        wsLoggedin = true;
      } else if (!wsLoggedin) {
        Serial.println(json_loggedin);
        wsLoggedin = false;
        // Todo max 3 times try bevore error
        Serial.printf("[Websocket] Login Fail try \n");
        Serial.println("[Websocket] Server now will close connection");
        // TODO KILL CONNECTION
      }
      // Prevent execution when not loggedin.
      if(!wsLoggedin) break; 
      
      if (json_action) {
        Serial.println("[Websocket] Got an action");
        if (json_game!="null" && !validGame(json_game)) {
          Serial.println("[Websocket] Game cannot be found");
          sendMessageWebSocket("{\"error\": \"GAME_NOT_FOUND\"}");
          if (json_action == "prepare") {
            sendMessageWebSocket("{\"error\": \"GAME_NOT_PREPARED\"}");
          }
          sendMessageWebSocket("{\"status\": false, \"game\": \"" + json_game + "\"}");
          break;
        }
        if (json_action == "prepare") {
          // Start prepairing the game
          Serial.println("[Websocket] Start Prepairing game");
          robot_status = "preparing_game";
          prepairGame = true;
          gameName = json_game;
        } else if (json_action == "start") {
          // Start game
          Serial.println("[Websocket] Starting game");
          if (!gameReady) {
            sendMessageWebSocket("{\"error\": \"GAME_NOT_READY\"}");
            break;
          }
          prepairGame = false;
          playGame = true;
          gameName = json_game;
        } else if (json_action == "ended") {
          // Game ended

          Serial.println("[Websocket] Stopping game");
          analogWrite(FrontRight, 0);
          analogWrite(FrontLeft, 0);
          analogWrite(BackRight, 0);
          analogWrite(BackLeft, 0);
          setGameDefaults();
        }
      }


      // Print the status and action from the json data
      Serial.printf("[Websocket] recieved data: game=%s\n", json_game);
      Serial.printf("[Websocket] recieved data: status=%s\n", json_status);
      Serial.printf("[Websocket] recieved data: action=%s\n", json_action);
      Serial.printf("[Websocket] recieved data: start=%s\n", json_start);

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
  WiFi.mode(WIFI_STA);         // change the wifi mode to Station mode, which is the standard client mode
  WiFi.begin(ssid, password);  // Connect to the WiFi
  // Wait until the device is connected
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  // Print wifi infromation
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  String hostname = "Robby";
  WiFi.setHostname(hostname.c_str());
  Serial.print("Mac Address: ");
  Serial.println(WiFi.macAddress());

  // Start the connection to the websocket
  webSocket.begin(webSocketHost, webSocketPort);
  // Tell what will happen on a websocket event
  webSocket.onEvent(webSocketEvent);
  // if connection failed retry every 5s
  webSocket.setReconnectInterval(5000);
}
