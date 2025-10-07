#include "Ball.h"
#include <SFML/Graphics.hpp>  // Ensure this is included for sf::FloatRect

Ball::Ball(void)
{
	loadDefaultSettings();
}

Ball::Ball(float newRadius)
{
	loadDefaultSettings();

	// Override default settings here:
	setRadius(newRadius);

	// Set the origin again since the radius of the ball changed.
	auto bounds = getLocalBounds();
	setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
}

/**
 * Destructor
 */
Ball::~Ball(void)
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
	if (getStatus() == Ball::Status::LAUNCHING)
	{
		setXVelocity(0);
		setYVelocity(-std::abs(getSpeed()));

		// Then change the ball's status to 'moving'.
		setStatus(Ball::Status::MOVING);
	}

	// If the ball is returning a status of 'moving' or 'bouncing':
	else if (getStatus() == Ball::Status::MOVING || getStatus() == Ball::Status::BOUNCING)
	{
		// Move the ball according to its direction and speed.
		if (getVelocity().x != 0)
			setPosition(sf::Vector2f(
				getPosition().x + (getVelocity().x * dt.asSeconds()),
				getPosition().y
			));

		if (getVelocity().y != 0)
			setPosition(sf::Vector2f(
				getPosition().x,
				getPosition().y + (getVelocity().y * dt.asSeconds())
			));
	}
}

/**
 * Loads default settings.
 */
void Ball::loadDefaultSettings()
{
	this->setSpeed(500);
	this->setCanBounce(true);
	this->setDamage(1);
	this->setLabel("Ball");
	this->setXVelocity(0);
	this->setYVelocity(0);
	this->setRadius(16.f);

	// Center the origin based on local bounds (safe during setup)
	setOrigin(sf::Vector2f(getGlobalBounds().size.x / 2.f, getGlobalBounds().size.y / 2.f));

	// Ensure that the ball will be stuck to the player's paddle at the beginning of each round.
	status = STUCK_TO_PLAYER;

	this->setFillColor(sf::Color::Yellow);
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
 * @param newStatus an unsigned int. (STUCK_TO_PLAYER = 1, LAUNCHING = 2, MOVING = 3, BOUNCING = 4)
 */
void Ball::setStatus(size_t newStatus)
{
	status = newStatus;
}
