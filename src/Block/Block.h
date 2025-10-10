#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <SFML/Graphics.hpp>
#include "../GameObject.h"
#include "../PowerUp/PowerUp.h"
#include "../PowerUp/DamageIncrease.h"
#include "../PowerUp/GrowPaddle.h"
#include "../Window.h"

class Block: public GameObject, public sf::RectangleShape
{
public:
	enum Status {
		ACTIVE,
		DESTROYED,
	};

	virtual ~Block( void );
	virtual void update(std::vector<Block *> block, sf::Time dt);
	virtual void loadDefaultSettings();
	virtual void render(Window &window, sf::Time dt);

	void dropPowerUp();
	void randomizePowerUp();

public:
	// setters and getters
	void setHitPoints( int hitPoints );
	int getHitPoints();

	bool getActive();
	void setActive( bool active );

	bool getHasDroppedPowerUp();
	void setHasDroppedPowerUp(bool hasDroppedPowerUp);

	PowerUp *getPowerUp();

	int getDropChance();
	void setDropChance(int dropChance);

	int getTextureID();
	void setTextureID( int textureID );

protected:
	// helpers
	void unloadTextures();

protected:
	std::vector<sf::Texture> textures;

private:
	// private members
	bool active;
	bool hasDroppedPowerUp;
	int dropChance;
	int hitPoints;
	PowerUp *powerup;
	size_t status;
	size_t textureID;
};

#endif
