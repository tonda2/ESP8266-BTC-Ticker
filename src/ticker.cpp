#include "Arduino.h"

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET 16
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* host = "api.coindesk.com";

const unsigned char logo [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xa0, 0x00, 0x00, 0x01, 0xb0, 0x00,
  0x00, 0x1f, 0x30, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x07, 0x3e, 0x00,
  0x00, 0x0f, 0x1e, 0x00, 0x00, 0x0e, 0x0e, 0x00, 0x00, 0x0e, 0x1e, 0x00, 0x00, 0x0f, 0xfc, 0x00,
  0x00, 0x1e, 0xf8, 0x00, 0x00, 0x1c, 0x3c, 0x00, 0x00, 0x1c, 0x1e, 0x00, 0x00, 0xfc, 0x1e, 0x00,
  0x00, 0xff, 0x3c, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x1b, 0xf8, 0x00, 0x00, 0x1b, 0x00, 0x00,
  0x00, 0x13, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void configModeCallback (WiFiManager *myWiFiManager) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.setCursor(20,4);
  display.println(F("SSID: BTC Ticker"));
  display.setCursor(8,13);
  display.println(F("Psswd: btctothemoon"));
  display.setCursor(5,22);
  display.println(F("Click configure WiFi"));
  display.display();
}

void display_logo(int x, int y){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.drawBitmap(x, y, logo, 32, 32, WHITE);
  display.display();
  delay(20);
}

int get_price(){
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
  }

  // request url
  String url = "/v1/bpi/currentprice.json";

  // send request to server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(100);
  String answer;
  while(client.available()){
    String line = client.readStringUntil('\r');
    answer += line;
  }

  client.stop();
  String jsonAnswer;
  int jsonIndex = 0;

  for (unsigned int i = 0; i < answer.length(); i++) {
    if (answer[i] == '{') {
      jsonIndex = i;
      break;
    }
  }

  // Get JSON data
  jsonAnswer = answer.substring(jsonIndex);
  jsonAnswer.trim();

  // Get price from data
  int rateIndex = jsonAnswer.indexOf("rate_float");
  String priceString = jsonAnswer.substring(rateIndex + 12, rateIndex + 18);
  priceString.trim();
  int price = priceString.toInt();

  return price;
}

void display_price(int input){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("BTC price:"));

  display.drawBitmap(98, 0, logo, 32, 32, WHITE);
  display.display();

  display.setCursor(0,16);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.print(F("$"));
  display.setCursor(12,16);
  display.print(input);
  display.display();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  //wm.resetSettings();  //deletes stored credentials for testing purposes

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display_logo(48,0);
  wm.setAPCallback(configModeCallback);
  bool res;
  res = wm.autoConnect("BTC Ticker","btctothemoon"); // create password protected AP

  if(!res) {
      Serial.println("Failed to connect");
      // ESP.restart();
  }
}

void loop() {
  int price_old = 0, price;

  while (get_price() == 0){}

  while (1){
    price = get_price();

    if (price){
      price_old = price;
    }
    else{
      price = price_old;
    }

    display_price(price);
    delay(10000);
  }
}
