#include "Level0.h"

void Level0::processEvents(Window &window)
{
	sf::Event userInput;

	while (window.pollEvent(userInput))
	{
		switch (userInput.type)
		{
			case sf::Event::Closed:
				setNextState( GameState::State::STATE_EXIT );
				break;

			case sf::Event::KeyPressed:
				if (userInput.key.code == sf::Keyboard::Space &&
					(ball->getStatus() == Ball::STUCK_TO_PLAYER) &&
					(isPlaying) &&
					(!isPaused))
				{
					ball->setStatus(Ball::LAUNCHING);
				}
				else if ((userInput.key.code == sf::Keyboard::A || 
					userInput.key.code == sf::Keyboard::Left) &&
					(player->getCanMoveLeft()) &&
                    (isPlaying) && 
                    (!isPaused))
                {
                    player->setXVelocity(-player->getSpeed());
                }              
                else if ((userInput.key.code == sf::Keyboard::D || 
                    userInput.key.code == sf::Keyboard::Right) &&
					(player->getCanMoveRight()) &&
                    (isPlaying) && 
                    (!isPaused))
                {
                    player->setXVelocity(player->getSpeed());
                }
				else if (userInput.key.code == sf::Keyboard::R)
				{
					setNextState( GameState::State::STATE_LEVEL1 );
				}
				else if (userInput.key.code == sf::Keyboard::P)
				{
					if (this->isPaused)
						this->isPaused = false;
					else
						this->isPaused = true;
				}
				else if (userInput.key.code == sf::Keyboard::Escape)
				{
					setNextState( STATE_EXIT );
				}
				break;
			case sf::Event::KeyReleased:
					if (userInput.key.code == sf::Keyboard::A || userInput.key.code == sf::Keyboard::D || userInput.key.code == sf::Keyboard::Left || userInput.key.code == sf::Keyboard::Right)
						player->setXVelocity( 0 );
				break;
		}
	}
}

void Level0::update(Window &window)
{
	// If the game is playing and not paused.
	if (isPlaying && !isPaused)
	{
		// Reset the delta time at the beginning of every update phase.
		sf::Time dt = deltaClock.restart();

		// :::::::::::::::::
		// :::: UPDATES ::::
		// :::::::::::::::::

		// Play music if it is not
		if (music.getStatus() != sf::Music::Playing)
		{
			music.play();
		}

		// Update active block count
		updateActiveBlockCount();
		
		// Update game objects.
		updateGameObjects();

		// Update the player's currently active powerup.
		updatePowerUp();

		// The ball follows player's paddle until it is launched.
		stickBallToPlayer();

		// :::::::::::::::::::::::::::
		// :::: DETECT COLLISIONS ::::
		// :::::::::::::::::::::::::::

		if (ball->getCanBounce() &&
		detectCollisionBallAndBlock() ||
		detectCollisionBallAndLeftPanel() ||
		detectCollisionBallAndPlayer() ||
		detectCollisionBallAndRightPanel() ||
		detectCollisionBallAndTopPanel()
		)
		{
			ball->setCanBounce( false );
		}
		else if (!ball->getCanBounce() &&
		!detectCollisionBallAndBlock() &&
		!detectCollisionBallAndLeftPanel() &&
		!detectCollisionBallAndPlayer() &&
		!detectCollisionBallAndRightPanel() &&
		!detectCollisionBallAndTopPanel()
		)
		{
			ball->setCanBounce( true );
		}

		// Detect and handle collision between the player and the left panel.
		detectCollisionPlayerAndLeftPanel();

		// Detect and handle collision between the player and the right panel.
		detectCollisionPlayerAndRightPanel();

		// Detect and handle collision between the player and a powerup.
		detectCollisionPlayerAndPowerUp();

		// Detect and handle the powerup going out of bounds.
		detectPowerUpOutOfBounds(window);

		// Detect and handle the ball going out of bounds.
		detectBallOutOfBounds(window);
	}
	else
	{
		music.pause();
		//window.setMouseCursorVisible( true );
	}
}

void Level0::render(Window& window)
{
	window.clear();

	window.draw(getRefToBackground());

	window.draw(*player);

	window.draw(*ball);

	window.draw(*topPanel);

	window.draw(*leftPanel);

	window.draw(*rightPanel);

	for (int i = 0; i < blocks.size(); ++i)
	{
		blocks[i]->render(window);
	}

}

