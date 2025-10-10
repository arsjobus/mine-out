#pragma once

#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp> 
#include <SFML/Audio.hpp>
#include "Block/Block.h"
#include "GameObject.h"

class Explosion: public GameObject, public sf::RectangleShape
{
public:
	Explosion(void);
	Explosion(float radius, float positionX, float positionY);
	~Explosion(void);

	bool detectCollisionWithBlocks(Block &block);

	bool nextFrame(sf::Time dt);

	bool getActive();
	void setActive(bool active);

	float getRadius();
	void setRadius(float radius);

private:
	bool active;
	bool canPlayAnimation;
	float radius;
	sf::Time animationTimer;
	sf::Vector2i currentFrame;
	sf::Vector2i totalFrames;

	void loadDefaultSettings();
};

#endif
