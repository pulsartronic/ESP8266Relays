#include <Schedules.h>

Schedules::Schedule::~Schedule() {
	// Never tested, test it and delete this comment
	delete[] this->days;
	for (uint32_t i = 0ul; i < this->events->length; i++)
		delete this->events->buffer[i];
	delete this->events;
}

Schedules::Schedule::Schedule(Schedules* schedules) : schedules(schedules) {
	
}

void Schedules::Schedule::JSON(JsonObject& params) {
	JsonArray days = params.createNestedArray("days");
	for (int i = 0; i < 7; i++) {
		bool wactive = this->days[i];
		days.add(wactive);
	}
	
	JsonArray events = params.createNestedArray("events");
	for (int i = 0; i < this->events->length; i++) {
		Event* event = this->events->get(i);
		JsonObject sdata = events.createNestedObject();
		event->JSON(sdata);
	}
}

void Schedules::Schedule::fromJSON(JsonObject& params) {
	JsonArray days = params["days"].as<JsonArray>();
	int dlength = days.size();
	for (int i = 0; i < min(7, dlength); i++) {
		this->days[i] = days[i];
		Serial.println(String(this->days[i]));
	}
	
	JsonArray events = params["events"].as<JsonArray>();
	int elength = events.size();
	for (int i = 0; i < elength; i++) {
		JsonObject data = events[i].as<JsonObject>();
		Event* event = NULL;
		if (this->events->length <= i) {
			event = new Event();
			this->events->add(event);
		} else {
			event = this->events->get(i);
		}
		event->fromJSON(data);
	}
	
	// you can do it more efficiently, change it
	while (elength < this->events->length) {
		Event* event = this->events->get(elength);
		this->events->removeAt(elength);
		delete event;
	}
}

void Schedules::Schedule::loop() {
	uint32_t ms = millis();
	if (1000u <= (ms - this->lastTic)) {
		this->lastTic = ms;
		time_t timestamp = time(nullptr);
		tm ltm = *localtime(&timestamp);
		if ((60 * 60 * 24) < timestamp) {
			bool wactive = this->days[ltm.tm_wday];
			if (wactive) {
				for (uint32_t i = 0ul; i < this->events->length; i++) {
					Event* event = this->events->get(i);
					if (0 == event->nday) {
						event->updateDay(this->days, 0);
						Serial.println("Updated day: " + String(event->nday) + " tm_mday: " + String(ltm.tm_mday));
					}
					if (event->nday == ltm.tm_mday) {
						uint16_t et = 60u * event->hour + event->minute;
						uint16_t at = 60u * ltm.tm_hour + ltm.tm_min;
						Serial.println("et: " + String(et) + " at: " + String(at));
						if (et <= at) {
							if (et > (at - 1)) {
								this->schedules->onEvent(event);
							}
							event->updateDay(this->days, 1);
							Serial.println("Shoted day: " + String(event->nday) + " tm_mday: " + String(ltm.tm_mday));
						}
					}
				}
			}
		}
	}
}