/**
* Detect and handle the ball going out of bounds.
*/
void Level0::detectBallOutOfBounds(Window &window)
{
	if (ball->getPosition().y > window.getSize().y ||
		ball->getPosition().y < 0 ||
		ball->getPosition().x > window.getSize().x ||
		ball->getPosition().x < 0
		)
		resetMatch(window);
}

bool Level0::detectCollisionBallAndBlock()
{
	for (int i = 0; i < blocks.size(); ++i) 
	{
		if (blocks[i]->getActive() &&
			ball->getCanBounce() &&
			ball->getGlobalBounds().intersects( blocks[i]->getGlobalBounds() ) )
		{
			blocks[i]->setHitPoints( blocks[i]->getHitPoints() - ball->getDamage() );

			log.quickWrite(std::string( "Ball" + log.getSeparator() + "has dealt " + log.intToString(ball->getDamage()) + " points of damage to " + blocks[i]->getLabel() + "." ));

			if (ball->getPosition().y > blocks[i]->getGlobalBounds().top + blocks[i]->getGlobalBounds().height ||
				ball->getPosition().y < blocks[i]->getGlobalBounds().top)
			{
				ball->setYVelocity( -ball->getVelocity().y );
			}
			else
			{
				ball->setXVelocity( -ball->getVelocity().x );
			}

			return true;
		}
	}

	return false;
}

bool Level0::detectCollisionBallAndLeftPanel()
{
	if (ball->getCanBounce() &&
		ball->getGlobalBounds().intersects( leftPanel->getGlobalBounds() ) )
	{
		ball->setXVelocity( -ball->getVelocity().x );
		return true;
	}
	else return false;
}

bool Level0::detectCollisionBallAndPlayer()
{
	if (ball->getCanBounce() &&
		ball->getGlobalBounds().intersects(player->getGlobalBounds()))
	{
		player->playSound( 0 );

		if (ball->getPosition().y > player->getGlobalBounds().top + player->getGlobalBounds().height ||
			ball->getPosition().y < player->getGlobalBounds().top)
		{
			if (ball->getPosition().y < player->getGlobalBounds().top) 
			{
				ball->setYVelocity( -ball->getVelocity().y );

				ball->setXVelocity((ball->getPosition().x - player->getPosition().x) * 2);
			}
		}
		else
		{
			ball->setXVelocity( -ball->getVelocity().x );
		}

		return true;
	}

	return false;
}

void Level0::detectCollisionPlayerAndLeftPanel()
{
	if (player->getCanMoveLeft() &&
		leftPanel->getPosition().x < player->getPosition().x && 
		player->getGlobalBounds().intersects( leftPanel->getGlobalBounds() ) )
	{
		player->setCanMoveLeft( false );
		player->setXVelocity( NULL );
	}

	else if (player->getCanMoveRight() &&
		rightPanel->getPosition().x > player->getPosition().x &&
		player->getGlobalBounds().intersects( rightPanel->getGlobalBounds() ) )
	{
		player->setCanMoveRight( false );
		player->setXVelocity( NULL );
	}
}

void Level0::detectCollisionPlayerAndRightPanel()
{
	if ( ( !player->getCanMoveLeft() ) && 
		leftPanel->getPosition().x < player->getPosition().x &&
		( !player->getGlobalBounds().intersects( leftPanel->getGlobalBounds() ) ) )
    {
		player->setCanMoveLeft( true );
    }

    else if ( ( !player->getCanMoveRight() ) && 
		rightPanel->getPosition().x > player->getPosition().x && 
		( !player->getGlobalBounds().intersects( rightPanel->getGlobalBounds() ) ) )
    {
        player->setCanMoveRight( true );
    }
}

/**
* Detect and handle a powerup colliding with the player.
*/
void Level0::detectCollisionPlayerAndPowerUp()
{
	for (int i = 0; i < blocks.size(); ++i)
	{
		if (blocks[i]->getPowerUp()->getActive() &&
			player->getGlobalBounds().intersects(blocks[i]->getPowerUp()->getGlobalBounds()))
		{
			// Set the powerup to inactive.
			blocks[i]->getPowerUp()->setActive( false );

			// Set the players active power up equal to the ID of the powerup that was
			// collided with.
			player->setActivePowerUp( blocks[i]->getPowerUp()->getPowerID() );
		}
	}
}

