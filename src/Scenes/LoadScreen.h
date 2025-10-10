#pragma once

#ifndef LOADSCREEN_H
#define LOADSCREEN_H

#include "../GameState.h"

class LoadScreen: public GameState
{
public:
	LoadScreen(Window &window);
	~LoadScreen(void);

	void processEvents(Window &window);
    void update(Window &window, sf::Time dt);
    void render(Window &window, sf::Time dt);

	void initialize();
	void initilizeBackground(Window &window);
	void initilizeObjects(Window &window);

private:
	bool animationsAreLoading;
	bool backgroundsAreLoading;
	bool blocksAreLoading;
	bool levelsAreLoading;
	bool paddlesAreLoading;
	bool powerupsAreLoading;
	bool soundsAreLoading;

	size_t loadedAnimationsCount;
	size_t loadedBackgroundsCount;
	size_t loadedBlocksCount;
	size_t loadedLevelCount;
	size_t loadedPaddlesCount;
	size_t loadedPowerupCount;
	size_t loadedSoundCount;

	std::vector<std::string> animationTextureNames;
	std::vector<std::string> backgroundTextureNames;
	std::vector<std::string> blockTextureNames;
	std::vector<std::string> levelTextureNames;
	std::vector<std::string> paddleTextureNames;
	std::vector<std::string> powerupTextureNames;
	std::vector<std::string> soundNames;

	sf::RectangleShape rectLoadBar;
	sf::RectangleShape rectLoadBarBackground;
	std::unique_ptr<sf::Text> textLoadingStatus;
};

#endif
