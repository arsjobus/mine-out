#include "PowerUp.h"

PowerUp::~PowerUp(void) { }

void PowerUp::update(sf::Time dt) {
	if (this->getActive()) {
		this->setPosition( sf::Vector2f(this->getPosition().x, this->getPosition().y + this->getSpeed() * dt.asSeconds()) );
	}
}

void PowerUp::loadDefaultSettings() {
	this->setActive( false );
	this->setPowerID( TypeID::IncreasedDamage );
	this->setSpeed( 100.f );
	this->setFillColor(sf::Color::White);
	this->setSize(sf::Vector2f(32, 32));
	this->setOrigin(sf::Vector2f(this->getGlobalBounds().size.x / 2, this->getGlobalBounds().size.y / 2));
	this->setPosition(sf::Vector2f(0,0));
}

void PowerUp::render(Window &window) {
	if ( this->getActive() ) {
		window.draw( *this );
	}
}

bool PowerUp::detectOutOfBounds(Window &window) {
	if (this->getActive() &&
		this->getPosition().y > window.getSize().y) {
		this->setActive( false );
		return true;
	}
	else return false;
}

bool PowerUp::getActive() { return this->active; }
void PowerUp::setActive(bool active) { this->active = active; }
float PowerUp::getFallSpeed() { return this->fallSpeed; }
void PowerUp::setFallSpeed(float fallSpeed) { this->fallSpeed = fallSpeed; }
size_t PowerUp::getPowerID() { return this->powerID; }
void PowerUp::setPowerID(int powerID) { this->powerID = powerID; }
