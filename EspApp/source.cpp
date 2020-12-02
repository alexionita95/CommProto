// Including Arduino.h is required for using Serial functions
#include "Arduino.h"

#include <Logging.h>

#define LED 14

// the setup routine runs once when you press reset:
void setup() {
	// initialize serial communication at 9600 bits per second:
	Serial.begin(9600);

	// print out hello world
	Serial.println("Hello World");

	// Setup to blink the inbuilt LED
        pinMode(LED, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
	// Blink the inbuilt LED
  LOG_INFO("This is a message from the logger.");

  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(4000);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  LOG_ERROR("IT'S ALL SHUTTING DOWN, NO MORE LIGHT ON PIN %d",LED);
}
