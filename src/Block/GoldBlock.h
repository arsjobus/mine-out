#pragma once

#ifndef GOLDBLOCK_H
#define GOLDBLOCK_H

#include "Block.h"

class GoldBlock: public Block
{
public:
	GoldBlock(void);
	GoldBlock(float width, float height);
	~GoldBlock(void);

	void render(Window &window, sf::Time dt);
	void update(const std::vector<std::shared_ptr<Block>>& blocks, sf::Time dt);

private:
	// helpers
	void loadDefaultSettings();
};

#endif