#pragma once

#ifndef LEVEL0
#define LEVEL0

#include <fstream>
#include <sstream>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Ball.h"
#include "../Block/GoldBlock.h"
#include "../Block/NormalBlock.h"
#include "../Block/StoneBlock.h"
#include "../Block/TNTBlock.h"
#include "../Panel/PanelL.h"
#include "../Panel/PanelR.h"
#include "../Panel/PanelT.h"
#include "../GameState.h"
#include "../Log.h"
#include "../Paddle.h"
#include "../Window.h"

class Level0: public GameState
{
public:
	virtual ~Level0(void) {};

	void processEvents(Window &window);
	void update(Window &window);
	void render(Window &window);

public:
	void loadBackground(Window &window);
	void loadDefaultSettings();
	void loadLevelDataFromFile(const char *filename);
	void loadObjects(Window &window);
	void resetAllObjects(Window &window);
	void resetMatch(Window &window);

public:
	size_t getBlocksPerRow();
	void setBlocksPerRow(int blockCount);

	std::string getDefaultDataDirectoryName();
	void setDefaultDataDirectoryName(const char *newName);

	float getLevelMargin();
	void setLevelMargin(float marginLength);

	int getActiveBlocksCount();
	void setActiveBlocksCount(int count);

private:
	void detectBallOutOfBounds(Window &window);
	bool detectCollisionBallAndBlock();
	void detectCollisionPlayerAndPowerUp();
	void detectPowerUpOutOfBounds(Window &window);
	void loadBall(float radius);
	void loadPaddle(int paddleWidth, int paddleHeight);
	void loadPanelL(int newWidth, int newHeight);
	void loadPanelR(int newWidth, int newHeight);
	void loadPanelT(int newWidth, int newHeight);
	void resetBall(Window &window);
	void resetBlocks(Window &window);
	void resetPanelL(Window &window);
	void resetPanelR(Window &window);
	void resetPanelT(Window &window);
	void resetPlayer(Window &window);
	void updateActiveBlockCount();
	void updateGameObjects();
	void updatePowerUp();

private:
	size_t blocksPerRow;
	size_t activeBlocksCount;
	bool isMusicLoaded;

protected:
	Log log;
	Ball *ball;
	PanelL *panelL;
	PanelT *panelT;
	PanelR *panelR;
	Paddle *player;
	std::vector<Block *> blocks;
	bool isPaused;
	int defaultBallSpeed;
	std::string defaultDataDirectoryName;
	int defaultPaddleSpeed;
	float levelMargin;
	sf::Music music;

protected:
	sf::Clock deltaClock;
	GameState::State nextGameState = GameState::State::STATE_NULL;

protected:
	void unloadObjects();
};

#endif
