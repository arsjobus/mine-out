#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "PreloadResources.h"
#include "Log.h"
#include "Window.h"

class GameObject
{
public:
    virtual ~GameObject(void) {};
	virtual void update() {};

	void playSound(int index);

public:
	// Accessors and mutators
	int getDamage();
	void setDamage(int damage);

    float getDirection();
    void setDirection(float newDirection);

	float getSpeed();
    void setSpeed(float speed);

	std::vector<sf::Sound> *getRefToSound();

	std::string getLabel();
	void setLabel(const char *blockLabel);

	sf::Vector2f getVelocity();
	void setXVelocity(float x);
	void setYVelocity(float y);

	sf::Clock deltaClock;

protected:
	Log log;
	PreloadResources resource;

	float direction;
    float speed;
	int damage;

	std::string label;

	sf::Time dt;
	sf::Vector2f velocity;

	std::vector<sf::Sound> sound;
};

#endif
