/*
  TSC MoodBoard
*/

#include <WiFi.h>
//#include <NTPClient.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include "credentials.h"
#include <Wire.h>  
#include "SSD1306Wire.h" 

// constants won't change. Used here to set a pin number:
const int ledGreen =  4;
const int ledYellow =  3;
const int ledRed =  15;

const int buttonGreen = 34;
const int buttonYellow = 32;
const int buttonRed = 35;

const int OLED_SDA_1 = 23;
const int OLED_SCL_1 = 22;
const int OLED_SDA_2 = 25;
const int OLED_SCL_2 = 33;
const int OLED_SDA_3 = 26;
const int OLED_SCL_3 = 27;
const char* OLED_TEXT_1     = "The mood is :-)";
const char* OLED_TEXT_2     = "Feeling uneasy :-|";
const char* OLED_TEXT_3     = "Worst day ever :-(";


// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, OLED_SDA_1, OLED_SCL_1);
SSD1306Wire  display2(0x3c, OLED_SDA_2, OLED_SCL_2);
SSD1306Wire  display3(0x3c, OLED_SDA_3, OLED_SCL_3);

void setup() {
  Serial.begin(112500); 
  //initWifi();
  initMood(buttonGreen, ledGreen);
  initMood(buttonYellow, ledYellow);
  initMood(buttonRed, ledRed);
  initDisplays();
  updateDisplays();
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


void initDisplays() {  
  // Initialising the UI will init the display too.
  display.init();
  display2.init();
  display3.init();
  // This will make sure that multiple instances of a display driver
  // running on different ports will work together transparently
  display.setI2cAutoInit(true);
  display2.setI2cAutoInit(true);
  display3.setI2cAutoInit(true);
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display2.flipScreenVertically();
  display2.setFont(ArialMT_Plain_10);
  display2.setTextAlignment(TEXT_ALIGN_LEFT);
  display3.flipScreenVertically();
  display3.setFont(ArialMT_Plain_10);
  display3.setTextAlignment(TEXT_ALIGN_LEFT);
}

void updateDisplays() {
  display.clear();
  display.drawString(0, 0, OLED_TEXT_1);
  display.display();
  display2.clear();
  display2.drawString(0, 0, OLED_TEXT_2);
  display2.display();
  display3.clear();
  display3.drawString(0, 0, OLED_TEXT_3);
  display3.display();
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
    schick(value);;
    Serial.print("'button': ");
    Serial.print(button);
    Serial.print(", 'led': ");
    Serial.print(led);
    Serial.print(", 'value': ");
    Serial.print(value);
    Serial.println("}");
    delay(800);
  } else {
    digitalWrite(led, LOW);
  }
}

void schick(int button) {
  HTTPClient http;
  //https://techtutorialsx.com/2017/05/20/esp32-http-post-requests/
  http.begin(endpoint); //Specify destination for HTTP request
  http.addHeader("Content-Type", "text/plain"); //Specify content-type header

  int httpResponseCode = http.POST("hase"); //Send the actual POST request
  Serial.println(endpoint);
  if (httpResponseCode > 0) {
    String response = http.getString();  //Get the response to the request
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
}
