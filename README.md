# BilMuh Telegram Bot

<img src="https://user-images.githubusercontent.com/30238276/83972942-cc80de80-a8eb-11ea-9541-ea2e76cdec73.png" width="20%" height="20%"> <img src="https://user-images.githubusercontent.com/30238276/83972947-cee33880-a8eb-11ea-815a-e5a11b83577a.png" width="20%" height="20%"> <img src="https://user-images.githubusercontent.com/30238276/83972944-ce4aa200-a8eb-11ea-963b-1d82e6a3c7eb.png" width="30%" height="30%">
<img src="https://user-images.githubusercontent.com/30238276/83972946-ce4aa200-a8eb-11ea-9bf7-94d3c5d77b12.png" width="20%" height="20%">

Some of us are too lazy to check our department's website to see the important announcements. To overcome this problem we created the BilMuh Telegram bot. Basically, Telegram bot runs on the ESP8266 development board and fetches pure HTML code at our department's announcement scrollbar using ThingSpeak's ThingHTTP (We preferred ESP8266 by NodeMCU because it is a cost-effective and highly integrated Wi-Fi MCU). After getting data, our C algorithm makes HTTP GET request to ThingHTTP and fetches all data available. Then, comparing the new data with old data using Firebase Realtime Database. If new data differs from the old one, then make an HTTP PUT request to Firebase and replace the current data to new data. At the end of this process, we put ESP8266's MCU to deep sleep mode for power efficiency.

## Goal

We don't need to check our department website every day anymore. We push a Telegram notification to more than 200 students in a single cycle. This automation makes our life carefree as we don't miss any important notification. For electricity, ESP8266 sleeps for 99% of the day, so don't worry about the electricity bill.

## Reliability

Our current system has been working for 3 months without any issue.

<img src="https://user-images.githubusercontent.com/30238276/83168267-e6ffce80-a119-11ea-9330-3a7d18797741.png" width="60%" height="60%">

## Getting Started

These instructions will get you a copy of the project up and running on your development board: <br>
Step 1) Download Arduino IDE<br>
Step 2) Download ESP8266 Core for IDE<br>
Step 3) Create an account on Firebase<br>
Step 4) Create an account on ThingSpeak<br>
Step 5) Download Universal Telegram Bot Library<br>
Step 6) Compile the given code<br>
Step 7) Connect a jumper cable between D0 and RST pins.<br>
Step 8) Plug some high-quality 5V adaptor to ESP8266(Raspberry Pi adaptor works great)<br>
<br>
That's all! While you are sleeping, ESP8266 will work at 1-hour intervals.<br>
<br>

<img src="https://user-images.githubusercontent.com/30238276/83973381-b7597f00-a8ee-11ea-8b77-40ee1828e2be.jpg" width="60%" height="60%">

### Prerequisites

1 x Breadboard
1 x Jumper Cable
1 x 5V 2A adaptor
1 x ESP8266
1 x USB Cable (to upload the code)
2 x Heatsink (Optional)

## Built With

* [Arduino](https://www.arduino.cc/) - Arduino IDE
* [ThingSpeak](https://thingspeak.com/) - ThingSpeak
* [Firebase](https://firebase.google.com/) - Firebase
* [Telegram](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot) - Universal Telegram Bot Library
* [ESP](https://github.com/esp8266/Arduino) - Arduino ESP8266 Core
* [Firebase Lib](https://github.com/FirebaseExtended/firebase-arduino) - Arduino Firebase library
