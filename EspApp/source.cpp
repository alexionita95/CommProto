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
variable::ContextHandle ctx;
parser::MessageBuilderHandle builder;
#define LED 14

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
        digitalWrite(LED, HIGH);
    } else{
        digitalWrite(LED, LOW);
    }
}


// the setup routine runs once when you press reset:
void setup() {
     Serial.begin(115200);


     Serial.println("Little bytestream test...");

     std::string testIn = "Test";
     parser::ByteStream stream1;
     stream1.write(testIn);
     stream1.write(100u);
     stream1.write(true);

     parser::ByteStream stream2(stream1.getStream());

    std::string strTest;
    uint32_t intTest = 0;
    bool boolTest= false;

    stream2.read(strTest);
    stream2.read(intTest);
    stream2.read(boolTest);

    Serial.println("Did it work?");

    Serial.print("String: ");
    Serial.print(strTest.c_str());
    Serial.print(" Int: ");
    Serial.print(intTest);
    Serial.print(" Bool: ");
    Serial.print(boolTest);
    Serial.println();

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
     ctx = std::make_shared<variable::ContextImpl>(client);

     variable::VariableCallback cb = &setLED;
     ctx->subscribe(0, cb);

     parser::ParserDelegatorHandle delegator = parser::ParserDelegatorFactory::build(ctx);
     builder = std::make_shared<parser::MessageBuilder>(client, delegator);
}


// the loop routine runs over and over again forever:


int counter = 0;
int sleepDelay = 5;
int printDelay = 2000;
void loop() {
    counter+=sleepDelay;
    builder->pollAndRead();
    delay(sleepDelay);
    client->sendBytes(okMsg);
    if(counter == printDelay){
        counter = 0;
        LOG_DEBUG("Heap size: %d",ESP.getFreeHeap());
    }
}
