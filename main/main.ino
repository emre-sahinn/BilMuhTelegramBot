#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <UniversalTelegramBot.h>

const char *ssid = "";
const char *password = "";

// Initialize Telegram BOT
#define BOTtoken "1141708876:AAEzVcWR0zKuGyDAFyiG2bQROV5u8u4fGxU"
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

String test_photo_url = "https://www.arduino.cc/en/uploads/Trademark/ArduinoCommunityLogo.png";

String oldResult;
String msg;
String oldMsg;
String msg2;
String oldMsg2;


//Web address to read/write from
const char *host = "api.thingspeak.com";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80

const char fingerprint[] PROGMEM = "F9 C2 65 6C F9 Ef 7F 66 8B F7 35 FE 15 EA 82 9F 5F 55 54 3E";
//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  client.setInsecure();
  bot._debug = true;
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Efendim";

    /*if (text == "/get_test_photo") {
      bot.sendPhoto(chat_id, test_photo_url, "Caption is optional, you may not use photo caption");
      }*/

    if (text == "/baslat") {
      String welcome = "Merhaba" + from_name + ".\n";
      welcome += "Ben ESP8266 mikrokontrolcüsü üzerinde çalışan ve bilmuh adresinden duyuruları çeken bir botum :)\n";
      welcome += "Yazılım: Emre ŞAHİN, Numan KILIÇ, Fatih Cüre\n";
      welcome += "github.com/ARXINO/BilMuhTelegramBot || Versiyon 1.0 || 03.03.2020\n\n";
      welcome += "Komutlar:\n";
      welcome += "/komutlar : komutları listeler\n";
      welcome += "/reset    : sunucuya reset atar(sunucu çökerse işe yarayabilir)\n";
      welcome += "/butunduyurular : bütün duyuruları çeker\n";
      bot.sendMessage(chat_id, welcome, "");
    }

  //KOMUTLARIN KODUNU YAZ!
    
  }
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  Serial.println(host);

  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  client.setFingerprint(fingerprint);
  //client.setTimeout(15000); // 15 Seconds
  delay(1000);

  Serial.print("HTTPS Connecting");
  int r = 0; //retry counter
  while ((!client.connect(host, httpsPort)) && (r < 30)) {
    delay(100);
    Serial.print(".");
    r++;
  }
  if (r == 30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to web");
  }
  String ADCData, getData, Link;

  //GET Data
  Link = "/apps/thinghttp/send_request?api_key=AGK10O9MPFY48XCS";

  Serial.print("requesting URL: ");
  Serial.println(host + Link);

  client.print(String("GET ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  Serial.println("reply was:");
  Serial.println("==========");
  String line = client.readString();
  Serial.println(line); //Print response
  /*while (httpsClient.available()) {
    line = httpsClient.readStringUntil('\n');  //Read Line by Line
    Serial.println(line); //Print response
    }*/
  Serial.println("==========");
  Serial.println("closing connection");


  client.setFingerprint("BB DC 45 2A 07 E3 4A 71 33 40 32 DA BE 81 F7 72 6F 4A 2B 6B");
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  while (numNewMessages) {
    Serial.println("got response");
    handleNewMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
  /* HTML PARSE OPERATIONS - BEGIN */
  if (line != oldResult && line != "") {
    Serial.println("Yeni Duyuru Var!");
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
    if (msg != oldMsg && msg != "") {
      Serial.println("New Message 1");
      Serial.println(msg);
      oldMsg = msg;
      Serial.print("Durum 1: ");
      bot.sendMessage("872893196", temp, "");//kontrol ekle while olan
      delay(1000);
      Serial.println(1);
    }
    //end of msg_1

    //msg_2
    for (int i = 0; i < 19; i++) {
      firstIndex = line.indexOf('<', firstIndex + 1);
    }
    for (int i = 0; i < 19; i++) {
      lastIndex = line.indexOf('>', lastIndex + 1);
    }
    msg2 = line.substring(lastIndex, firstIndex);
    temp = msg2 + " duyurusu yayınlanmıştır. Ayrıntısını görmek için https://bilmuh.ege.edu.tr/ adresine bakınız.";
    if (msg2 != oldMsg2 && msg2 != msg && msg2 != "") {
      Serial.println("New Message 2");
      Serial.println(msg2);
      oldMsg2 = msg2;
      Serial.print("Durum 2: ");
      bot.sendMessage("872893196", temp, "");
      delay(1000);
      Serial.println(1);
    }
    //end of msg_2
  }
  /* HTML PARSE OPERATIONS - END */

  delay(2000);  //GET Data at every 2 seconds
}
