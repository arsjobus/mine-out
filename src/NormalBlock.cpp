#include "NormalBlock.h"

NormalBlock::NormalBlock( void )
{
	loadDefaultSettings();
}

NormalBlock::NormalBlock( float newWidth, float newHeight )
{
	loadDefaultSettings();

	// Override default settings here:
	setSize( sf::Vector2f( newWidth, newHeight ) );

	setOrigin( getGlobalBounds().width / 2.f, getGlobalBounds().height / 2.f );
}

/**
* Destructor
*/
NormalBlock::~NormalBlock( void )
{
	unloadTextures();
}

void NormalBlock::render(Window &window) {
	Block::render(window);
}

/**
* Updates members of this object.
*/
void NormalBlock::update(std::vector<Block *> block)
{
	Block::update(block);

	// Override Block update here:

}

/**
* Loads default settings.
*/
void NormalBlock::loadDefaultSettings()
{
	Block::loadDefaultSettings();

	// Override Block default settings here:
	setTextureID( 4 );

	setLabel( "NormalBlock" );

	setDropChance( 10.f );
}