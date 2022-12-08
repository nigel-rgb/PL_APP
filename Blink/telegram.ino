/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/
  
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
  Example based on the Universal Arduino Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/FlashLED/FlashLED.ino
*/

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>  // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Replace with your network credentials
const char *ssid = "MIMI HOME";
const char *password = "Xi292806";



// Initialize Telegram BOT
#define BOTtoken "1845757913:AAFmo-XeUK0dFvQDD6nlDkzvu56VFwroFIY"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "324650269"


WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
int numNewMessages;
char rx_message[250];
String global_chat_id;

const int ledPin = 2;
bool ledState = LOW;
bool get_led_state, led_on, led_off  = false;


struct plant {
  char name[30];
  char type[30];
  int age;
};



// Handle what happens when you receive new messages
void handleNewMessages(void *parameters) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    String from_name = bot.messages[i].from_name;
    if (chat_id != CHAT_ID) {

      String tx_message = "Sorry " + from_name + ".\n";
      tx_message += "You are not allowed to interact with this bot. \n";
      tx_message += "Ask Nigel to authenticate you\n";
      tx_message += "Have a nice day ._.❀ \n";
      bot.sendMessage(chat_id, tx_message, "");
      if (from_name == "Yi Leo") {
        tx_message = "hello ive detected that you are " + from_name + ".\n";
        tx_message += "dont be sad there are many things to look forward too!!\n";
        tx_message += "when life is tough, just click on this link: \n";
        tx_message += "https://t.me/+EmbEw5Y1UMB2B9Lr";
        bot.sendMessage(chat_id, tx_message, "");
      }
      continue;
    }

      
    

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);
    global_chat_id = chat_id;


    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/led_on to turn GPIO ON \n";
      welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/led_on") {led_on = true;}

    if (text == "/led_off") {led_off = true;}

    if (text == "/state") {get_led_state = true;}

    // if(text == "/add_new_plant"){
    //         def_new_plant()
    // }
  }
}

void led_control(void *parameter) {
  while (1) {
    if (get_led_state) {
      if (digitalRead(ledPin)) {
        bot.sendMessage(String(global_chat_id), "LED is ON", "");
        // bot.sendMessageWithInlineKeyboard(const String &chat_id, const String &text, const String &parse_mode, const String &keyboard)
        // Serial.println(chat_id);
      } else {
        bot.sendMessage(String(global_chat_id), "LED is OFF", "");
        // Serial.println(chat_id);
      }
      get_led_state = false;      
    } else if (led_on) {
      bot.sendMessage(String(global_chat_id), "LED state set to ON", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
      led_on = false;

    } else if (led_off) {
      bot.sendMessage(String(global_chat_id), "LED state set to OFF", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
      led_off = false;
    }
  }
}


void listen_new_message(void *parameters) {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      // handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}


void setup() {
  Serial.begin(115200);

#ifdef ESP8266
  configTime(0, 0, "pool.ntp.org");  // get UTC time via NTP
  client.setTrustAnchors(&cert);     // Add root certificate for api.telegram.org
#endif

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
#ifdef ESP32
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
#endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  xTaskCreatePinnedToCore(
    led_control,
    "led control",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

  xTaskCreatePinnedToCore(
    listen_new_message,
    "listen_new_message",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

  xTaskCreatePinnedToCore(
    handleNewMessages,
    "handleNewMessages",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

  vTaskDelete(NULL);
}



void loop() {
}