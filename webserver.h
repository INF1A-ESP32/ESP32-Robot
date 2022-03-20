#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void webserverSetup(char* ssid, char* password)
{
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid, password);
  Serial.println("");

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am Robbie.");
  });

  AsyncElegantOTA.begin(&server);
  server.begin();
}
