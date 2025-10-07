#include "Paddle.h"

Paddle::Paddle(void) {
	loadDefaultSettings();
}

Paddle::Paddle( int newWidth, int newHeight ) {
	loadDefaultSettings();

	// Override default settings here:
	setSize( sf::Vector2f( newWidth, newHeight ) );
	setOrigin( sf::Vector2f(getGlobalBounds().size.x / 2, getGlobalBounds().size.y / 2) ); // Set the origin again after setting the size
}

/**
* Destructor
*/
Paddle::~Paddle( void ) {
	for (int i = 0; i < this->getRefToSound()->size(); ++i)
		this->getRefToSound()->pop_back(); // Clear the sound buffer
}

/**
* Updates members of this object.
*/
void Paddle::update() {
	sf::Time dt = deltaClock.restart();
	setPosition( sf::Vector2f(getPosition().x + getVelocity().x * dt.asSeconds(), getPosition().y ) ); // Update the position of the player's paddle
}

size_t Paddle::getActivePowerUp() { return this->activePowerUp; }
void Paddle::setActivePowerUp(int activePowerUp) { this->activePowerUp = activePowerUp; }
size_t Paddle::getShotAccuracy() { return shotAccuracy; }
void Paddle::setShotAccuracy(int newAccuracy) { shotAccuracy = newAccuracy; }
size_t Paddle::getStatus() { return status; }
void Paddle::setStatus( size_t newStatus ) { status = newStatus; }
bool Paddle::getCanMoveLeft() { return canMoveLeft; }
bool Paddle::getCanMoveRight() { return canMoveRight; }
void Paddle::setCanMoveLeft( bool newBoolean ) { canMoveLeft = newBoolean; }
void Paddle::setCanMoveRight( bool newBoolean ) { canMoveRight = newBoolean; }

void Paddle::loadDefaultSettings() {
	sf::Vector2f defaultSize( 64.f, 32.f );
	sf::Sound sndHit = sf::Sound(resource.getBufferedSound( 0 ));
	this->getRefToSound()->push_back(sndHit);
	this->canMoveLeft = this->canMoveRight = true;
	this->setActivePowerUp( 0 );
	this->setStatus( Paddle::Status::NULL_STATUS );
	this->setSpeed( 500.f );
	this->setXVelocity( 0.f );
	this->setYVelocity( 0.f );
	this->setSize( defaultSize ); // Set the default size
	// Set the origin after settings the size
	this->setOrigin( sf::Vector2f(this->getGlobalBounds().size.x / 2, this->getGlobalBounds().size.y / 2) );
	this->setFillColor( sf::Color::White ); // Set default color to white.
	this->setTexture( &resource.getPaddleTexture(0) ); // Set default texture.
	this->setShotAccuracy( 5 ); // The lower the value the more accurate the shot
}