#include <DebugM.h>

#define ARDUINOJSON_USE_DOUBLE 1
#include <ArduinoJson.h>
#include <Node.h>
#include <System.h>
#include <Schedules.h>

#ifndef __Relays__
#define __Relays__

class Relays : public Node {
	public:

	class Relay : public Node {
		public:

		class Touch {
			public:
			int pin = -1;
			int status;
			int previous = LOW;
			uint32_t last = 0;
			uint32_t debounce = 500;
			std::function<void(void)> toggle = NULL;

			Touch();
			void setup();
			void loop();
			virtual void applySettings();
		};
	
		class Settings {
			public:
			String title;
			int pin = -1;
			int nc = 0;
			int touch = -1;
		};

		Schedules* schedules = NULL;
		Touch* touch = NULL;
		Settings settings;

		Relay(Node* parent, const char* name);
		virtual ~Relay();
		void setup();
		void loop();

		void toggle();
		void set(int TO);
		void turn(JsonObject& params, JsonObject& response, JsonObject& broadcast);
		void onEvent(Schedules::Schedule::Event* event);

		virtual void applySettings();
		virtual void state(JsonObject& params, JsonObject& response, JsonObject& broadcast);
		virtual void fromJSON(JsonObject& params);
		virtual void JSON(JsonObject& params);
	};

	Relays(Node* parent, const char* name);
	void setup();
	void loop();

	virtual void applySettings();
	virtual void state(JsonObject& params, JsonObject& response, JsonObject& broadcast);
	virtual void fromJSON(JsonObject& params);
	virtual void JSON(JsonObject& params);
};

#endif
