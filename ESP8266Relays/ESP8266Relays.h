#ifndef __ESP8266Relays__
#define __ESP8266Relays__

#define ARDUINOJSON_USE_DOUBLE 1
#include <ArduinoJson.h>
#include <NetworkNode.h>
#include <SystemClock.h>
#include <WIFI.h>
#include <System.h>
//#include <RS232.h>
#include <Relays.h>

#include "index.html.h"

class ESP8266Relays : public NetworkNode {
	public:
	WIFI* wifi = NULL;
	System* system = NULL;
	//RS232* serial = NULL;
	Relays* relays = NULL;

	ESP8266Relays();
	virtual ~ESP8266Relays();
	void loop();
	void setup();
	virtual void html();
};

#endif
