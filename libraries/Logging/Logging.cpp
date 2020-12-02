#include "Logging.h"
#if defined(ESP32) || defined(ESP8266)
#include "Arduino.h"
#endif
#include <stdio.h>

void log(const char * format, ...)
{
#if defined(ESP32) || defined(ESP8266)
    static char buf[255];
    int printed = 0;
    va_list args;
    va_start(args, format);
    printed = vsprintf(buf,format, args);
    va_end(args);
    Serial.write(buf,printed);
    Serial.println();
#else
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);   
#endif
}
