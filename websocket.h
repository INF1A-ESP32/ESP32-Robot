#include <WiFi.h>              // ESP32 WiFi Library
#include <WebSocketsClient.h>  // WebSocket Client Library for WebSocket
#include <ArduinoJson.h>       // Arduino JSON Library

WebSocketsClient webSocket;   // websocket client class instance
StaticJsonDocument<100> doc;  // Allocate a static JSON document

bool ws_loggedin = false;
bool ws_connected = false;    // to keep track of status of the websocket
bool ws_has_json = false;     // to keep track if the websocket request has json

void sendMessageWebSocket(String wsData, char* wsMessage = "data") {
  String wsString = String(wsData);
  Serial.printf("[Websocket] send %s: %s\n", wsMessage, wsString);
  webSocket.sendTXT(wsString);
}

void webSocketEvent(WStype_t WStype, uint8_t* payload, size_t length) {
  // Try to parse JSON
  auto error = deserializeJson(doc, payload);
  ws_has_json = false;
  if (!error) ws_has_json = true;
  const char* json_loggedin = doc["loggedin"];
  const char* json_action = doc["action"];
  const char* json_status = doc["status"];
  const char* json_start = doc["start"];
  String login_msg = "{\"action\": \"login\", \"id\":\"" + String(WiFi.macAddress().c_str()) + "\"}";

  // switch case for the different types of websocket events
  switch (WStype) {
    case WStype_DISCONNECTED:
      Serial.println("[Websocket] Connection lost!!");
      ws_connected = false;
      break;

    case WStype_CONNECTED:
      Serial.printf("[Websocket] Connected to websocket: %s:%i\n", webSocketHost, webSocketPort);
      ws_connected = true;
      // Authenticate with the websocket client
      sendMessageWebSocket(login_msg, "credentials");
      ws_has_json = false;

      delay(2000);
      sendMessageWebSocket("{\"status\":\"preparing\", \"isDriving\": false, \"acceleration\": 0}");
      // delay(1000);
      //      sendMessageWebSocket("{\"status\":\"ready\", \"ready\": true}");
      //sendMessageWebSocket("Hallo");
      break;

    case WStype_TEXT:
      Serial.printf("[Websocket] recieved data: %s\n", payload);
      if (!ws_has_json) {
        // If there whas a error with parsing the json display it
        Serial.print("deserializeJson() failed with code ");
        Serial.println(error.c_str());
        break;
      }

      if (json_loggedin){
        Serial.printf("[Websocket] recieved data: loggedin=%s\n", json_loggedin);
      }

      // Print the status and action from the json data
      if (json_status) Serial.printf("[Websocket] recieved data: status=%s\n", json_status);
      if (json_action) Serial.printf("[Websocket] recieved data: action=%s\n", json_action);
      if (json_start) Serial.printf("[Websocket] recieved data: start=%s\n", json_start);
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
  Serial.print("Mac Address: ");
  Serial.println(WiFi.macAddress());

  // Start the connection to the websocket
  webSocket.begin(webSocketHost, webSocketPort);
  // Tell what will happen on a websocket event
  webSocket.onEvent(webSocketEvent);
  // if connection failed retry every 5s
  webSocket.setReconnectInterval(5000);
}
