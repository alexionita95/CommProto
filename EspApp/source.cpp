// Including Arduino.h is required for using Serial functions
#include "Arduino.h"
#include <ESP8266WiFiMulti.h>
#include <Logging.h>
#include <CommProto.h>

#ifndef STASSID
#define STASSID "esti nebun?"
#define STAPSK  "01LMS222"
#endif

using namespace commproto;

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.1.2";
const uint16_t port = 4242;
ESP8266WiFiMulti WiFiMulti;
Message okMsg;
commproto::sockets::SocketHandle client = std::make_shared<sockets::SocketImpl>();

#define LED 14

// the setup routine runs once when you press reset:
void setup() {
    Serial.begin(115200);

     // We start by connecting to a WiFi network
     WiFi.mode(WIFI_STA);
     WiFiMulti.addAP(ssid, password);

     Serial.println();
     Serial.println();
     Serial.print("Wait for WiFi... ");

     while (WiFiMulti.run() != WL_CONNECTED) {
       Serial.print(".");
       delay(500);
     }

     Serial.println("");
     Serial.println("WiFi connected");

     delay(500);
     LOG_INFO("Attempting to connect a client socket to %s:%d...",host,port);
     bool init = client->initClient(host, port);
     if (!init) {
         LOG_ERROR("An error occurred while attempting to connect.");
     }
     okMsg.push_back(1);
     pinMode(LED,OUTPUT);
}


// the loop routine runs over and over again forever:

void loop() {
    bool led_state = false;
    int polled = client->pollSocket();
    if(polled !=0 ){
        Message msg;
        int recv = client->receive(msg,polled);
        if(recv != 0 ){
            parser::ByteStream stream(msg);
            led_state = msg[0];
            LOG_INFO("Read LED state: %s", (led_state?"True":"False"));
        }
    }

    if(led_state) {
        digitalWrite(LED, HIGH);
    } else{
        digitalWrite(LED, LOW);
    }
    delay(1000);
    client->sendBytes(okMsg);
}
