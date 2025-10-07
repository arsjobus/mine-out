#include "Paddle.h"

Paddle::Paddle(void)
{
	loadDefaultSettings();
}

Paddle::Paddle( int newWidth, int newHeight )
{
	loadDefaultSettings();

	// Override default settings here:
	setSize( sf::Vector2f( newWidth, newHeight ) );

	// Set the origin again after setting the size
	setOrigin( sf::Vector2f(getGlobalBounds().size.x / 2, getGlobalBounds().size.y / 2) );
}

/**
* Destructor
*/
Paddle::~Paddle( void )
{
	// Clear the sound buffer
	for (int i = 0; i < this->getRefToSound()->size(); ++i)
		this->getRefToSound()->pop_back();
}

/**
* Updates members of this object.
*/
void Paddle::update() {
	sf::Time dt = deltaClock.restart();

	// Update the position of the player's paddle
	setPosition( sf::Vector2f(getPosition().x + getVelocity().x * dt.asSeconds(), getPosition().y ) );
}

size_t Paddle::getActivePowerUp()
{
	return this->activePowerUp;
}

void Paddle::setActivePowerUp(int activePowerUp)
{
	this->activePowerUp = activePowerUp;
}

size_t Paddle::getShotAccuracy()
{
	return shotAccuracy;
}

void Paddle::setShotAccuracy(int newAccuracy)
{
	shotAccuracy = newAccuracy;
}

size_t Paddle::getStatus()
{
	return status;
}

void Paddle::setStatus( size_t newStatus )
{
	status = newStatus;
}

bool Paddle::getCanMoveLeft()
{
	return canMoveLeft;
}

bool Paddle::getCanMoveRight()
{
	return canMoveRight;
}

void Paddle::setCanMoveLeft( bool newBoolean )
{
	canMoveLeft = newBoolean;
}

void Paddle::setCanMoveRight( bool newBoolean )
{
	canMoveRight = newBoolean;
}

void Paddle::loadDefaultSettings()
{
	sf::Vector2f defaultSize( 64.f, 32.f );
	
	sf::Sound sndHit = sf::Sound(resource.getBufferedSound( 0 ));
		
	this->getRefToSound()->push_back(sndHit);

	this->canMoveLeft = this->canMoveRight = true;

	this->setActivePowerUp( NULL );

	this->setStatus( Paddle::Status::NULL_STATUS );

	this->setSpeed( 500.f );

	this->setXVelocity( NULL );

	this->setYVelocity( NULL );

	// Set the default size
	this->setSize( defaultSize );

	// Set the origin after settings the size
	this->setOrigin( sf::Vector2f(this->getGlobalBounds().size.x / 2, this->getGlobalBounds().size.y / 2) );

	// Set default color to white.
	this->setFillColor( sf::Color::White );

	// Set default texture.
	this->setTexture(&resource.getPaddleTexture(0));

	// The lower the value the more accurate the shot
	this->setShotAccuracy( 5 );
}