#include <Node.h>
#define dtNBR_ALARMS 64
#include <DS.h>

#ifndef __Schedules__
#define __Schedules__

class Schedules : public Node {
	public:
	
	class Schedule {
		public:
		
		class Event {
			public:
			int hour = 0;
			int minute = 0;
			int status = 0;
			
			int nday = 0; // next day
			
			Event();
			void updateDay(bool* days, int offset);
			virtual void fromJSON(JsonObject& params);
			virtual void JSON(JsonObject& params);
		};
		
		bool* days = new bool[7] {false, false, false, false, false, false, false};
		DS::List<Event*>* events = new DS::List<Event*>();
		Schedules* schedules;
		
		virtual ~Schedule();
		Schedule(Schedules* schedules);
		
		void loop();
		virtual void fromJSON(JsonObject& params);
		virtual void JSON(JsonObject& params);
		
		private:
		uint32_t lastTic = 0l;
		int next = 0;
	};
	
	DS::List<Schedule*>* saved = new DS::List<Schedule*>();
	std::function<void(Schedule::Event*)> onEvent = NULL;
	
	Schedules(Node* parent, const char* name);
	virtual ~Schedules();
	void setup();
	void loop();

	virtual void applySettings();
	virtual void state(JsonObject& params, JsonObject& response, JsonObject& broadcast);
	virtual void fromJSON(JsonObject& params);
	virtual void JSON(JsonObject& params);
};
#endif
