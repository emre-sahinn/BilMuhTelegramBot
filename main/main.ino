#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

WiFiClientSecure client;
#define BOTtoken "Your Bot Token"
String chatID = "Group ChatID";
UniversalTelegramBot bot(BOTtoken, client);

char ssid[] = "Your wifi id";
char password[] = "Wifi password";

String result;
String oldResult;
String msg;
String oldMsg;
String msg2;
String oldMsg2;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.println("Connected!");
  client.setInsecure();
  delay(100);
  //WiFi.disconnect();
  //Serial.println("Disconnected!");
}

//fetchMessage: Fetches data via HTTP GET and returns html code as a string
String fetchMessage(char host[]) {
  HTTPClient http;
  http.begin(host);
  int httpCode = http.GET();
  String payload = "";

  if (httpCode > 0) {
    Serial.printf("HTTP GET ... code: %d\n", httpCode);
    Serial.println(HTTP_CODE_OK);
    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
    }
  } else {
    Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  return payload;
}

void loop() {
  result = fetchMessage("http://api.thingspeak.com/apps/thinghttp/send_request?api_key=WTPIX8RFT54YZAZE");
  delay(1000);
  //Serial.println(result);

  /* HTML PARSE OPERATIONS - BEGIN */
  if (result != oldResult) {
    int firstIndex = -1;
    int lastIndex = -1;

    //msg_1
    for (int i = 0; i < 18; i++) {
      firstIndex = result.indexOf('<', firstIndex + 1);
    }
    for (int i = 0; i < 17; i++) {
      lastIndex = result.indexOf('>', lastIndex + 1);
    }
    msg = result.substring(lastIndex, firstIndex) + " duyurusu yayınlanmıştır. Ayrıntısını görmek için https://bilmuh.ege.edu.tr/ adresine bakınız.";
    if (msg != oldMsg) {
      oldMsg = msg;
      Serial.print("Durum 1: "); Serial.println(bot.sendMessage(chatID, msg, ""));
      delay(1000);
    }
    //end of msg_1

    //msg_2
    for (int i = 0; i < 19; i++) {
      firstIndex = result.indexOf('<', firstIndex + 1);
    }
    for (int i = 0; i < 19; i++) {
      lastIndex = result.indexOf('>', lastIndex + 1);
    }
    msg2 = result.substring(lastIndex, firstIndex) + " duyurusu yayınlanmıştır. Ayrıntısını görmek için https://bilmuh.ege.edu.tr/ adresine bakınız.";
    if (msg2 != oldMsg2 && msg2 != msg) {
      oldMsg2 = msg2;
      Serial.print("Durum 2: "); Serial.println(bot.sendMessage(chatID, msg2, ""));
      delay(1000);
    }
    //end of msg_2

    oldResult = result;
  }
  /* HTML PARSE OPERATIONS - END */
  delay(1800000);
}
