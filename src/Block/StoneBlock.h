#pragma once

#ifndef STONEBLOCK_H
#define STONEBLOCK_H

#include "Block.h"

class StoneBlock: public Block
{
public:
	StoneBlock( void );
	StoneBlock( int newWidth, int newHeight );
	~StoneBlock( void );

	void render(Window &window, sf::Time dt);
	void update(const std::vector<std::shared_ptr<Block>>& blocks, sf::Time dt);

private:
	// helpers
	void loadDefaultSettings();
};

#endif
