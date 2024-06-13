#include "video_display.h"
#include "Interpreter.h"
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "MEO-0E1870";
const char* password = "ddcb9dc1e3";
WebServer server(80);

void handlePreflight() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204);
}

void executeArduinoFunction() {
  if (server.hasArg("input")) {
    String inputMessage = server.arg("input");
    const char* list[] = {inputMessage.c_str()};
    call_BTNfunction(list,1);
  }
  
 
}

void handleGetRequest() {
  if (server.hasArg("input")) {
    String inputMessage = server.arg("input");
    execute_script(inputMessage.c_str());
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{\"status\": \"success\", \"message\": \"Message received: " + inputMessage + "\"}");
  } else {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "application/json", "{\"status\": \"error\", \"message\": \"No input provided\"}");
  }
}

void handleExecuteRequest() {
  executeArduinoFunction();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"status\": \"success\", \"message\": \"Function executed\"}");
}

void setup() {
  Serial.begin(115200);
  initializeVideo();

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define as rotas
  server.on("/get", HTTP_GET, handleGetRequest);
  server.on("/execute", HTTP_GET, handleExecuteRequest);
  server.on("/get", HTTP_OPTIONS, handlePreflight);
  server.on("/execute", HTTP_OPTIONS, handlePreflight);

  // Inicia o servidor
  server.begin();
}

void loop() {
  // Handle server requests
  server.handleClient();
}
