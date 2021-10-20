#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     16 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// API server
const char* host = "api.coindesk.com";

#define LOGO_HEIGHT   19
#define LOGO_WIDTH    16


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

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  //wm.resetSettings();  //uncomment to delete stored credentials for testing purposes

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();                                                     //Clear the display
  display.setTextColor(SSD1306_WHITE);                                      //Set the text colour to white
  display.drawBitmap(48, 0, logo, 32, 32, WHITE);                             //Display bitmap from array
  display.display();
  delay(2000); 

  bool res;
  res = wm.autoConnect("BTC Ticker","btctothemoon"); // create password protected ap

  
    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
}

void loop() {
  // Connect to API
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/v1/bpi/currentprice.json";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
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
  Serial.println();
  Serial.println("closing connection");

  // Process answer
  Serial.println();
  Serial.println("Answer: ");
  Serial.println(answer);

  // Convert to JSON
  String jsonAnswer;
  int jsonIndex;

  for (int i = 0; i < answer.length(); i++) {
    if (answer[i] == '{') {
      jsonIndex = i;
      break;
    }
  }

  // Get JSON data
  jsonAnswer = answer.substring(jsonIndex);
  Serial.println();
  Serial.println("JSON answer: ");
  Serial.println(jsonAnswer);
  jsonAnswer.trim();

  // Get rate as float
  int rateIndex = jsonAnswer.indexOf("rate_float");
  String priceString = jsonAnswer.substring(rateIndex + 12, rateIndex + 18);
  priceString.trim();
  int price = priceString.toInt();

  // Print price
  Serial.println();
  Serial.println("Bitcoin price: ");
  Serial.println(price);


  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Cena BTC je:"));

  display.drawBitmap(98, 0, logo, 32, 32, WHITE); 
  display.display();

  display.setCursor(0,16); 
  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("$"));
  display.setCursor(12,16);
  display.print(price);
/*  display.setCursor(110,24);
  display.setTextSize(1); 
  display.print(F("USD"));
  */
  display.display();
  delay(5000);
}
