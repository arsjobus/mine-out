#include "GoldBlock.h"

GoldBlock::GoldBlock( void ) { this->loadDefaultSettings(); }
GoldBlock::GoldBlock( float width, float height ) {
	this->loadDefaultSettings();
	// Override default settings here:
	this->setSize( sf::Vector2f( width, height ) );
	this->setOrigin(sf::Vector2f( this->getGlobalBounds().size.x / 2.f, this->getGlobalBounds().size.y / 2.f ));
}
GoldBlock::~GoldBlock(void) { this->unloadTextures(); }

void GoldBlock::render(Window &window, sf::Time dt) { Block::render(window, dt); }

/**
* Updates members of this object.
*/
void GoldBlock::update(const std::vector<std::shared_ptr<Block>>& blocks, sf::Time dt) { Block::update(blocks, dt); }

/**
* Loads default settings.
*/
void GoldBlock::loadDefaultSettings(){
	Block::loadDefaultSettings();
	// Override Block default settings here:
	this->setLabel( "GoldBlock" );
	this->setTextureID( 5 );
	this->setDropChance( 90.f );
}
