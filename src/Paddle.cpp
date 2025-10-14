#include "Paddle.h"

Paddle::Paddle(void) { loadDefaultSettings(); }
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
void Paddle::update(sf::Time dt) {
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

bool Paddle::isCollisionDetected(GameObject *otherGameObject) {
	if (otherGameObject->getLabel() == "panel-l")  return checkCollisionWithPanelL(otherGameObject);
	if (otherGameObject->getLabel() == "panel-r")  return checkCollisionWithPanelR(otherGameObject);
	if (otherGameObject->getLabel() == "power-up") return checkCollisionWithPowerUp(otherGameObject);
	return false;
}

bool Paddle::checkCollisionWithPanelL(GameObject *otherGameObject) {
	sf::RectangleShape *otherRectShape = dynamic_cast<sf::RectangleShape*>(otherGameObject);
	if (getGlobalBounds().findIntersection( otherRectShape->getGlobalBounds() )) {
		setCanMoveLeft( false );
		setXVelocity( 0.f );
		return true;
	} else {
		setCanMoveLeft( true );
		return false;
	}
}

bool Paddle::checkCollisionWithPanelR(GameObject *otherGameObject) {
    sf::RectangleShape *otherRectShape = dynamic_cast<sf::RectangleShape*>(otherGameObject);
	if (getGlobalBounds().findIntersection( otherRectShape->getGlobalBounds() )) {
		setCanMoveRight( false );
		setXVelocity( 0.f );
		return true;
	} else {
		setCanMoveRight( true );
		return false; 
	}
}

bool Paddle::checkCollisionWithPowerUp(GameObject *otherGameObject) {
	sf::RectangleShape *otherRectShape = dynamic_cast<sf::RectangleShape*>(otherGameObject);
	PowerUp *powerup = dynamic_cast<PowerUp*>(otherGameObject);
	if (getGlobalBounds().findIntersection( otherRectShape->getGlobalBounds() )) {
		powerup->setActive( false );
		setActivePowerUp( powerup->getPowerID() );
		return true;
	} return false;
}

void Paddle::loadDefaultSettings() {
	sf::Vector2f defaultSize( 64.f, 32.f );
	sf::Sound sndHit = sf::Sound(resource.getBufferedSound( 0 ));
	this->getRefToSound()->push_back(sndHit);
	this->canMoveLeft = this->canMoveRight = true;
	this->setActivePowerUp( 0 );
	this->setLabel( "player" );
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

void Paddle::resizeSmall() {
	this->setSize( sf::Vector2f(96.f, 16.f) );
	this->setTexture( &resource.getPaddleTexture(0) );
	this->setTextureRect(sf::Rect<int>(
		sf::Vector2i(0, 0),
		sf::Vector2i(96, 16)
	));
	this->setOrigin( sf::Vector2f(this->getLocalBounds().size.x / 2, this->getLocalBounds().size.y / 2) );
}

void Paddle::resizeLarge(const GameObject* panelL, const GameObject* panelR) {
	this->setSize( sf::Vector2f(192.f, 16.f) );
	this->setTexture( &resource.getPaddleTexture(1) );
	this->setTextureRect(sf::Rect<int>(
		sf::Vector2i(0, 0),
		sf::Vector2i(192, 16)
	));
	this->setOrigin( sf::Vector2f(this->getLocalBounds().size.x / 2, this->getLocalBounds().size.y / 2) );

	// Cast to sf::RectangleShape to access position and size
	const sf::RectangleShape* leftPanelRect = dynamic_cast<const sf::RectangleShape*>(panelL);
	const sf::RectangleShape* rightPanelRect = dynamic_cast<const sf::RectangleShape*>(panelR);
	if (!leftPanelRect || !rightPanelRect) return; // Safety check

	float leftPanelEdge = leftPanelRect->getPosition().x + leftPanelRect->getSize().x / 2.f;
	float rightPanelEdge = rightPanelRect->getPosition().x - rightPanelRect->getSize().x / 2.f;
	float halfPaddle = getSize().x / 2.f;
	float paddleY = getPosition().y;
	float newX = getPosition().x;
	if (newX - halfPaddle < leftPanelEdge) {
		newX = leftPanelEdge + halfPaddle;
	}
	if (newX + halfPaddle > rightPanelEdge) {
		newX = rightPanelEdge - halfPaddle;
	}
	setPosition(sf::Vector2f(newX, paddleY));
}