#pragma once

#ifndef PRELOADRESOURCES_H
#define PRELOADRESOURCES_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Log.h"

class PreloadResources
{
public:
	enum TextureList{
		BLOCK_DEFAULT,
		BLOCK_NORMAL,
		BLOCK_STONE,
	};

	PreloadResources(void);
	~PreloadResources(void);

	void loadAnimationTexture(const char *filename);
	void loadBlockTexture(const char *filename);
	void loadLevelTexture(const char *filename);
	void loadPaddleTexture(const char *filename);
	void loadPowerupTexture(const char *filename);
	void loadSound(const char *filename);

	std::string animationTextureFilePath();
	std::string blockTextureFilePath();
	std::string levelTextureFilePath();
	std::string paddleTextureFilePath();
	std::string powerupTextureFilePath();
	std::string soundFilePath();

	static void unloadResources();

public:
	// Accessors and Mutators
	float getLoadPercentile();
	void setLoadPercentile(float loadPercentile);

	int getResourceCount();
	void setResourceCount(int resourceCount);

	int getLoadedResourceCount();
	void setLoadedResourceCount(int loadedResourceCount);

	std::string &getSoundFileType();
	void setSoundFileType(const char *soundFileType);

	std::string &getTextureDirectoryName();
	void setTextureDirectoryName(const char *textureDirectoryName);

	std::string &getTextureFileType();
	void setTextureFileType(const char *textureFileType);

	std::string &getAnimationDirectoryName();
	void setAnimationDirectoryName(const char *animationDirectoryName);

	std::string &getBlockDirectoryName();
	void setBlockDirectoryName(const char *blockDirectoryName);

	std::string &getLevelDirectoryName();
	void setLevelDirectoryName(const char *levelDirectoryName);

	std::string &getPaddleDirectoryName();
	void setPaddleDirectoryName(const char *paddleDirectoryName);

	std::string &getPowerupDirectoryName();
	void setPowerupDirectoryName(const char *powerupDirectoryName);

	std::string &getSoundDirectoryName();
	void setSoundDirectoryName(const char *soundDirectoryName);

	sf::SoundBuffer &getBufferedSound(int index);
	sf::Texture &getAnimationTexture(int index);
	sf::Texture &getBlockTexture(int index);
	sf::Texture &getLevelTexture(int index);
	sf::Texture &getPaddleTexture(int index);
	sf::Texture &getPowerupTexture(int index);

private:
	// Helpers
	void calculateLoadPercentile();
	void loadDefaultSettings();

private:
	// Private Members
	float loadPercentile;
	size_t loadedResourceCount;
	size_t resourceCount;

	static std::vector<sf::SoundBuffer> sound;
	static std::vector<sf::Texture> txtAnimation;
	static std::vector<sf::Texture> txtBlock;
	static std::vector<sf::Texture> txtLevel;
	static std::vector<sf::Texture> txtPaddle;
	static std::vector<sf::Texture> txtPowerup;

	std::string animationDirectoryName;
	std::string blockDirectoryName;
	std::string levelDirectoryName;
	std::string paddleDirectoryName;
	std::string powerupDirectoryName;
	std::string soundDirectoryName;
	std::string soundFileType;
	std::string textureDirectoryName;
	std::string textureFileType;
};

#endif
