#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "bilmuhbot.firebaseio.com"
#define FIREBASE_AUTH ""
#define WIFI_SSID "internetev"
#define WIFI_PASSWORD ""

#define ChatID  ""
#define logChatID  ""
#define BOTtoken ""
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//String test_photo_url = "https://www.arduino.cc/en/uploads/Trademark/ArduinoCommunityLogo.png";

const char *host = "api.thingspeak.com";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80

const char fingerprint[] PROGMEM = "F9 C2 65 6C F9 Ef 7F 66 8B F7 35 FE 15 EA 82 9F 5F 55 54 3E";

String msg = "emre sahin";

void setup() {
  Serial.begin(115200);
  client.setInsecure();
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {

  //client.setFingerprint("BB DC 45 2A 07 E3 4A 71 33 40 32 DA BE 81 F7 72 6F 4A 2B 6B");
  Serial.print("bot durum:"); Serial.println(bot.sendMessage(logChatID, "OK..", ""));
  //client.setFingerprint(fingerprint);
  int8_t r = 0; //retry counter
  while ((!client.connect(host, httpsPort)) && (r < 30)) {
    delay(100);
    //Serial.print(".");
    r++;
  }

  client.print(String("GET ") + "/apps/thinghttp/send_request?api_key=AGK10O9MPFY48XCS" + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(1000);
  String line;
  int firstIndex = -1;
  int lastIndex = -1;
  firstIndex = line.indexOf('<', firstIndex + 1);
  lastIndex = line.indexOf('>', lastIndex + 1);

  for (uint8_t i; i < 20; i++) {
    line = client.readStringUntil('\n');  //Read Line by Line
    //Serial.print(i); Serial.println(line); //Print response
  }

  //msg_1
  for (int i = 0; i < 2; i++) {
    firstIndex = line.indexOf('<', firstIndex + 1);
  }
  for (int i = 0; i < 1; i++) {
    lastIndex = line.indexOf('>', lastIndex + 1);
  }
  msg = line.substring(lastIndex + 1, firstIndex);

  Serial.println("***");
  Serial.println(msg);
  Serial.println("***");
  //Convert special characters
  msg.replace("Ç", "C");
  msg.replace("İ", "I");
  msg.replace("Ü", "U");
  msg.replace("Ğ", "G");
  msg.replace("Ö", "O");
  msg.replace("Ş", "S");
  msg.replace("ı", "i");
  msg.replace("ç", "c");
  msg.replace("ü", "u");
  msg.replace("ğ", "g");
  msg.replace("ö", "o");
  msg.replace("ş", "s");
  msg.replace("⸮", " ");
  /////////////////////
  Serial.println(msg);
  String f_data = Firebase.getString("Duyuru1");
  Serial.println("???");
  Serial.println(f_data);
  Serial.println(msg);
  Serial.println(f_data != msg);
  Serial.println("???");
  if (f_data != "" && msg != "" && f_data != msg) {
    Serial.println(f_data);
    Firebase.setString("Duyuru1", msg);
    if (Firebase.failed()) {
      Serial.print("message failed:"); Serial.println(Firebase.error());
      bot.sendMessage(logChatID, Firebase.error(), "");
    }
    delay(3000);
    Serial.print("bot durum msg:"); Serial.println(bot.sendMessage(ChatID, msg + " duyurusu yayinlanmistir. Ayrıntısını görmek için https://bilmuh.ege.edu.tr/ adresine bakınız.", ""));
    delay(3000);
  }

  delay(300000);
}
