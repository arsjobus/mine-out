#include "Block.h"

Block::~Block(void) { }

void Block::dropPowerUp() {
	int difficulty = 100;
	int lowerWinningBound = 1;
	int upperWinningBound = this->getDropChance();
	int randomNumber = std::rand() % difficulty + 1;
	if (randomNumber <= upperWinningBound) {
		log.quickWrite(std::string(this->getLabel() + log.getSeparator() + "just dropped a power-up!"));
		powerup->setActive( true );
		powerup->deltaClock.restart();
	}
}

void Block::randomizePowerUp() {
	size_t randomPowerupID = std::rand() % (PowerUp::TypeID::LAST_VALUE - 1) + 1;
	switch (randomPowerupID)
	{
		case PowerUp::TypeID::IncreasedDamage:
			powerup = std::make_unique<DamageIncrease>();
			break;
		case PowerUp::TypeID::GrowPaddle:
			powerup = std::make_unique<GrowPaddle>();
			break;
	};
	this->powerup->setPosition(sf::Vector2f(this->getPosition().x, this->getPosition().y));
}

/**
* Updates members of this object.
*/
void Block::update(const std::vector<std::shared_ptr<Block>>& blocks, sf::Time dt) {
	if (this->getActive()) {
		this->setTexture( &resource.getBlockTexture( this->getTextureID() ) );
		this->powerup->setPosition(sf::Vector2f(this->getPosition().x, this->getPosition().y));
	}
	if ((this->getActive()) && (this->getHitPoints() <= 0)) {
		this->setActive( false );
		log.quickWrite(std::string( this->getLabel() + log.getSeparator() + "was set to inactive!" ));
	}
	else if (!this->getActive() && !this->getHasDroppedPowerUp()) {
		this->dropPowerUp();
		// Set to true so that powerup will not drop from same block again.
		this->setHasDroppedPowerUp( true );
	}
	else if (!this->getActive() && this->powerup->getActive() && this->getHasDroppedPowerUp())
		powerup->update(dt);
}

/**
* Loads default settings.
*/
void Block::loadDefaultSettings() {
	sf::Vector2f defaultSize( 96.f, 32.f );
	this->setActive( true );
	this->randomizePowerUp();
	this->setHasDroppedPowerUp( false );
	this->setLabel( "Block" );
	this->setDropChance( 0.f );
	this->setHitPoints( 1 );
	this->setTextureID( 0 );
	this->setTexture( &resource.getBlockTexture( this->getTextureID() ) );
	this->setSize( defaultSize );
	this->setOrigin( sf::Vector2f(this->getGlobalBounds().size.x / 2.f, this->getGlobalBounds().size.y / 2.f) );
}

void Block::render(Window &window, sf::Time dt) {
	if (this->getActive())
		window.draw( *this );
	powerup->render(window);
}

/**
* Unloads all textures.
*/
void Block::unloadTextures() {
	if ( this->textures.size() > 0 ) {
		for ( int i = 0; i < this->textures.size(); ++i )
			this->textures.pop_back();
	}
}

bool Block::getActive() { return this->active; }
void Block::setActive( bool active ) { this->active = active; }
bool Block::getHasDroppedPowerUp() { return this->hasDroppedPowerUp; }
void Block::setHasDroppedPowerUp(bool hasDroppedPowerUp) { this->hasDroppedPowerUp = hasDroppedPowerUp; }
int Block::getDropChance() { return this->dropChance; }
void Block::setDropChance(int dropChance) { this->dropChance = dropChance; }
int Block::getHitPoints() { return this->hitPoints; }
void Block::setHitPoints( int hitPoints ) { this->hitPoints = hitPoints; }
PowerUp *Block::getPowerUp() { return this->powerup.get(); }
int Block::getTextureID() { return this->textureID; }
void Block::setTextureID( int textureID ) { this->textureID = textureID; }