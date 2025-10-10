#pragma once

#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>
#include "../GameObject.h"
#include "../Window.h"

class PowerUp: public GameObject, public sf::RectangleShape
{
public:
	enum TypeID {
		None,
		IncreasedDamage,
		GrowPaddle,
		LAST_VALUE,
	};

	~PowerUp(void);

	virtual void update(sf::Time dt);
	virtual void loadDefaultSettings();
	virtual void render(Window &window);

	bool detectOutOfBounds(Window &window);

	bool getActive();
	void setActive(bool active);

	float getFallSpeed();
	void setFallSpeed(float setFallSpeed);

	size_t getPowerID();
	void setPowerID(int powerID);

private:
	bool active;
	float fallSpeed;
	size_t powerID;
};

#endif