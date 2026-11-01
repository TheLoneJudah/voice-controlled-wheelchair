// Voice-Controlled Wheelchair
// Copyright (c) 2025 Ajibade Praise Titus. All rights reserved.

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Wheelchair_AP";
const char* password = "12345678";

ESP8266WebServer server(80);
String latestMessage = "";

void setup() {
  Serial.begin(9600);   // Communication with Arduino
  WiFi.softAP(ssid, password);
  server.on("/command", HTTP_GET, []() {
    String cmd = server.arg("text");
    Serial.println(cmd);  // Send to Arduino
    server.send(200, "text/plain", "Command Sent: " + cmd);
  });

  server.on("/status", HTTP_GET, []() {
    server.send(200, "text/plain", latestMessage);  // Send latest Arduino message to app
  });

  server.begin();
}

void loop() {
  server.handleClient();

  if (Serial.available()) {
    latestMessage = Serial.readStringUntil('\n');
    latestMessage.trim();
  }
}
