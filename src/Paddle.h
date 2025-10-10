#pragma once

#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "PowerUp/PowerUp.h"
#include "StaticObject.h"

class Paddle: public GameObject, public sf::RectangleShape
{
public:
	enum Status {
		NULL_STATUS,
		COLLISION_DETECTED,
	};

	Paddle(void);
	Paddle(int newWidth, int newHeight);
	~Paddle(void);

	void update(bool isPaused);

public:
	// setters and getters
	size_t getActivePowerUp();
	void setActivePowerUp(int activePowerUp);
	
	size_t getShotAccuracy();
	void setShotAccuracy(int newAccuracy);

	size_t getStatus();
	void setStatus(size_t newStatus);

	bool getCanMoveLeft();
	void setCanMoveLeft(bool newBoolean);

	bool getCanMoveRight();
	void setCanMoveRight(bool newBoolean);

	bool isCollisionDetected(GameObject *otherGameObject);

private:
	// helpers
	void loadDefaultSettings();
	bool checkCollisionWithPanelL(GameObject *otherGameObject);
	bool checkCollisionWithPanelR(GameObject *otherGameObject);
	bool checkCollisionWithPowerUp(GameObject *otherGameObject);

private:
	// private members
	bool canMoveLeft;
	bool canMoveRight;
	size_t activePowerUp;
	size_t shotAccuracy;
	size_t status;
};

#endif
