#include "TNTBlock.h"

TNTBlock::TNTBlock( void ) { this->loadDefaultSettings(); }
TNTBlock::TNTBlock( float width, float height ) {
	this->loadDefaultSettings();
	// Override default settings here:
	this->setSize( sf::Vector2f( width, height ) );
	this->setOrigin(sf::Vector2f(this->getGlobalBounds().size.x / 2.f, this->getGlobalBounds().size.y / 2.f ));
}
/**
* Destructor
*/
TNTBlock::~TNTBlock( void ) { this->unloadTextures(); }

/**
* Updates members of this object.
*/
void TNTBlock::update(std::vector<Block *> block, bool isPaused) {
	Block::update(block, isPaused);
	// Override Block updates here:
	if ( !this->getActive() && !this->getHasExploded() ) {
		dt += deltaClock.restart();
		if (explosion == NULL) {
			explosion = new Explosion( this->getExplosiveRadius(), this->getPosition().x, this->getPosition().y );

			explosion->playSound( 0 );
		}
		if (!explosion->getActive() || 
			dt.asMilliseconds() >= sf::milliseconds(250).asMilliseconds()) {
			// Detect collision between explosion and other blocks.
			for (int i = 0; i < block.size(); ++i) {
				if (block[i]->getActive() && explosion->detectCollisionWithBlocks(*block[i])) {
					block[i]->setHitPoints(block[i]->getHitPoints() - explosion->getDamage());
					log.quickWrite(std::string( this->getLabel() + log.getSeparator() + "has dealt " + log.intToString(explosion->getDamage()) + " points of damage to " + block[i]->getLabel() + "." ));
				}
			}
			dt = sf::seconds( 0 );
			this->setHasExploded( true );
		}
	}
}

void TNTBlock::render(Window &window) {
	Block::render(window);
	if (explosion != NULL) {
		if (explosion->getActive()) {
			window.draw(*explosion);
			explosion->nextFrame();
		}
	}
}

/**
* Loads default settings.
*/
void TNTBlock::loadDefaultSettings() {
	Block::loadDefaultSettings();
	// Override Block default settings here:
	this->deltaClock.restart();
	this->dt = sf::seconds(0);
	this->setDropChance( 0.f );
	this->setTextureID( 6 );
	this->setLabel( "TNTBlock" );
	this->setHasExploded( false );
	this->setExplosiveRadius( 50 );
	explosion = NULL;
}

float TNTBlock::getExplosiveRadius() { return this->explosiveRadius; }
void TNTBlock::setExplosiveRadius(float explosiveRadius) { this->explosiveRadius = explosiveRadius; }
bool TNTBlock::getHasExploded() { return this->hasExploded; }
void TNTBlock::setHasExploded(bool hasExploded) { this->hasExploded = hasExploded; }
Explosion *TNTBlock::getExplosion() { return this->explosion; }