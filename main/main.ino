#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <FirebaseArduino.h>

ESP8266WiFiMulti WiFiMulti;
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID "internetev"
#define WIFI_PASSWORD ""

#define ChatID  "-1001126227417"// -226141867  872893196
#define logChatID  "872893196"
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
  Serial.setTimeout(2000);
  client.setInsecure();
  delay(1000);
  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFiMulti.addAP("internetev", "");
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  blink(2);
}

void loop() {
  String payload = "";
  //client.setFingerprint("BB DC 45 2A 07 E3 4A 71 33 40 32 DA BE 81 F7 72 6F 4A 2B 6B");
  Serial.print("bot durum:"); Serial.println(bot.sendMessage(logChatID, "OK..", ""));
  //client.setFingerprint(fingerprint);


  if ((WiFiMulti.run() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    //link: https://api.thingspeak.com/apps/thinghttp/send_request?api_key=45F1HRSG176JVCSF
    //test link: https://api.thingspeak.com/apps/thinghttp/send_request?api_key=AGK10O9MPFY48XCS
    if (https.begin(*client, "https://api.thingspeak.com/apps/thinghttp/send_request?api_key=45F1HRSG176JVCSF")) {  // HTTPS

      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          payload = https.getString();
          //Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }

  }

  //String parse operations
  int firstIndex = -1;
  int lastIndex = -1;
  for (int i = 0; i < 2; i++) {
    firstIndex = payload.indexOf('<', firstIndex + 1);
  }
  lastIndex = payload.indexOf('>', lastIndex + 1);
  msg = payload.substring(lastIndex + 1, firstIndex);

  String f_data = Firebase.getString("Duyuru1");//Fetch data from database
  Serial.println("---");
  Serial.println(f_data);
  Serial.println(msg);
  Serial.println(f_data != msg);
  Serial.println("---");
  if (f_data != "" && msg != "" && f_data != msg) {
    Serial.println(f_data);
    Firebase.setString("Duyuru1", msg);
    if (Firebase.failed()) {
      Serial.print("message failed:"); Serial.println(Firebase.error());
      bot.sendMessage(logChatID, Firebase.error(), "");
    }
    delay(3000);
    Serial.print("bot durum msg:"); Serial.println(bot.sendMessage(ChatID, msg + " duyurusu yayınlanmıştır. Ayrıntısını görmek için https://bilmuh.ege.edu.tr/ adresine bakınız.", ""));
  }
  delay (100);
  WiFi.disconnect();
  Serial.println("sleeping");
  delay (100);
  WiFi.mode (WIFI_OFF);
  WiFi.forceSleepBegin();
  delay (100);
  blink(3);
  delay (100);
  ESP.deepSleep(3600 * 1000000UL, WAKE_RF_DEFAULT); //sleep 3600 x 10^6 micro seconds
  //delay(300000);
}

void blink(int n) {
  static bool first_time = true;
  if (first_time == true) {
    pinMode(LED_BUILTIN, OUTPUT);
    first_time = false;
  }
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i = 0; i < n; ++i) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  }
}
