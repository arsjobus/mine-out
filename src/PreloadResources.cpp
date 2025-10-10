#include "PreloadResources.h"

std::vector<sf::SoundBuffer> PreloadResources::sound;
std::vector<sf::Texture> PreloadResources::txtAnimation;
std::vector<sf::Texture> PreloadResources::txtBlock;
std::vector<sf::Texture> PreloadResources::txtLevel;
std::vector<sf::Texture> PreloadResources::txtPaddle;
std::vector<sf::Texture> PreloadResources::txtPowerup;

PreloadResources::PreloadResources(void) {
	loadDefaultSettings();
	// Override default settings here:
}
PreloadResources::~PreloadResources(void) { }

void PreloadResources::loadDefaultSettings() {
	this->setAnimationDirectoryName("animations");
	this->setBlockDirectoryName("blocks");
	this->setPaddleDirectoryName("paddles");
	this->setLevelDirectoryName("levels");
	this->setPowerupDirectoryName("powerups");
	this->setTextureDirectoryName("images");
	this->setSoundDirectoryName("sfx");
	this->setSoundFileType(".wav");
	this->setTextureFileType(".png");
	this->setLoadPercentile( 0.f );
	this->setLoadedResourceCount( 0 );
	this->setResourceCount( 0 );
}

std::string PreloadResources::soundFilePath() {
	return std::string( this->getSoundDirectoryName() + "/" );
}

std::string PreloadResources::animationTextureFilePath() {
	return std::string( this->getTextureDirectoryName() + "/" + this->getAnimationDirectoryName() + "/" );
}

std::string PreloadResources::blockTextureFilePath() {
	return std::string( this->getTextureDirectoryName() + "/" + this->getBlockDirectoryName() + "/" );
}

std::string PreloadResources::levelTextureFilePath() {
	return std::string( this->getTextureDirectoryName() +  "/" + this->getLevelDirectoryName() + "/" );
}

std::string PreloadResources::paddleTextureFilePath() {
	return std::string( this->getTextureDirectoryName() + "/" + this->getPaddleDirectoryName() + "/" );
}

std::string PreloadResources::powerupTextureFilePath() {
	return std::string( this->getTextureDirectoryName() + "/" + this->getPowerupDirectoryName() + "/" );
}

void PreloadResources::loadSound(const char *filename) {
	sf::SoundBuffer soundBuffer;
	if (!soundBuffer.loadFromFile(std::string(this->soundFilePath() + filename + this->getSoundFileType())))
		std::cout << "Could not load: " << filename << this->getSoundFileType() << std::endl;
	else
		std::cout << "Loaded: " << filename << this->getSoundFileType() << std::endl;
	sound.push_back(soundBuffer);
	this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
	this->calculateLoadPercentile();
}

void PreloadResources::loadAnimationTexture(const char *filename) {
	sf::Texture texture;
	if (!texture.loadFromFile(std::string(this->animationTextureFilePath() + filename + this->getTextureFileType())))
		std::cout << "Could not load: " << filename << this->getTextureFileType() << std::endl;
	else
		std::cout << "Loaded: " << filename << this->getTextureFileType() << std::endl;
	txtAnimation.push_back(texture);
	this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
	this->calculateLoadPercentile();
}

void PreloadResources::loadBlockTexture(const char *filename) {
	sf::Texture texture;
	if (!texture.loadFromFile(std::string(this->blockTextureFilePath() + filename + this->getTextureFileType())))
		std::cout << "Could not load: " << filename << this->getTextureFileType() << std::endl;
	else
		std::cout << "Loaded: " << filename << this->getTextureFileType() << std::endl;
	txtBlock.push_back(texture);
	this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
	this->calculateLoadPercentile();
}

void PreloadResources::loadLevelTexture(const char *filename) {
	sf::Texture texture;
	if (!texture.loadFromFile(std::string(this->levelTextureFilePath() + filename + this->getTextureFileType())))
		std::cout << "Could not load: " << filename << this->getTextureFileType() << std::endl;
	else
		std::cout << "Loaded: " << filename << this->getTextureFileType() << std::endl;
	txtLevel.push_back(texture);
	this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
	this->calculateLoadPercentile();
}

