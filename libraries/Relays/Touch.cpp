#include <Relays.h>

Relays::Relay::Touch::Touch() {
	
}

void Relays::Relay::Touch::setup() {

}

void Relays::Relay::Touch::applySettings() {
	if (0 <= this->pin) {
		pinMode(this->pin, INPUT);
	}
}

void Relays::Relay::Touch::loop() {
	if (0 <= this->pin) {
		this->status = digitalRead(this->pin);
		if (this->status == HIGH && previous == LOW && (millis() - last) > debounce) {
			this->last = millis();
			this->toggle();
		}
		this->previous = this->status;
	}
}

