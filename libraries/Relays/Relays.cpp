#include <Relays.h>

Relays::Relays(Node* parent, const char* name) : Node(parent, name) {

}

void Relays::setup() {
	this->readFile();
	this->applySettings();

	for (int i = 0; i < this->nodes->length; i++) {
		KeyValue<Node>* keyValue = this->nodes->keyValues[i];
		Relay* relay = (Relay*) keyValue->value;
		relay->setup();
	}
}

void Relays::loop() {
	for (int i = 0; i < this->nodes->length; i++) {
		KeyValue<Node>* keyValue = this->nodes->keyValues[i];
		Relay* relay = (Relay*) keyValue->value;
		relay->loop();
	}
}

void Relays::applySettings() {

}

void Relays::state(JsonObject& params, JsonObject& response, JsonObject& broadcast) {
	JsonObject object = this->rootIT(response);
	JsonObject state = object.createNestedObject("state");
	this->JSON(state);

	for (int i = 0; i < this->nodes->length; i++) {
		KeyValue<Node>* keyValue = this->nodes->keyValues[i];
		Relay* relay = (Relay*) keyValue->value;
		relay->state(params, response, broadcast);
	}
}

void Relays::JSON(JsonObject& params) {
	params["length"] = this->nodes->length;
}

void Relays::fromJSON(JsonObject& params) {

}

