#include "ThingsBoard.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>  /*version 6.16.1*/
//#include <WiFi.h>
#include <PZEM004Tv30.h>

#define WIFI_AP             "iPhone.."
#define WIFI_PASSWORD       "ttuk020142"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#define TOKEN               "Jqy99X6uSux6XxtvMNT5"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();
}

void loop() {
  delay(1000);

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }

  Serial.println("Sending data...");

  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details

  float PZEMVoltage = random(450, 1450);
  float PZEMCurrent = random(100, 1000);
  float PZEMPower = random(100, 1000);
  float PZEMEnergy = random(100, 1000);
  float PZEMFrequency = random(100, 1000);
  float PZEMPF = random(100, 1000);
  PZEMVoltage = PZEMVoltage * 0.1;
  PZEMCurrent = PZEMCurrent * 0.1;
  PZEMPower = PZEMPower * 0.1;
  PZEMEnergy = PZEMEnergy * 0.1;
  PZEMFrequency = PZEMFrequency * 0.1;
  PZEMPF = PZEMPF * 0.1;
  //-------------------------------------------
  StaticJsonDocument<1000> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();

  root["Voltage1"] = PZEMVoltage;
  root["Current1"] = PZEMCurrent;
  root["Power1"] = PZEMPower;
  root["Energy1"] = PZEMEnergy;
  root["Frequency1"] = PZEMFrequency;
  root["PF1"] = PZEMPF;

  String Buffer = "";
  serializeJson(root, Buffer);
  Serial.println();
  Serial.println(Buffer);
  Serial.println(Buffer.length () + 1);
  char bufferIn[Buffer.length () + 1 ];
  Buffer.toCharArray(bufferIn, Buffer.length () + 1);
  tb.sendTelemetryJson(bufferIn);
  //-----------------------------------------------
  tb.loop();
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
