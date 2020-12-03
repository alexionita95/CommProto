// Including Arduino.h is required for using Serial functions
#include "Arduino.h"
#include <ESP8266WiFiMulti.h>
#include <Logging.h>
#include <CommProto.h>
#define ESP8266

#include <FastLED.h>

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

commproto::sockets::SocketHandle client = std::make_shared<sockets::SocketImpl>();
variable::ContextHandle ctx;
parser::MessageBuilderHandle builder;

#define DATA_PIN 14
#define NUM_LEDS 8

CRGB leds[NUM_LEDS];

void setLED(variable::VariableBaseHandle & var)
{
    bool led_state = false;
    switch (var->getType())
    {
    case variable::ValueType::bool8:
        led_state = std::static_pointer_cast<variable::BoolVariable>(var)->get();
        break;
    default:;
    }
    LOG_DEBUG("toggling led state to %s",led_state?"on":"off");

    if(led_state) {
        FastLED.setBrightness(50);
        leds[7]=CRGB::Blue;
        FastLED.show();
    } else{
        FastLED.setBrightness(50);
        leds[7]=CRGB::Black;
        FastLED.show();
    }
    FastLED.show();
}


// the setup routine runs once when you press reset:
void setup() {
     Serial.begin(115200);
     FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

     for(int i=0;i<NUM_LEDS;++i){
         leds[i]=CRGB::Black;
     }
     FastLED.show();

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
     else
     {
         FastLED.setBrightness(50);
         leds[0]=CRGB::Blue;
         FastLED.show();
         LOG_DEBUG("Making context...");
         ctx = std::make_shared<variable::ContextImpl>(client);

         LOG_DEBUG("Making callback and subscribing...");
         variable::VariableCallback cb = &setLED;
         ctx->subscribe("LED", cb);

         LOG_DEBUG("Making parser delegator...");
         parser::ParserDelegatorHandle delegator = parser::ParserDelegatorFactory::build(ctx);
         LOG_DEBUG("Making builder...");
         builder = std::make_shared<parser::MessageBuilder>(client, delegator);
         FastLED.setBrightness(50);
         leds[1]=CRGB::Blue;
         FastLED.show();
     }
}


// the loop routine runs over and over again forever:


int counter = 0;
int sleepDelay = 5;
int printDelay = 2000;
void loop() {
    counter+=sleepDelay;
    builder->pollAndRead();
    delay(sleepDelay);
    if(counter == printDelay){
        counter = 0;
        LOG_DEBUG("Heap size: %d",ESP.getFreeHeap());
    }
}
