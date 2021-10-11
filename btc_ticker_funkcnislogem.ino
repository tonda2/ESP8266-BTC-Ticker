#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     16 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid     = "ssid";
const char* password = "passw";

// API server
const char* host = "api.coindesk.com";

#define LOGO_HEIGHT   19
#define LOGO_WIDTH    16

const unsigned char logo_bmp [] PROGMEM={
  B00000000, B00000000,
  B00001101, B10000000,
  B00001101, B10000000,
  B01111111, B11100000,
  B01111111, B11110000,
  B00011000, B01111000,
  B00011000, B00111000,
  B00011000, B00111000,
  B00011000, B01110000,
  B00011111, B11110000,
  B00011000, B01111000,
  B00011000, B00111000,
  B00011000, B00111000,
  B00011000, B01111000,
  B01111111, B11110000,
  B01111111, B11000000,
  B00011011, B00000000, 
  B00011011, B00000000,
  B00000000, B00000000,
  B00000000, B00000000
  };

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  display.clearDisplay();
  delay(10);
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

  display.drawBitmap(98,6,logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
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