bool Level0::detectCollisionBallAndRightPanel()
{
	if (ball->getCanBounce() &&
		ball->getGlobalBounds().intersects(rightPanel->getGlobalBounds() ) )
	{
		ball->setXVelocity( -ball->getVelocity().x );
		return true;
	}
	else return false;
}

bool Level0::detectCollisionBallAndTopPanel()
{
	if (ball->getCanBounce() && 
		ball->getGlobalBounds().intersects( topPanel->getGlobalBounds() ) )
	{
		ball->setYVelocity( -ball->getVelocity().y );
		return true;
	}
	else return false;
}

/**
*Detect and handle powerups that are outside of the window.
*/
void Level0::detectPowerUpOutOfBounds(Window &window)
{
	for (int i = 0; i < blocks.size(); ++i)
	{
		if (blocks[i]->getActive())
			blocks[i]->getPowerUp()->detectOutOfBounds(window);
	}

}

void Level0::updateActiveBlockCount()
{
	for (int i = 0; i < blocks.size(); ++i)
	{
		if (blocks[i]->getActive())
			this->setActiveBlocksCount(this->getActiveBlocksCount() + 1);
	}
}

void Level0::updateGameObjects()
{
	// Update the player object
	player->update();

	// Update the ball object
	ball->update();

	// Update every block object
	for (int i = 0; i < blocks.size(); ++i)
	{
		blocks[i]->update(blocks);
	}

}

void Level0::updatePowerUp()
{
	// Update active powerup
	if (player->getActivePowerUp() == PowerUp::TypeID::IncreasedDamage)
	{
		ball->setDamage( 5 );
	}
	else
	{
		ball->setDamage( 1 );
	}

	if (player->getActivePowerUp() == PowerUp::TypeID::GrowPaddle)
	{
		player->setScale(2.0, 1.0);
	}
	else
	{
		player->setScale(1.0, 1.0);
	}
}

void Level0::loadBackground(Window &window)
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading background.."));

	sf::Vector2f screenResolution(window.getScreenResolution().x, window.getScreenResolution().y);

	getRefToBackground().setSize(sf::Vector2f(screenResolution.x, screenResolution.y));
    getRefToBackground().setFillColor(sf::Color::Black);
    getRefToBackground().setOrigin(getRefToBackground().getGlobalBounds().width / 2, getRefToBackground().getGlobalBounds().height / 2);
	getRefToBackground().setPosition(screenResolution.x / 2, screenResolution.y / 2);
}

void Level0::loadBall()
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the ball.."));

	ball = new Ball(16);
}

void Level0::loadBall(float radius)
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the ball.."));

	ball = new Ball(radius);
}

void Level0::loadDefaultSettings()
{
	// Load music and play
	music.openFromFile("music/crystalcave.ogg");

	music.setLoop( true );
	
	activeBlocksCount = 0;

	// Set the blocks per row to 15
	setBlocksPerRow(15);

	// Set default data directory name
	setDefaultDataDirectoryName("data");
	
	// Set the level margin to 10px
	setLevelMargin(10.f);

	isPaused = false;

	isPlaying = true;
}

void Level0::loadLeftPanel()
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the left panel.."));

	leftPanel = new LeftPanel();
}

void Level0::loadLeftPanel(int newWidth, int newHeight)
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the left panel.."));

	leftPanel = new LeftPanel(newWidth, newHeight);
}

