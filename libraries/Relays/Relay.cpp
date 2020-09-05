#include <Relays.h>

Relays::Relay::Relay(Node* parent, const char*  name) : Node(parent, name) {
	Method* turn = new Method(std::bind(&Relays::Relay::turn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	this->methods->set("turn", turn);
	
	this->schedules = new Schedules(this, "schedules");
	this->nodes->set(this->schedules->name, this->schedules);
	this->schedules->onEvent = std::bind(&Relays::Relay::onEvent, this, std::placeholders::_1);
	
	this->touch = new Relay::Touch();
	this->touch->toggle = std::bind(&Relays::Relay::toggle, this);
}

Relays::Relay::~Relay() {
	delete this->schedules;
	delete this->touch;
}

void Relays::Relay::setup() {
	this->readFile();
	this->applySettings();
	this->schedules->setup();
}

void Relays::Relay::loop() {
	this->touch->loop();
	this->schedules->loop();
}

void Relays::Relay::applySettings() {
	this->touch->applySettings();
	if (0 <= this->settings.pin) {
		pinMode(this->settings.pin, OUTPUT);
	}
}

void Relays::Relay::state(JsonObject& params, JsonObject& response, JsonObject& broadcast) {
	JsonObject object = this->rootIT(response);
	JsonObject state = object.createNestedObject("state");
	this->JSON(state);
	state["hi"] = digitalRead(this->settings.pin);
	this->schedules->state(params, response, broadcast);
}

void Relays::Relay::JSON(JsonObject& params) {
	params["title"] = this->settings.title;
	params["pin"] = this->settings.pin;
	params["nc"] = this->settings.nc;
	params["touch"] = this->touch->pin;
}

void Relays::Relay::fromJSON(JsonObject& params) {
	if (params.containsKey("title")) { this->settings.title = params["title"].as<String>(); }
	if (params.containsKey("pin")) { this->settings.pin = params["pin"]; }
	if (params.containsKey("nc")) { this->settings.nc = params["nc"]; }
	if (params.containsKey("touch")) { this->touch->pin = params["touch"]; }
}

void Relays::Relay::turn(JsonObject& params, JsonObject& response, JsonObject& broadcast) {
	int HI = params["hi"];
	if (0 <= this->settings.pin) {
		digitalWrite(this->settings.pin, HI);
		yield();
		JsonObject object = this->rootIT(broadcast);
		JsonObject state = object.createNestedObject("state");
		this->JSON(state);
		state["hi"] = digitalRead(this->settings.pin);
	}
}

void Relays::Relay::toggle() {
	if (0 <= this->settings.pin) {
		int status = digitalRead(this->settings.pin);
		int TO = status == HIGH ? LOW : HIGH;
		this->set(TO);
	}
}

void Relays::Relay::set(int TO) {
	if (0 <= this->settings.pin) {
		digitalWrite(this->settings.pin, TO);
		yield();
		DynamicJsonDocument rootDocument(256);
		JsonObject command = rootDocument.to<JsonObject>();
		JsonObject object = this->rootIT(command);
		JsonObject state = object.createNestedObject("state");
		state["hi"] = TO;
		state["nc"] = this->settings.nc;
		this->command(command);
	}
}

void Relays::Relay::onEvent(Schedules::Schedule::Event* event) {
	this->set(event->status);
}

