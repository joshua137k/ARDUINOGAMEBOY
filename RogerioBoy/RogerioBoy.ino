#include "video_display.h"
#include "Interpreter.h"
#include <WiFi.h>
#include <WebServer.h>


const char* ssid = "MEO-0E1870";
const char* password = "ddcb9dc1e3";
WebServer server(80);


// Página HTML
const char* html = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <title>ESP32 Text Input</title>
    <style>
      body { font-family: Arial, sans-serif; margin: 40px; }
      textarea { width: 100%; height: 200px; }
      input[type=submit] { font-size: 16px; padding: 10px 20px; }
    </style>
  </head>
  <body>
    <h1>Enter Script Text</h1>
    <form action="/get" method="GET">
      <textarea name="input"></textarea><br><br>
      <input type="submit" value="Submit">
    </form>
  </body>
  </html>
)rawliteral";


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
  // Imprime o endereço IP
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Rota para servir a página HTML
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", html);
  });

  // Rota para lidar com o formulário
  server.on("/get", HTTP_GET, []() {
    if (server.hasArg("input")) {
      String inputMessage = server.arg("input");
      execute_script(inputMessage.c_str());
      server.send(200, "text/html", "Message received: " + inputMessage + "<br><a href=\"/\">Go Back</a>");
    } else {
      server.send(200, "text/html", "No message sent<br><a href=\"/\">Go Back</a>");
    }
  });

  // Inicia o servidor
  server.begin();
  //execute_script("TOOS");


  
  

  

}





void loop() {
  // Handle server requests
  server.handleClient();
}