void Level0::loadLevelDataFromFile(const char *filename)
{
	// Craft the file path
	std::string strFilePath = (sf::String(getDefaultDataDirectoryName() + "/" + filename));

	// Open the file by using the crafted file path
	std::ifstream levelDataFile(strFilePath.c_str());

	// If the file cannot be open then stop the game until it is replaced
	if (!levelDataFile.is_open())
	{
		log.quickWrite(LOG_ERROR, std::string(getCurrentModeName() + log.getSeparator() + "Unable to open level data file: " + filename));

		// Set the next state to exit the game.
		setNextState( STATE_EXIT );
	}
	else
	{
		std::string strDataEntry;

		NormalBlock *ptrNormalBlock;
		StoneBlock *ptrStoneBlock;
		GoldBlock *ptrGoldBlock;
		TNTBlock *ptrTNTBlock;

		while(!levelDataFile.eof())
		{
			// Read data into memory
			std::getline(levelDataFile, strDataEntry, ',');

			if (std::strcmp(strDataEntry.c_str(), ":normal:") == 0)
			{
				ptrNormalBlock = new NormalBlock();

				blocks.push_back(ptrNormalBlock);
			}
			else if (std::strcmp(strDataEntry.c_str(), ":stone:") == 0)
			{
				ptrStoneBlock = new StoneBlock();

				blocks.push_back(ptrStoneBlock);
			}
			else if (std::strcmp(strDataEntry.c_str(), ":gold:" ) == 0)
			{
				ptrGoldBlock = new GoldBlock();

				blocks.push_back(ptrGoldBlock);
			}
			else if (std::strcmp(strDataEntry.c_str(), ":tnt:" ) == 0)
			{
				ptrTNTBlock = new TNTBlock();

				blocks.push_back(ptrTNTBlock);
			}
			else if (std::strcmp(strDataEntry.c_str(), ":blank:" ) == 0)
			{
				ptrNormalBlock = new NormalBlock();

				ptrNormalBlock->setActive( false );

				ptrNormalBlock->setHasDroppedPowerUp( true );

				blocks.push_back(ptrNormalBlock);
			}
		}
	}
}

void Level0::loadObjects(Window &window)
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading objects.."));

	loadTopPanel(window.getScreenResolution().x, 100);

	loadRightPanel(topPanel->getSize().y / 2, window.getScreenResolution().y - topPanel->getSize().y);

	loadLeftPanel(topPanel->getSize().y / 2, window.getScreenResolution().y - topPanel->getSize().y);

	loadBall(8);

	loadPaddle(96, 16);
}

void Level0::loadTopPanel()
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the top panel.."));

	topPanel = new TopPanel();
}

void Level0::loadTopPanel(int newWidth, int newHeight)
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the top panel.."));

	topPanel = new TopPanel(newWidth, newHeight);
}

void Level0::loadRightPanel()
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the right panel.."));

	rightPanel = new RightPanel();
}

void Level0::loadRightPanel(int newWidth, int newHeight)
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the right panel.."));

	rightPanel = new RightPanel(newWidth, newHeight);
}

void Level0::loadPaddle() 
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the player's paddle.."));

	player = new Paddle(160, 32);
}

void Level0::loadPaddle(int paddleWidth, int paddleHeight) 
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the player's paddle.."));

	player = new Paddle(paddleWidth, paddleHeight);
}

void Level0::resetAllObjects(Window &window)
{
	// Always reset the top panel before the left panel and right panel
	resetTopPanel(window);

	resetRightPanel(window);

	resetLeftPanel(window);

	// Always reset the player before the ball.
	resetPlayer(window);

	resetBall(window);

	resetBlocks(window);
}

void Level0::resetMatch(Window &window)
{
	resetBall(window);
	resetPlayer(window);
}

void Level0::resetBall(Window &window)
{
	// Dependent on the position of the player
	sf::Vector2f position(player->getPosition().x, player->getGlobalBounds().top - ball->getRadius());

	std::string strXPosition = log.floatToString(position.x);

	std::string strYPosition = log.floatToString(position.y);

	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Resetting position of the ball: " + "X = " + strXPosition + "; Y = " + strYPosition ));

	ball->setStatus( Ball::Status::STUCK_TO_PLAYER );

	ball->setPosition(position);

	defaultBallSpeed = 500;
}

void Level0::resetBlocks(Window &window)
{
	int blockCounter = 0;

	float padding = 2;

	int xPosition = leftPanel->getSize().x + getLevelMargin();

	int yPosition = topPanel->getSize().y + getLevelMargin();

	float blockWidth = ((window.getSize().x - (leftPanel->getSize().x + (getLevelMargin() * 2.f) + rightPanel->getSize().x)) / (getBlocksPerRow()) - padding);

	float blockHeight = 32.f;
	
	for (int i = 0; i < blocks.size(); ++i)
	{
		blocks[i]->setSize(sf::Vector2f(blockWidth, blockHeight));

		blocks[i]->setOrigin(blocks[i]->getGlobalBounds().width / 2, blocks[i]->getGlobalBounds().height / 2);

		if (++blockCounter == 1)
			xPosition += blocks[i]->getOrigin().x;
		else if ((blockCounter - 1) % getBlocksPerRow() == 0)
		{
			xPosition = leftPanel->getSize().x + getLevelMargin() + blocks[i]->getOrigin().x;
			yPosition += blocks[i]->getGlobalBounds().height + getLevelMargin();
		}
		else
			xPosition += padding + blocks[i]->getSize().x;

		blocks[i]->setPosition(xPosition + padding, yPosition + blocks[i]->getOrigin().y);
	}
}

