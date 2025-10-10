#pragma once

#ifndef TNTBLOCK_H
#define TNTBLOCK_H

#include "Block.h"
#include "../Explosion.h"

class TNTBlock: public Block
{
public:
	TNTBlock(void);
	TNTBlock(float width, float height);
	~TNTBlock(void);

	void render(Window &window, sf::Time dt);
	void update(const std::vector<std::shared_ptr<Block>>& blocks, sf::Time dt);

	Explosion *getExplosion();

	float getExplosiveRadius();
	void setExplosiveRadius(float explosiveRadius);

	bool getHasExploded();
	void setHasExploded(bool hasExploded);

private:
	// helpers
	bool hasExploded;
	std::unique_ptr<Explosion> explosion;
	void loadDefaultSettings();
	float explosiveRadius;
	sf::Time explosionTimer;
};

#endif
