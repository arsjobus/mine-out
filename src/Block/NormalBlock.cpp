#include "NormalBlock.h"

NormalBlock::NormalBlock( void ) { loadDefaultSettings(); }

NormalBlock::NormalBlock( float newWidth, float newHeight ) {
	loadDefaultSettings();
	// Override default settings here:
	setSize( sf::Vector2f( newWidth, newHeight ) );
	setOrigin( sf::Vector2f(getGlobalBounds().size.x / 2.f, getGlobalBounds().size.y / 2.f) );
}

/**
* Destructor
*/
NormalBlock::~NormalBlock( void ) { unloadTextures(); }

void NormalBlock::render(Window &window) { Block::render(window); }

/**
* Updates members of this object.
*/
void NormalBlock::update(std::vector<Block *> block, bool isPaused) {
	Block::update(block, isPaused);
	// Override Block update here:
}

/**
* Loads default settings.
*/
void NormalBlock::loadDefaultSettings() {
	Block::loadDefaultSettings();
	// Override Block default settings here:
	setTextureID( 4 );
	setLabel( "NormalBlock" );
	setDropChance( 10.f );
}