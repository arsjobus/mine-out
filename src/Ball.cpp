#include "Ball.h"

Ball::Ball( void )
{
	loadDefaultSettings();
}

Ball::Ball( float newRadius )
{
	loadDefaultSettings();

	// Override default settings here:
	setRadius( newRadius );

	// Set the origin again since the radius of the ball changed.
	setOrigin( getGlobalBounds().width / 2, getGlobalBounds().height / 2 );
}

/**
* Destructor
*/
Ball::~Ball( void )
{
}

/**
* Updates members of this object.
*/
void Ball::update()
{
	// Reset delta time at the beginning of each update phase.
	sf::Time dt = deltaClock.restart();

	// If the ball is returning a status of 'launching':
	if ( getStatus() == Ball::Status::LAUNCHING )
	{
		setXVelocity( 0 );
		setYVelocity( -std::abs(getSpeed()) );

		// Then change the balls status to 'moving'.
		setStatus( Ball::Status::MOVING );
	}

	// If the ball is returning a status of 'moving' or 'bouncing':
	else if ( getStatus() == Ball::Status::MOVING || getStatus() == Ball::Status::BOUNCING )
	{
		// Do some fancy math to set the move the ball according to its direction and speed.
		if ( getVelocity().x != 0)
			setPosition(
            getPosition().x + ( ( getVelocity().x ) * dt.asSeconds() ), 
            getPosition().y
			);

		if ( getVelocity().y != 0)
			setPosition(
            getPosition().x, 
            getPosition().y + ( ( getVelocity().y ) * dt.asSeconds() ) 
			);
	}
}

/**
* Loads default settings.
*/
void Ball::loadDefaultSettings()
{
	this->setSpeed( 500 );
	
	this->setCanBounce( true );

	this->setDamage( 1 );

	this->setLabel( "Ball" );

	this->setXVelocity( 0 );

	this->setYVelocity( 0 );

	this->setRadius( 16.f );

	this->setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);

	// Ensure that the ball will be stuck to the player's paddle at the beginning of each round.
	status = STUCK_TO_PLAYER;

	this->setFillColor( sf::Color::Yellow );
}

bool Ball::getCanBounce()
{
	return canBounce;
}

void Ball::setCanBounce(bool canBounce)
{
	this->canBounce = canBounce;
}

/**
* Returns the status of this object.
* @return unsigned int value.
*/
size_t Ball::getStatus()
{
	return status;
}

/**
* Sets the status of this object.
* @param newStatus an unsigned int. ( STUCK_TO_PLAYER = 1, LAUNCHING = 2, MOVING = 3, BOUNCING = 4 )
*/
void Ball::setStatus( size_t newStatus )
{
	status = newStatus;
}
