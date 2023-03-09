/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp8266-nodemcu-plot-readings-charts-multiple/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/
#define RXp2 16
#define TXp2 17

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Arduino_JSON.h>

// Replace with your network credentials
const char* ssid = "Huyen Minh";
const char* password = "huyen2002";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to hold sensor readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;


//String getSensorReadings(){
////sensors.requestTemperatures();
//  readings["sensor1"] = Serial.read();
//
//  String jsonString = JSON.stringify(readings);
//  return jsonString;
//}
void initSPIFFS() {
  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}
// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  initWiFi();
  initSPIFFS();
while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");
  

  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    lastTime = lastTime + timerDelay;
    request->send(200, "text/plain", "OK!");
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
  server.begin();
}
String rotorval = "";
String prev_rotorval = "";
void loop() {
    rotorval = Serial2.readStringUntil('\n');
    if (rotorval != "") {
      prev_rotorval = rotorval;
      }
    Serial.print("rotorval: ");
    Serial.println(rotorval);
    Serial.print("prev_rotorval: ");
    Serial.println(prev_rotorval);
    
//    Serial.println(char(Serial2.read()));
    if (rotorval != "") {
      readings["sensor1"] = rotorval;
    } else {
      readings["sensor1"] = prev_rotorval;
      }
    
    String jsonString = JSON.stringify(readings);
//    Serial.println(jsonString);
  if ((millis() - lastTime) > timerDelay) {
    // Send Events to the client with the Sensor Readings Every 10 seconds
    events.send("ping",NULL,millis());
//    events.send(getSensorReadings().c_str(),"new_readings" ,millis());
    events.send(jsonString.c_str(),"new_readings" ,millis());
    lastTime = millis();
  }

}
