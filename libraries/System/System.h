#include <DebugM.h>

#include <SystemClock.h>
#include <Node.h>
#include <ESP8266httpUpdate.h>
#include <FS.h>
#include <sntp.h>

#ifndef __System__
#define __System__

#define VERSION 1

class Data {
	public:
	class Packet {
		public:
		class Handler {
			public:
			virtual void onPacket(Data::Packet* packet) = 0;
		};
		uint8_t* buffer = NULL;
		uint16_t size = 0u;
		Packet(uint16_t size);
		virtual ~Packet();
	};
};

class System : public Node {
	public:

	class Pins {
		public:
		static int VALUE[];
		static int length;
	};

	class Settings {
		public:
		uint16_t version = 1u;
		String vurl = "";
	};

	class NTP : public Node {
		public:
		class Settings {
			public:
			String host = "nl.pool.ntp.org";
			int8_t tz = (int8_t) 0;
		};

		Settings settings;

		NTP(Node* parent, const char* name);
		void setup();
		void loop();

		virtual void getPing(JsonObject& response);
		virtual void state(JsonObject& params, JsonObject& response, JsonObject& broadcast);
		virtual void fromJSON(JsonObject& params);
		virtual void JSON(JsonObject& params);
	};

	class ESPS : public Node {
		public:
		ESPS(Node* parent, const char* name);
		virtual ~ESPS();
		void setup();
		void loop();
		virtual void getPing(JsonObject& response);
		virtual void state(JsonObject& params, JsonObject& response, JsonObject& broadcast);
	};

	System::NTP* ntp = NULL;
	System::ESPS* espSystem = NULL;
	Settings settings;

	System(Node* parent, const char* name);
	virtual ~System();
	void setup();
	void loop();
	void upgrade(JsonObject& params, JsonObject& response, JsonObject& broadcast);
	virtual void state(JsonObject& params, JsonObject& response, JsonObject& broadcast);
};

#endif
