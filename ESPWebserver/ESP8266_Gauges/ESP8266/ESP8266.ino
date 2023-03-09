#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <Arduino_JSON.h>


// Replace with your network credentials
const char* ssid = "Huyen Minh";
const char* password = "huyen2002";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;  
unsigned long timerDelay = 1000;


// Get Sensor Readings and return JSON object
String getSensorReadings_1(){
  readings["temperature"] = random(10,40);  //String(bme.readTemperature());
  readings["humidity"] = random(0,100);  //String(bme.readHumidity());
 
  String jsonString_1 = JSON.stringify(readings);
  return jsonString_1;
}
String getSensorReadings_2(){
 //sensors.requestTemperatures();
  readings["sensor1"] = random(0,25);
  readings["sensor2"] = random(30,50);
  readings["sensor3"] = random(40,60);
  readings["sensor4"] = random(60,90);
String jsonString_2 = JSON.stringify(readings);
  return jsonString_2;
}
// Initialize LittleFS
void initFS() {
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
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
  Serial.begin(115200);
  initWiFi();
  initFS();

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.serveStatic("/", LittleFS, "/");
  
  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String();
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

void loop() {
  if ((millis() - lastTime) > timerDelay) {
//     Send Events to the client with the Sensor Readings Every 30 seconds
    events.send("ping",NULL,millis());
    events.send(getSensorReadings_1().c_str(),"new_readings_1" ,millis());
    events.send(getSensorReadings_2().c_str(),"new_readings_2" ,millis());
    lastTime = millis();
  }
}
