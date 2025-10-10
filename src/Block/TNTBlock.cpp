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
void TNTBlock::update(const std::vector<std::shared_ptr<Block>>& blocks, sf::Time dt) {
	Block::update(blocks, dt);

	if (!getActive() && !getHasExploded()) {
		// Accumulate time only if the game is not paused
		explosionTimer += dt;

		if (explosion == nullptr) {
			explosion = std::make_unique<Explosion>(getExplosiveRadius(), getPosition().x, getPosition().y);
			explosion->playSound(0);
		}

		if (!explosion->getActive() || explosionTimer >= sf::milliseconds(50)) {
			// Damage nearby blocks
			for (auto& b : blocks) {
				if (b->getActive() && explosion->detectCollisionWithBlocks(*b)) {
					b->setHitPoints(b->getHitPoints() - explosion->getDamage());
					log.quickWrite(getLabel() + log.getSeparator() +
						"has dealt " + log.intToString(explosion->getDamage()) + " points of damage to " + b->getLabel() + ".");
				}
			}
			explosionTimer = sf::Time::Zero;
			setHasExploded(true);
		}
	}

	if (explosion == NULL) return;
	if (explosion->getActive()) {
		explosion->nextFrame(dt);
	}
}

void TNTBlock::render(Window &window, sf::Time dt) {
	Block::render(window, dt);
	if (explosion != NULL) {
		if (explosion->getActive()) {
			window.draw(*explosion);
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
	this->explosionTimer = sf::Time::Zero;
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
Explosion *TNTBlock::getExplosion() { return this->explosion.get(); }