#include "ESP8266Relays.h"

/**
 * Contructor. Here's the program entry point
 */
ESP8266Relays::ESP8266Relays() : NetworkNode(NULL, "root") {
	Serial.begin(9600);

	// ////////////////////////////////////////////////////////////////////////////////
	// DEFAULT CONFIGURATION //////////////////////////////////////////////////////////
	// Change these values as needed, most of them can be modified from the web interface
		
	// TODO:: make checks against this
	bool filesystemInitilized = LittleFS.begin();

	this->system = new System(this, "system");
	this->nodes->set(this->system->name, this->system);

	this->wifi = new WIFI(this, "wifi");
	this->nodes->set(this->wifi->name, this->wifi);

	this->relays = new Relays(this, "relays");
	this->nodes->set(this->relays->name, this->relays);
		// //////////////////////////////////////////////////////////////////////
		// RELAYS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		// Add as many relays as your hardware supports
		Relays::Relay* relay_0 = new Relays::Relay(this->relays, "0");
		this->relays->nodes->set(relay_0->name, relay_0);
		relay_0->settings.title = "living room";
		relay_0->settings.pin = 4; // which pin is going to go HIGH/LOW to handle a digital based relay
		relay_0->settings.nc = 0;  // nc = 0 Normally Closed , nc = 1 Normally Open, it is only to correctly show ON/OFF in the interface
		relay_0->touch->pin = 5;   // which pin is going to be read when using a digital component like a capacitive touch
		
		// Relays::Relay* relay_1 = new Relays::Relay(this->relays, "1");
		// this->relays->nodes->set(relay_1->name, relay_1);
		// relay_1->settings.title = "Garage";
		// relay_1->settings.pin = 4; // which pin is going to go HIGH/LOW to handle a digital based relay
		// relay_1->settings.nc = 0;  // nc = 0 Normally Closed , nc = 1 Normally Open, it is only to correctly show ON/OFF in the interface
		// relay_1->touch->pin = 16;   // which pin is going to be read when using a digital component like a capacitive touch
	
	// ////////////////////////////////////////////////////////////////
	// Http and WebSockets ports this app is going to listen
	// these values cannot be changed from the configuration interface
	this->settings.hport = 80;
	this->settings.wport = 3498;

	// Default credentials for the WiFi Access Point
	this->wifi->ap->settings.ssid = "ESP8266 RELAYS " + String(ESP.getChipId());
	this->wifi->ap->settings.pass = "12345678";

	// ////////////////////////////////////////////////////////////////
	// WiFi Client, Maybe you need to set it here
	// this->wifi->client->settings.ssid = "";
	// this->wifi->client->settings.password = "";
	
	// ////////////////////////////////////////////////////////////////
	// NTP, Network Time Protocol
	this->system->ntp->settings.host = "pool.ntp.org";
	this->system->ntp->settings.tz = 341; // TZ_Europe_London

	// ////////////////////////////////////////////////////////////////
	// Default password for the configuration interface
	// SHA256("adminadmin") change "adminadmin" by "user+pass"
	this->key = new byte[32] {
		0xd8, 0x24, 0x94, 0xf0, 0x5d, 0x69, 0x17, 0xba, 
		0x02, 0xf7, 0xaa, 0xa2, 0x96, 0x89, 0xcc, 0xb4,
		0x44, 0xbb, 0x73, 0xf2, 0x03, 0x80, 0x87, 0x6c,
		0xb0, 0x5d, 0x1f, 0x37, 0x53, 0x7b, 0x78, 0x92
	};
}

ESP8266Relays::~ESP8266Relays() {
	delete this->system;
	delete this->wifi;
	delete this->relays;
}

void ESP8266Relays::setup() {
	NetworkNode::setup();
	this->system->setup();
	this->wifi->setup();
	this->relays->setup();
}

void ESP8266Relays::loop() {
	NetworkNode::loop();
	this->system->loop();
	this->wifi->loop();
	this->relays->loop();
}

void ESP8266Relays::html() {
	this->httpServer->send_P(200, "text/html", HTML);
}


