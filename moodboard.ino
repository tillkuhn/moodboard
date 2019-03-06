/*
  TSC MoodBoard
*/

#include <WiFi.h>
//#include <NTPClient.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include "credentials.h"

// constants won't change. Used here to set a pin number:
const int ledGreen =  4;
const int ledYellow =  3;
const int ledRed =  15;

const int buttonGreen = 34;
const int buttonYellow = 32;
const int buttonRed = 35;

int requestId = 0;

void setup() {
  Serial.begin(112500);
  initWifi();
  initMood(buttonGreen, ledGreen);
  initMood(buttonYellow, ledYellow);
  initMood(buttonRed, ledRed);
}

void loop() {
  mach(buttonGreen, ledGreen, 1);
  mach(buttonYellow, ledYellow, 0);
  mach(buttonRed, ledRed, -1);
}

void initMood(int button, int led) {
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
}

void initWifi() {
  delay(2000);   //Delay needed before calling the WiFi.begin
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void mach(int button, int led, int value) {
  int pressed = digitalRead(button);
  if (pressed) {
    digitalWrite(led, HIGH);
    schick(button,led,value);;
    Serial.print("'button': ");
    Serial.print(button);
    Serial.print(", 'led': ");
    Serial.print(led);
    Serial.print(", 'value': ");
    Serial.print(value);
    Serial.println("");
    delay(800);
  } else {
    digitalWrite(led, LOW);
  }
}

void schick(int button, int led, int value) {
  HTTPClient http;
  requestId++;
  //https://techtutorialsx.com/2017/05/20/esp32-http-post-requests/
  http.begin(endpoint); //Specify destination for HTTP request
  http.addHeader("Content-Type", "application/json"); //Specify content-type header
  http.addHeader("X-Request-Id", String(requestId)); //Specify content-type header
  String json = String("{\"requestId\": ") + requestId+ String(", \"value\": ") + value +  String(", \"button\": ") + button + String("}") ;
  int httpResponseCode = http.POST(json); //Send the actual POST request
  Serial.print(endpoint);
  Serial.print(" JSON: ");
  Serial.println(json);
  if (httpResponseCode > 0) {
    String response = http.getString();  //Get the response to the request
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
}
