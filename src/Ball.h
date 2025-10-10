#pragma once

#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include "Block.h"
#include "GameObject.h"
#include "Panel/LeftPanel.h"
#include "Panel/RightPanel.h"
#include "Panel/TopPanel.h"
#include "Window.h"

#define PI 3.14159

class Ball: public GameObject, public sf::CircleShape
{
public:
	enum Status {
		STUCK_TO_PLAYER = 1,
		LAUNCHING,
		MOVING,
		BOUNCING,
	};

	Ball( void );
	Ball( float newRadius );
	~Ball( void );

	void update();

public:
	// setters and getters
	size_t getStatus();
	void setStatus(size_t newStatus);

	bool getCanBounce();
	void setCanBounce(bool canBounce);
	bool isCollisionDetected(GameObject *otherGameObject);

private:
	// helpers
	void loadDefaultSettings();
	bool checkCollisionWithPlayer(GameObject *otherGameObject);
	bool checkCollisionWithPanelL(GameObject *otherGameObject);
	bool checkCollisionWithPanelR(GameObject *otherGameObject);
	bool checkCollisionWithPanelT(GameObject *otherGameObject);

private:
	// private members
	bool canBounce;
	size_t status;
};

#endif
