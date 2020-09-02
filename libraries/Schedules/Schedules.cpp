#include <Schedules.h>


Schedules::~Schedules() {
	// TODO:: free memory
}


Schedules::Schedules(Node* parent, const char* name) : Node(parent, name) {

}

void Schedules::setup() {
	this->readFile();
	this->applySettings();
}

void Schedules::loop() {
	for (int i = 0; i < this->saved->length; i++) {
		Schedule* schedule = this->saved->get(i);
		schedule->loop();
	}
}

void Schedules::applySettings() {
	
}

void Schedules::state(JsonObject& params, JsonObject& response, JsonObject& broadcast) {
	JsonObject object = this->rootIT(response);
	JsonObject state = object.createNestedObject("state");
	this->JSON(state);
}

void Schedules::JSON(JsonObject& params) {
	JsonArray saved = params.createNestedArray("saved");
	for (int i = 0; i < this->saved->length; i++) {
		JsonObject data = saved.createNestedObject();
		Schedule* schedule = this->saved->get(i);
		schedule->JSON(data);
	}
}

void Schedules::fromJSON(JsonObject& params) {
	JsonArray saved = params["saved"];
	int length = saved.size();
	for(int i = 0; i < length; i++) {
		JsonObject sdata = saved[i].as<JsonObject>();
		Schedule* schedule = NULL;
		if (this->saved->length <= i) {
			schedule = new Schedule(this);
			this->saved->add(schedule);
		} else {
			schedule = this->saved->get(i);
		}
		schedule->fromJSON(sdata);
	}
	while (length < this->saved->length) {
		Schedule* schedule = this->saved->get(length);
		this->saved->removeAt(length);
		delete schedule;
	}
}