void PreloadResources::loadPaddleTexture(const char *filename) {
	sf::Texture texture;
	if (!texture.loadFromFile(std::string(this->paddleTextureFilePath() + filename + this->getTextureFileType())))
		std::cout << "Could not load: " << filename << this->getTextureFileType() << std::endl;
	else
		std::cout << "Loaded: " << filename << this->getTextureFileType() << std::endl;
	txtPaddle.push_back(texture);
	this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
	this->calculateLoadPercentile();
}

void PreloadResources::loadPowerupTexture(const char *filename) {
	sf::Texture texture;
	if (!texture.loadFromFile(std::string(this->powerupTextureFilePath() + filename + this->getTextureFileType())))
		std::cout << "Could not load: " << filename << this->getTextureFileType() << std::endl;
	else
		std::cout << "Loaded: " << filename << this->getTextureFileType() << std::endl;
	txtPowerup.push_back(texture);
	this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
	this->calculateLoadPercentile();
}

void PreloadResources::calculateLoadPercentile() {
	this->setLoadPercentile( this->getLoadedResourceCount() * 100 / this->getResourceCount() * 100 );
}

void PreloadResources::unloadResources() {
	sound.clear();
	txtAnimation.clear();
	txtBlock.clear();
	txtLevel.clear();
	txtPaddle.clear();
	txtPowerup.clear();
}

sf::SoundBuffer &PreloadResources::getBufferedSound(int index) { return this->sound[index]; }

sf::Texture &PreloadResources::getAnimationTexture(int index) { return this->txtAnimation[index]; }

sf::Texture &PreloadResources::getBlockTexture(int index) { return this->txtBlock[index]; }

sf::Texture &PreloadResources::getLevelTexture(int index) { return this->txtLevel[index]; }

sf::Texture &PreloadResources::getPaddleTexture(int index) { return this->txtPaddle[index]; }

sf::Texture &PreloadResources::getPowerupTexture(int index) { return this->txtPowerup[index]; }

float PreloadResources::getLoadPercentile() { return this->loadPercentile; }

void PreloadResources::setLoadPercentile(float loadPercentile) { this->loadPercentile = loadPercentile; }

int PreloadResources::getLoadedResourceCount() { return this->loadedResourceCount; }

void PreloadResources::setLoadedResourceCount(int loadedResourceCount) { this->loadedResourceCount = loadedResourceCount; }

int PreloadResources::getResourceCount() { return this->resourceCount; }

void PreloadResources::setResourceCount(int resourceCount) { this->resourceCount = resourceCount; }

std::string &PreloadResources::getSoundFileType() { return this->soundFileType; }

void PreloadResources::setSoundFileType(const char *soundFileType) { this->soundFileType = soundFileType; }

std::string &PreloadResources::getTextureFileType() { return this->textureFileType; }

void PreloadResources::setTextureFileType(const char *textureFileType) { this->textureFileType = textureFileType; }

std::string &PreloadResources::getAnimationDirectoryName() { return this->animationDirectoryName; }

void PreloadResources::setAnimationDirectoryName(const char *animationDirectoryName) {
	this->animationDirectoryName = animationDirectoryName;
}

std::string &PreloadResources::getBlockDirectoryName() { return this->blockDirectoryName; }

void PreloadResources::setBlockDirectoryName(const char *blockDirectoryName) {
	this->blockDirectoryName = blockDirectoryName;
}

std::string &PreloadResources::getLevelDirectoryName() { return this->levelDirectoryName; }

void PreloadResources::setLevelDirectoryName(const char *levelDirectoryName) {
	this->levelDirectoryName = levelDirectoryName;
}

std::string &PreloadResources::getPaddleDirectoryName() { return this->paddleDirectoryName; }

void PreloadResources::setPaddleDirectoryName(const char *paddleDirectoryName) {
	this->paddleDirectoryName = paddleDirectoryName;
}

std::string &PreloadResources::getPowerupDirectoryName() { return this->powerupDirectoryName; }

void PreloadResources::setPowerupDirectoryName(const char *powerupDirectoryName) {
	this->powerupDirectoryName = powerupDirectoryName;
}

std::string &PreloadResources::getSoundDirectoryName() { return this->soundDirectoryName; }

void PreloadResources::setSoundDirectoryName(const char *soundDirectoryName) {
	this->soundDirectoryName = soundDirectoryName;
}

std::string &PreloadResources::getTextureDirectoryName(){ return this->textureDirectoryName; }

void PreloadResources::setTextureDirectoryName(const char *textureDirectoryName) {
	this->textureDirectoryName = textureDirectoryName;
}