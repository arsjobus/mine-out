#pragma once

#ifndef NORMALBLOCK_H
#define NORMALBLOCK_H

#include "Block.h"

class NormalBlock: public Block
{
public:
	NormalBlock( void );
	NormalBlock( float newWidth, float newHeight );
	~NormalBlock( void );

	void render(Window &window, sf::Time dt);
	void update(std::vector<Block *> block, sf::Time dt);

private:
	// helpers
	void loadDefaultSettings();
};

#endif
