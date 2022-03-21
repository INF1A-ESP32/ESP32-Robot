#include <WiFi.h> // ESP32 WiFi Library
#include <WebServer.h> // WebServer Library for ESP32
#include <WebSocketsClient.h> // WebSocket Client Library for WebSocket
#include <ArduinoJson.h> // Arduino JSON Library

WebSocketsClient webSocket; // websocket client class instance
StaticJsonDocument<100> doc; // Allocate a static JSON document

bool ws_connected = false; // to keep track of status of the websocket
bool ws_has_json = false; // to keep track if the websocket request has json

void sendMessageWebSocket(char* wsData, char* wsMessage="data")
{
  Serial.printf("[Websocket] send %s: %s\n", wsMessage, wsData);
  webSocket.sendTXT(wsData);
}

void webSocketEvent(WStype_t WStype, uint8_t * payload, size_t length) 
{
  // Try to parse JSON
  auto error = deserializeJson(doc, payload);
  ws_has_json=false;
  if(!error) ws_has_json=true;
  const char* json_action = doc["action"];
  const char* json_status = doc["status"];

  // switch case for the different types of websocket events
  switch(WStype) {
    case WStype_DISCONNECTED:
      Serial.println("[Websocket] Connection lost!!");
      ws_connected = false;
      break;
      
    case WStype_CONNECTED:
      Serial.printf("[Websocket] Connected to websocket: %s:%i\n", webSocketHost, webSocketPort);
      ws_connected = true;
      // Authenticate with the websocket client      
      sendMessageWebSocket("{\"id\":\"34:94:54:25:13:84\",\"name\":\"Robbie\", \"team\": \"INF1A\"}", "credentials");
      delay(1000);
      sendMessageWebSocket("{\"status\":\"booting\", \"ready\": false}");
      delay(1000);
      sendMessageWebSocket("{\"status\":\"ready\", \"ready\": true}");
      break;
      
    case WStype_TEXT:
      if(!ws_has_json){
        // If there whas a error with parsing the json display it
        Serial.print("deserializeJson() failed with code ");
        Serial.println(error.c_str());
        break;
      }
      // Print the status and action from the json data
      if(json_status) Serial.printf("[Websocket] recieved data: status=%s\n",json_status);
      if(json_action) Serial.printf("[Websocket] recieved data: action=%s\n",json_action);
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

void connectWebSocket()
{
  WiFi.mode(WIFI_STA); // change the wifi mode to Station mode, which is the standard client mode
  WiFi.begin(ssid, password); // Connect to the WiFi
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
