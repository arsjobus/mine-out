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

	void render(Window &window);
	void update(std::vector<Block *> block, bool isPaused);

private:
	// helpers
	void loadDefaultSettings();
};

#endif
