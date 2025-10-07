#include "StoneBlock.h"

StoneBlock::StoneBlock( void ) { loadDefaultSettings(); }
StoneBlock::StoneBlock( int newWidth, int newHeight ) {
	loadDefaultSettings();
	// Override default settings here:
	setSize( sf::Vector2f( newWidth, newHeight ) );
	setOrigin( sf::Vector2f(getGlobalBounds().size.x / 2.f, getGlobalBounds().size.y / 2.f) );
}
/**
* Destructor
*/
StoneBlock::~StoneBlock(void) { unloadTextures(); }

void StoneBlock::render(Window &window) { Block::render(window); }

/**
* Updates members of this object.
*/
void StoneBlock::update(std::vector<Block *> block) {
	// Change textures before allowing Block to update.
	if (this->getActive() && 
		this->getTextureID() != 2 &&
		this->getHitPoints() == 2)
			this->setTextureID(2);
	else if (getActive() && 
		this->getTextureID() != 3 &&
		this->getHitPoints() == 1)
			this->setTextureID(3);
	Block::update(block);
	// Make additional updates specific to this object here:
}

/**
* Loads default settings.
*/
void StoneBlock::loadDefaultSettings() {
	Block::loadDefaultSettings();
	// Override Block default settings here:
	setDropChance( 20.f );
	setHitPoints( 3 );
	setFillColor( sf::Color( 255, 255, 255 ) );
	setLabel( "StoneBlock" );
	setTextureID( 1 );
	setTexture( &resource.getBlockTexture( getTextureID() ) );
}