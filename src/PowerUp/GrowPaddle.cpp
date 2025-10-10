#include "GrowPaddle.h"

GrowPaddle::GrowPaddle(void) { this->loadDefaultSettings(); }
GrowPaddle::~GrowPaddle(void) { }

void GrowPaddle::update(sf::Time dt) {
	PowerUp::update(dt);
	// Override PowerUp updates here:
}

void GrowPaddle::loadDefaultSettings() {
	PowerUp::loadDefaultSettings();
	// Override PowerUp default settings here:
	this->setLabel("power-up");
	this->setPowerID( TypeID::GrowPaddle );
	this->setTexture( &resource.getPowerupTexture( 1 ) );
}

void GrowPaddle::render(Window &window) {
	PowerUp::render(window);
	// Make additional renders here:
}