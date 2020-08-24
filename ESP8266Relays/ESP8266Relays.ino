/*
 * Pulsartronic
 * An Open Source project is a forever Work In Progress, feel free to contribute
 *
 */

#include "ESP8266Relays.h"

ESP8266Relays* esp8266Relays = NULL;

void setup() {
	esp8266Relays = new ESP8266Relays();
	esp8266Relays->setup();
}

void loop() {
	esp8266Relays->loop();
}
