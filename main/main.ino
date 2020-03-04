#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <UniversalTelegramBot.h>
#include <EEPROM.h>

const char *ssid = "";
const char *password = "";

String ChatID = "";

#define BOTtoken ""
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//String test_photo_url = "https://www.arduino.cc/en/uploads/Trademark/ArduinoCommunityLogo.png";

String oldResult;
String msg;

const char *host = "api.thingspeak.com";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80

const char fingerprint[] PROGMEM = "F9 C2 65 6C F9 Ef 7F 66 8B F7 35 FE 15 EA 82 9F 5F 55 54 3E";

void setup() {
  EEPROM.begin(50);
  client.setInsecure();
  //bot._debug = true;
  delay(1000);
  //Serial.begin(115200);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  //Serial.println("");

  //Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Efendim";

    /*if (text == "/get_test_photo") {
      bot.sendPhoto(chat_id, test_photo_url, "Caption is optional, you may not use photo caption");
      }*/

    if (text == "/komutlar") {
      String welcome = "Merhaba " + from_name + ".\n";
      welcome += "Ben ESP8266 mikrokontrolcüsü üzerinde çalışan ve bilmuh adresinden duyuruları çeken bir botum.\n";
      welcome += "Yazılım: Emre ŞAHİN, Numan KILIÇ, Fatih CÜRE\n";
      welcome += "github.com/ARXINO/BilMuhTelegramBot || Versiyon 1.1 || 04.03.2020\n\n";
      welcome += "Komutlara yanıt en geç 2 dk içinde gelir.\n";
      welcome += "Komutlar:\n";
      welcome += "/komutlar : komutları listeler\n";
      //welcome += "/foto     : duyuruları .jpeg olarak atar\n";
      welcome += "/reset    : sunucuya reset atar\n";
      welcome += "/butunduyurular : bütün duyuruları çeker\n";
      welcome += "/kapat    : mesaj atmayı kapatır\n";
      bot.sendMessage(ChatID, welcome, "");
    }

    if (text == "/reset") {
      ChatID = "";
      String welcome = "Reset atılıyor..\n";
      bot.sendMessage(ChatID, welcome, "");
      ESP.reset();
    }

    if (text == "/butunduyurular") {
      String welcome = "Merhaba " + from_name + ".\n";
      welcome += "Henüz bunun kodu yazılmadı :(\n";
      bot.sendMessage(ChatID, welcome, "");
    }

    if (text == "/kapat") {
      String welcome = "Kapatılıyor..\n";
      bot.sendMessage(ChatID, welcome, "");
      ChatID = "";
    }

    //KOMUTLARIN KODUNU YAZ!

  }
}

void loop() {
  bot.sendMessage("", "OK", "");

  client.setFingerprint(fingerprint);
  //client.setTimeout(15000); // 15 Seconds
  delay(1000);

  //Serial.print("HTTPS Connecting");
  int r = 0; //retry counter
  while ((!client.connect(host, httpsPort)) && (r < 30)) {
    delay(100);
    //Serial.print(".");
    r++;
  }
  /*if (r == 30) {
    //Serial.println("Connection failed");
    }
    else {
    //Serial.println("Connected to web");
    }*/
  String Link;

  //GET Data
  Link = "/apps/thinghttp/send_request?api_key=WTPIX8RFT54YZAZE";// /apps/thinghttp/send_request?api_key=AGK10O9MPFY48XCS

  //Serial.print("requesting URL: ");
  //Serial.println(host + Link);

  client.print(String("GET ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  //Serial.println("request sent");

  //int say = 0;
  String line = "";
  /*Serial.println("reply was:");
    Serial.println("==========");*/
  line = client.readString();
  //Serial.println(line); //Print response
  /*while (httpsClient.available()) {
    line = httpsClient.readStringUntil('\n');  //Read Line by Line
    Serial.println(line); //Print response
    }*/
  //Serial.println("==========");
  //Serial.println("closing connection");


  client.setFingerprint("BB DC 45 2A 07 E3 4A 71 33 40 32 DA BE 81 F7 72 6F 4A 2B 6B");
  int numNewMessages = 1;//bot.getUpdates(bot.last_message_received + 1);
  while (numNewMessages) {
    //Serial.println("got response");
    handleNewMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
  /* HTML PARSE OPERATIONS - BEGIN */
  if (line != oldResult && line != "") {
    //Serial.println("Yeni Duyuru Var!");
    oldResult = line;
    int firstIndex = -1;
    int lastIndex = -1;
    //msg_1
    for (int i = 0; i < 18; i++) {
      firstIndex = line.indexOf('<', firstIndex + 1);
    }
    for (int i = 0; i < 17; i++) {
      lastIndex = line.indexOf('>', lastIndex + 1);
    }
    String temp = "";
    msg = line.substring(lastIndex, firstIndex);
    temp = msg + " duyurusu yayınlanmıştır. Ayrıntısını görmek için https://bilmuh.ege.edu.tr/ adresine bakınız.";
    String oldMsg = "";
    for (byte i = 0; i < 50; i++) {
      oldMsg += char(EEPROM.read(i));
      delay(100);
    }
    //Serial.print("OLD MESSAGE: ");Serial.println(oldMsg);
    //Serial.print("msg.substring(0,50): ");Serial.println(oldMsg);

    if (msg.substring(0, 50) != oldMsg && msg != "") {
      for (byte i = 0; i < 50; i++) {
        EEPROM.write(i, temp.charAt(i));
        delay(100);
      }
      EEPROM.commit(); //returns true when success
      //Serial.println("New Message 1");
      //Serial.println(msg);
      //oldMsg = msg;
      //Serial.print("Durum 1: ");
      bot.sendMessage(ChatID, temp, "");//kontrol ekle while olan
      delay(1000);
      //Serial.println(1);
    }
    //end of msg_1

    //msg_2
    /*
      for (int i = 0; i < 19; i++) {
      firstIndex = line.indexOf('<', firstIndex + 1);
      }
      for (int i = 0; i < 19; i++) {
      lastIndex = line.indexOf('>', lastIndex + 1);
      }
      msg2 = line.substring(lastIndex, firstIndex);
      temp = msg2 + " duyurusu yayınlanmıştır. Ayrıntısını görmek için https://bilmuh.ege.edu.tr/ adresine bakınız.";
      if (msg2 != oldMsg2 && msg2 != msg && msg2 != "") {
      //Serial.println("New Message 2");
      //Serial.println(msg2);
      oldMsg2 = msg2;
      //Serial.print("Durum 2: ");
      bot.sendMessage(ChatID, temp, "");
      delay(1000);
      //Serial.println(1);
      }
    */
    //end of msg_2
  }
  /* HTML PARSE OPERATIONS - END */

  delay(60000);  //GET Data at every 60 seconds
}