void Level0::resetLeftPanel(Window &window)
{
	// Dependent on the top panel
	sf::Vector2f position(
		leftPanel->getOrigin().x,
		window.getScreenResolution().y / 2 + topPanel->getOrigin().y
		);

	std::string strXPosition = log.floatToString(position.x);

	std::string strYPosition = log.floatToString(position.y);

	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Resetting position of the left panel: " + "X = " + strXPosition + "; Y = " + strYPosition ));

	leftPanel->setPosition(position);
}

void Level0::resetTopPanel(Window &window)
{
	sf::Vector2f position(window.getScreenResolution().x / 2, topPanel->getSize().y - topPanel->getOrigin().y);

	std::string strXPosition = log.floatToString(position.x);

	std::string strYPosition = log.floatToString(position.y);

	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Resetting position of the top panel: " + "X = " + strXPosition + "; Y = " + strYPosition ));

	topPanel->setPosition(position);
}

void Level0::resetRightPanel(Window &window)
{
	// Dependent on the top panel
	sf::Vector2f position(
		window.getScreenResolution().x - rightPanel->getOrigin().x,
		window.getScreenResolution().y / 2 + topPanel->getOrigin().y
		);

	std::string strXPosition = log.floatToString(position.x);

	std::string strYPosition = log.floatToString(position.y);

	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Resetting position of the right panel: " + "X = " + strXPosition + "; Y = " + strYPosition ));

	rightPanel->setPosition(position);
}

void Level0::resetPlayer(Window &window)
{
	// Dependent on the position of the bottom panel
	sf::Vector2f position(window.getScreenResolution().x / 2, window.getScreenResolution().y - player->getOrigin().y - 16);

	std::string strXPosition = log.floatToString(position.x);

	std::string strYPosition = log.floatToString(position.y);

	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Resetting position of the paddle: " + "X = " + strXPosition + "; Y = " + strYPosition ));

	player->setActivePowerUp( PowerUp::TypeID::None );

	player->setPosition(position);

	player->setXVelocity( NULL );

	player->setYVelocity( NULL );

	this->defaultPaddleSpeed = 500;
}

void Level0::stickBallToPlayer()
{
	// The ball will follow the players paddle until it is launched.
	if (ball->getStatus() == Ball::Status::STUCK_TO_PLAYER)
	{
		ball->setPosition(player->getPosition().x, ball->getPosition().y);
	}
}

void Level0::unloadObjects()
{
	if (ball != 0)
	{
		delete ball;
		ball = 0;
	}

	if (topPanel != 0)
	{
		delete topPanel;
		topPanel = 0;
	}

	if (leftPanel != 0)
	{
		delete leftPanel;
		leftPanel = 0;
	}
	
	if (rightPanel != 0)
	{
		delete rightPanel;
		rightPanel = 0;
	}

	if (player != 0)
	{
		delete player;
		player = 0;
	}

	if (blocks.size() > 0)
	{
		for (int i = 0; i < blocks.size(); ++i)
		{
			blocks.erase(blocks.begin() + i);
		}
	}
}

size_t Level0::getBlocksPerRow()
{
	return blocksPerRow;
}

void Level0::setBlocksPerRow(int blockCount)
{
	blocksPerRow = blockCount;
}

std::string Level0::getDefaultDataDirectoryName()
{
	return defaultDataDirectoryName;
}

void Level0::setDefaultDataDirectoryName(const char *newName)
{
	defaultDataDirectoryName = newName;
}

float Level0::getLevelMargin()
{
	return levelMargin;
}

void Level0::setLevelMargin(float marginLength)
{
	levelMargin = marginLength;
}

int Level0::getActiveBlocksCount()
{
	return this->activeBlocksCount;
}

void Level0::setActiveBlocksCount(int count)
{
	this->activeBlocksCount = count;
}