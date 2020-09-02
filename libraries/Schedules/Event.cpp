#include <Schedules.h>


Schedules::Schedule::Event::Event() {
	
}

void Schedules::Schedule::Event::JSON(JsonObject& params) {
	params["hour"] = this->hour;
	params["minute"] = this->minute;
	params["status"] = this->status;
}

void Schedules::Schedule::Event::fromJSON(JsonObject& params) {
	this->hour = params["hour"];
	this->minute = params["minute"];
	this->status = params["status"];
	this->nday = 0;
}

void Schedules::Schedule::Event::updateDay(bool* days, int offset) {
	time_t ltimestamp = time(nullptr);
	tm* lltm = localtime(&ltimestamp);

	int total = offset;
	for (int j = 0; j < 7; j++) {
		bool nwactive = days[(lltm->tm_wday + j) % 7];
		if (!nwactive) {
			total += 1;
		} else {
			break;
		}
	}
	time_t timestamp = ltimestamp + total * (60 * 60 * 24);
	tm* ltm = localtime(&timestamp);
	this->nday = ltm->tm_mday;
}

