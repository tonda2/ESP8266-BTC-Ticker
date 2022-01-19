# ESP8266 BTC Ticker

![device.jpg](https://github.com/tonda2/ESP8266-BTC-Ticker/blob/main/images/device.jpg)

This is a simple device to display the current BTC price. It is made for the <a href="https://www.aliexpress.com/item/1005001505600894.html?spm=a2g0o.productlist.0.0.28bb1605c8fzqE&algo_pvid=1409a5aa-ac15-4291-89f8-88c079f9286b&algo_exp_id=1409a5aa-ac15-4291-89f8-88c079f9286b-0&pdp_ext_f=%7B%22sku_id%22%3A%2212000016385819737%22%7D&pdp_pi=-1%3B7.96%3B-1%3B-1%40salePrice%3BUSD%3Bsearch-mainSearch"> TTGO ESP8266 0.91" OLED board</a> but should run on any ESP with an OLED (not tested).

The code is fairly simple. It uses an Adafruit library to run the screen and the WiFiManager library for easy network setup.

At first boot up, or if the board doesn't find a known Wi-Fi to connect to, the board will create an AP named <b>BTC Ticker</b>, the password being <b>btctothemoon</b>. After connecting to it, a sign up page will pop up, allowing you to connect the ESP to the Wi-Fi of your choice. It should then reboot and start displaying the price.

The <a href="https://coindesk.com/price/bitcoin"> CoinDesk </a> API is used to get the current price of Bitcoin.

/----CURRENT TO-DO LIST----\

1. Model and 3D print an enclosure for the device.
2. <del>Show instructions on screen when setting up WiFi.</del> Done
