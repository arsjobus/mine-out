#pragma once

#ifndef LOADSCREEN_H
#define LOADSCREEN_H

#include "../GameState.h"
#include <SFML/System/Clock.hpp>

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
	bool fontsAreLoading;
	bool levelsAreLoading;
	bool paddlesAreLoading;
	bool powerupsAreLoading;
	bool musicsAreLoading;
	bool soundsAreLoading;

	size_t loadedAnimationsCount;
	size_t loadedBackgroundsCount;
	size_t loadedBlocksCount;
	size_t loadedFontsCount;
	size_t loadedLevelCount;
	size_t loadedPaddlesCount;
	size_t loadedPowerupCount;
	size_t loadedMusicCount;
	size_t loadedSoundCount;

	std::vector<std::string> animationTextureNames;
	std::vector<std::string> backgroundTextureNames;
	std::vector<std::string> blockTextureNames;
	std::vector<std::string> fontNames;
	std::vector<std::string> levelTextureNames;
	std::vector<std::string> paddleTextureNames;
	std::vector<std::string> powerupTextureNames;
	std::vector<std::string> musicNames;
	std::vector<std::string> soundNames;

	sf::RectangleShape rectLoadBar;
	sf::RectangleShape rectLoadBarBackground;
	std::unique_ptr<sf::Text> textLoadingStatus;

	// Ensure the loading screen is visible for a minimum duration
	sf::Clock loadClock;
	float minDisplaySeconds = 1.5f;
	bool resourcesFinished = false;

	// Animated display percent for the loading bar (0..100)
	float displayedPercent = 0.f;
	// Animation-to-full state
	bool animatingToFull = false;
	float animationStartPercent = 0.f;
	float animationStartTimeSec = 0.f;
	float animationDurationSec = 0.f;
};

#endif
