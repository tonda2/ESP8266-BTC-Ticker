# ESP8266 BTC Ticker

This is a simple device to display the current BTC price. It is made for the TTGO ESP8266 0.91" OLED board but should run on any ESP with an OLED (not tested).

The code is fairly simple. It uses an Adafruit library to run the screen and the WiFiManager library for easy network setup.

At first boot up, or if the board doesn't find a known Wi-Fi to connect to, the board will create an AP named <b>BTC Ticker</b>, the password being <b>btctothemoon</b>. After connecting to it, a sign up page will pop up, allowing you to connect the ESP to the Wi-Fi of your choice. It should then reboot and start displaying the price.

The <a href="https://coindesk.com/price/bitcoin"> CoinDesk </a> API is used to get the current price of Bitcoin.

/----CURRENT TO-DO LIST----\

1. Model and 3D print an enclosure for the device.
2. Show instructions on screen when setting up WiFi.
3. Add OTA update functionality.
