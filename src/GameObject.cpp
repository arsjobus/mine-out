#include "GameObject.h"

float GameObject::getSpeed()
{
        return speed;
}

void GameObject::playSound(int index)
{
	sound[index].play();
}

std::vector<sf::Sound> *GameObject::getRefToSound()
{
	return &this->sound;
}

void GameObject::setSpeed(float speed)
{
    this->speed = speed;
}

int GameObject::getDamage()
{
	return this->damage;
}

void GameObject::setDamage(int damage)
{
	this->damage = damage;
}

float GameObject::getDirection()
{
        return direction;
}

void GameObject::setDirection(float newDirection)
{
        direction = newDirection;
}

std::string GameObject::getLabel()
{
	return this->label;
}

void GameObject::setLabel(const char *label)
{
	this->label = label;
}

sf::Vector2f GameObject::getVelocity()
{
	return velocity;
}

void GameObject::setXVelocity(float x)
{
	velocity.x = x;
}

void GameObject::setYVelocity(float y)
{
	velocity.y = y;
}