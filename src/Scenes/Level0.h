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
#include "../LeftPanel.h"
#include "../Block/GoldBlock.h"
#include "../Block/NormalBlock.h"
#include "../Block/StoneBlock.h"
#include "../Block/TNTBlock.h"
#include "../TopPanel.h"
#include "../RightPanel.h"
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
	void loadBall();
	void loadBall(float radius);

	void loadBackground(Window &window);

	void loadDefaultSettings();

	void loadLevelDataFromFile(const char *filename);

	void loadLeftPanel();
	void loadLeftPanel(int newWidth, int newHeight);

	void loadTopPanel();
	void loadTopPanel(int newWidth, int newHeight);

	void loadRightPanel();
	void loadRightPanel(int newWidth, int newHeight);

	void loadObjects(Window &window);

	void loadPaddle();
	void loadPaddle(int paddleWidth, int paddleHeight);

	void resetBall(Window &window);
	void resetBlocks(Window &window);
	void resetLeftPanel(Window &window);
	void resetTopPanel(Window &window);
	void resetRightPanel(Window &window);
	void resetPlayer(Window &window);

	void resetAllObjects(Window &window);

	void resetMatch(Window &window);

	void stickBallToPlayer();

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
	bool detectCollisionBallAndLeftPanel();
	bool detectCollisionBallAndPlayer();
	bool detectCollisionBallAndRightPanel();
	bool detectCollisionBallAndTopPanel();
	void detectCollisionPlayerAndLeftPanel();
	void detectCollisionPlayerAndRightPanel();
	void detectCollisionPlayerAndPowerUp();
	void detectPowerUpOutOfBounds(Window &window);
	void updateActiveBlockCount();
	void updateGameObjects();
	void updatePowerUp();

private:
	size_t blocksPerRow;
	size_t activeBlocksCount;

protected:
	Log log;
	Ball *ball;
	LeftPanel *leftPanel;
	TopPanel *topPanel;
	RightPanel *rightPanel;
	Paddle *player;
	std::vector<Block *> blocks;
	bool isPaused;
	bool isPlaying;
	int defaultBallSpeed;
	std::string defaultDataDirectoryName;
	int defaultPaddleSpeed;
	float levelMargin;
	sf::Music music;
protected:
	sf::Clock deltaClock;

protected:
	void unloadObjects();
};

#endif
