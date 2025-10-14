#include "Level0.h"

size_t Level0::getBlocksPerRow() { return blocksPerRow; }
void Level0::setBlocksPerRow(int blockCount) { blocksPerRow = blockCount; }

std::string Level0::getDefaultDataDirectoryName() { return defaultDataDirectoryName; }
void Level0::setDefaultDataDirectoryName(const char *newName) { defaultDataDirectoryName = newName; }

float Level0::getLevelMargin() { return levelMargin; }
void Level0::setLevelMargin(float marginLength) { levelMargin = marginLength; }

int Level0::getActiveBlocksCount() { return this->activeBlocksCount; }
void Level0::setActiveBlocksCount(int count) { this->activeBlocksCount = count; }

void Level0::processEvents(Window &window) {
    while (std::optional<sf::Event> event = window.pollEvent()) {
        // Handle window close
        if (event->is<sf::Event::Closed>())
        	setNextState(GameState::State::STATE_EXIT);
        // Handle key press
        else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            auto key = keyEvent->scancode;
            if (
				key == sf::Keyboard::Scancode::Space &&
                ball->getStatus() == Ball::STUCK_TO_PLAYER && !isPaused
			) ball->setStatus(Ball::LAUNCHING);
            else if (
				(key == sf::Keyboard::Scancode::A || key == sf::Keyboard::Scancode::Left) &&
				player->getCanMoveLeft() && !isPaused
			) player->setXVelocity(-player->getSpeed());
            else if (
				(key == sf::Keyboard::Scancode::D || key == sf::Keyboard::Scancode::Right) &&
				player->getCanMoveRight() && !isPaused
			) player->setXVelocity(player->getSpeed());
            else if (key == sf::Keyboard::Scancode::R) setNextState(GameState::State::STATE_LEVEL1);
            else if (key == sf::Keyboard::Scancode::P) {
				isPaused = !isPaused;
				if (isPaused) music->pause();
				else music->play();
			}
            else if (key == sf::Keyboard::Scancode::Escape) setNextState(GameState::State::STATE_EXIT);
        }
        // Handle key release
        else if (const auto* keyRelease = event->getIf<sf::Event::KeyReleased>()) {
            auto key = keyRelease->scancode;
            if (
				key == sf::Keyboard::Scancode::A     ||
                key == sf::Keyboard::Scancode::D     ||
                key == sf::Keyboard::Scancode::Left  ||
                key == sf::Keyboard::Scancode::Right
			) player->setXVelocity(0);
        }
    }
}

void Level0::update(Window &window, sf::Time dt) {
	dt = (!isPaused) ? dt : sf::Time::Zero;
	// ::::::::::::::::::::::::
	// :::: HANDLE UPDATES ::::
	// :::::::::::::::::
	updateActiveBlockCount(); // Update active block count 
	if (isPaused) return;
	updateGameObjects(dt); // Update game objects
	updatePowerUp(); // Update the player's currently active powerup
	ball->followPaddle(player.get()); // The ball follows player's paddle until it is launched
	// :::::::::::::::::::::::::::
	// :::: DETECT COLLISIONS ::::
	// :::::::::::::::::::::::::::
	if (
		ball->getCanBounce() && detectCollisionBallAndBlock() ||
		ball->isCollisionDetected(panelL.get()) ||
		ball->isCollisionDetected(player.get()) ||
		ball->isCollisionDetected(panelR.get()) ||
		ball->isCollisionDetected(panelT.get())
	) ball->setCanBounce( false );
	else if (
		!ball->getCanBounce() && !detectCollisionBallAndBlock() &&
		!ball->isCollisionDetected(panelL.get()) &&
		!ball->isCollisionDetected(player.get()) &&
		!ball->isCollisionDetected(panelR.get()) &&
		!ball->isCollisionDetected(panelT.get())
	) ball->setCanBounce( true );
	player->isCollisionDetected(panelL.get()); // detect / handle collision between the paddle and panel L
	player->isCollisionDetected(panelR.get()); // detect / handle collision between the paddle and panel R
	detectCollisionPlayerAndPowerUp();
	detectPowerUpOutOfBounds(window);
	if (ball->isOutOfBounds(window)) resetMatch(window);
}

void Level0::render(Window& window, sf::Time dt) {
	dt = (!isPaused) ? dt : sf::Time::Zero;
	window.clear();
	window.draw(getRefToBackground());
	window.draw(*player);
	window.draw(*ball);
	window.draw(*panelT);
	window.draw(*panelL);
	window.draw(*panelR);
	for (int i = 0; i < blocks.size(); ++i)
		blocks[i]->render(window, dt);
}

bool Level0::detectCollisionBallAndBlock() {
	for (int i = 0; i < blocks.size(); ++i) {
		if (blocks[i]->getActive() && ball->getCanBounce() && ball->getGlobalBounds().findIntersection( blocks[i]->getGlobalBounds() ) ) {
			blocks[i]->setHitPoints( blocks[i]->getHitPoints() - ball->getDamage() );
			log.quickWrite(std::string( "Ball" + log.getSeparator() + "has dealt " + log.intToString(ball->getDamage()) + " points of damage to " + blocks[i]->getLabel() + "." ));
			if (ball->getPosition().y > blocks[i]->getGlobalBounds().position.y + blocks[i]->getGlobalBounds().size.y ||
				ball->getPosition().y < blocks[i]->getGlobalBounds().position.y) {
				ball->setYVelocity( -ball->getVelocity().y );
			} else
				ball->setXVelocity( -ball->getVelocity().x );
			return true;
		}
	}
	return false;
}

/**
* Detect and handle a powerup colliding with the player.
*/
void Level0::detectCollisionPlayerAndPowerUp() {
	for (int i = 0; i < blocks.size(); ++i) {
		if (!blocks[i]->getPowerUp()->getActive()) continue;
		player->isCollisionDetected(blocks[i]->getPowerUp());
	}
}

/**
*Detect and handle powerups that are outside of the window.
*/
void Level0::detectPowerUpOutOfBounds(Window &window) {
	for (int i = 0; i < blocks.size(); ++i) {
		if (blocks[i]->getActive())
			blocks[i]->getPowerUp()->detectOutOfBounds(window);
	}
}

void Level0::updateActiveBlockCount() {
	activeBlocksCount = 0;
	for (int i = 0; i < blocks.size(); ++i) {
		if (blocks[i]->getActive()) activeBlocksCount++;
	}
}

void Level0::updateGameObjects(sf::Time dt) {
	player->update(dt, panelL.get(), panelR.get());
	ball->update(dt);   // Update the ball object
	for (int i = 0; i < blocks.size(); ++i) blocks[i]->update(blocks, dt);
}

void Level0::updatePowerUp() {
	// Update active powerup
	if (player->getActivePowerUp() == PowerUp::TypeID::IncreasedDamage) {
		ball->setDamage( 5 );
	} else ball->setDamage( 1 );

	if (player->getActivePowerUp() == PowerUp::TypeID::GrowPaddle) {
	player->resizeLarge(panelL.get(), panelR.get());
	} else player->resizeSmall();
}

void Level0::loadBackground(Window &window) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading background.."));
	sf::Vector2f screenResolution(window.getScreenResolution().x, window.getScreenResolution().y);
	getRefToBackground().setTexture( &resources.getBackgroundTexture(0) );
	getRefToBackground().setSize(sf::Vector2f(screenResolution.x, screenResolution.y));
    //getRefToBackground().setFillColor(sf::Color::White);
    getRefToBackground().setOrigin(sf::Vector2f(getRefToBackground().getGlobalBounds().size.x / 2, getRefToBackground().getGlobalBounds().size.y / 2));
	getRefToBackground().setPosition(sf::Vector2f(screenResolution.x / 2, screenResolution.y / 2));
}

void Level0::loadBall(float radius) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the ball.."));
	ball = std::make_unique<Ball>(radius);
}

void Level0::loadDefaultSettings() {
	music = std::make_unique<sf::Sound>(resources.getBufferedMusic(0));
	music->setLooping(true);
	music->play();
	activeBlocksCount = 0;
	setBlocksPerRow(15); // Set the blocks per row to 15
	setDefaultDataDirectoryName("data"); // Set default data directory name
	setLevelMargin(10.f); // Set the level margin to 10px
	isPaused = false;
}

void Level0::loadPanelL(int newWidth, int newHeight) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the left panel.."));
	panelL = std::make_unique<PanelL>(newWidth, newHeight);
}

void Level0::loadPanelR(int newWidth, int newHeight) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the right panel.."));
	panelR = std::make_unique<PanelR>(newWidth, newHeight);
}

void Level0::loadPanelT(int newWidth, int newHeight) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the top panel.."));
	panelT = std::make_unique<PanelT>(newWidth, newHeight);
}

void Level0::loadLevelDataFromFile(const char *filename) {
	// Craft the file path
	std::string strFilePath = (sf::String(getDefaultDataDirectoryName() + "/" + filename));
	// Open the file by using the crafted file path
	std::ifstream levelDataFile(strFilePath.c_str());
	// If the file cannot be open then stop the game until it is replaced
	if (!levelDataFile.is_open()) {
		log.quickWrite(LOG_ERROR, std::string(getCurrentModeName() + log.getSeparator() + "Unable to open level data file: " + filename));
		// Set the next state to exit the game.
		setNextState( STATE_EXIT );
	} else {
		std::string strDataEntry;
		std::shared_ptr<NormalBlock> ptrNormalBlock;
		std::shared_ptr<StoneBlock>  ptrStoneBlock;
		std::shared_ptr<GoldBlock>   ptrGoldBlock;
		std::shared_ptr<TNTBlock>    ptrTNTBlock;
		while(!levelDataFile.eof()) {
			// Read data into memory
			std::getline(levelDataFile, strDataEntry, ',');
			if (std::strcmp(strDataEntry.c_str(), ":normal:") == 0) {
				ptrNormalBlock = std::make_shared<NormalBlock>();
				blocks.push_back(ptrNormalBlock);
			}
			else if (std::strcmp(strDataEntry.c_str(), ":stone:") == 0) {
				ptrStoneBlock = std::make_shared<StoneBlock>();
				blocks.push_back(ptrStoneBlock);
			}
			else if (std::strcmp(strDataEntry.c_str(), ":gold:" ) == 0) {
				ptrGoldBlock = std::make_shared<GoldBlock>();
				blocks.push_back(ptrGoldBlock);
			}
			else if (std::strcmp(strDataEntry.c_str(), ":tnt:" ) == 0) {
				ptrTNTBlock = std::make_shared<TNTBlock>();
				blocks.push_back(ptrTNTBlock);
			}
			else if (std::strcmp(strDataEntry.c_str(), ":blank:" ) == 0) {
				ptrNormalBlock = std::make_shared<NormalBlock>();
				ptrNormalBlock->setActive( false );
				ptrNormalBlock->setHasDroppedPowerUp( true );
				blocks.push_back(ptrNormalBlock);
			}
		}
	}
}

void Level0::loadObjects(Window &window) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading objects.."));
	loadBackground(window);
	loadPanelT(window.getScreenResolution().x, 100);
	loadPanelR(panelT->getSize().y / 2, window.getScreenResolution().y - panelT->getSize().y);
	loadPanelL(panelT->getSize().y / 2, window.getScreenResolution().y - panelT->getSize().y);
	loadBall(8);
	loadPaddle(96, 16);
}

void Level0::loadPaddle(int paddleWidth, int paddleHeight) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading the player's paddle.."));
	player = std::make_unique<Paddle>(paddleWidth, paddleHeight);
}

void Level0::resetAllObjects(Window &window) {
	// Always reset the top panel before the left panel and right panel
	resetPanelT(window);
	resetPanelL(window);
	resetPanelR(window);
	// Always reset the player before the ball.
	resetPlayer(window);
	resetBall(window);
	resetBlocks(window);
}

void Level0::resetMatch(Window &window) {
	resetBall(window);
	resetPlayer(window);
}

void Level0::resetBall(Window &window) {
	// Dependent on the position of the player
	sf::Vector2f position(player->getPosition().x, player->getGlobalBounds().position.y - ball->getRadius());
	std::string strXPosition = log.floatToString(position.x);
	std::string strYPosition = log.floatToString(position.y);
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Resetting position of the ball: " + "X = " + strXPosition + "; Y = " + strYPosition ));
	ball->setStatus( Ball::Status::STUCK_TO_PLAYER );
	ball->setPosition(position);
	defaultBallSpeed = 500;
}

void Level0::resetBlocks(Window &window) {
	int blockCounter = 0;
	float padding = 2;
	int xPosition = panelL->getSize().x + getLevelMargin();
	int yPosition = panelT->getSize().y + getLevelMargin();
	float blockWidth = ((window.getSize().x - (panelL->getSize().x + (getLevelMargin() * 2.f) + panelR->getSize().x)) / (getBlocksPerRow()) - padding);
	float blockHeight = 32.f;
	for (int i = 0; i < blocks.size(); ++i) {
		blocks[i]->setSize(sf::Vector2f(blockWidth, blockHeight));
		blocks[i]->setOrigin(sf::Vector2f(blocks[i]->getGlobalBounds().size.x / 2, blocks[i]->getGlobalBounds().size.y / 2));
		if (++blockCounter == 1)
			xPosition += blocks[i]->getOrigin().x;
		else if ((blockCounter - 1) % getBlocksPerRow() == 0) {
			xPosition = panelL->getSize().x + getLevelMargin() + blocks[i]->getOrigin().x;
			yPosition += blocks[i]->getGlobalBounds().size.y + getLevelMargin();
		}
		else
			xPosition += padding + blocks[i]->getSize().x;
		blocks[i]->setPosition(sf::Vector2f(xPosition + padding, yPosition + blocks[i]->getOrigin().y));
	}
}

void Level0::resetPanelL(Window &window) {
	// Dependent on the top panel
	sf::Vector2f position(
		panelL->getOrigin().x,
		window.getScreenResolution().y / 2 + panelT->getOrigin().y
	);
	std::string strXPosition = log.floatToString(position.x);
	std::string strYPosition = log.floatToString(position.y);
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Resetting position of the left panel: " + "X = " + strXPosition + "; Y = " + strYPosition ));
	panelL->setPosition(position);
}

void Level0::resetPanelT(Window &window) {
	sf::Vector2f position(window.getScreenResolution().x / 2, panelT->getSize().y - panelT->getOrigin().y);
	std::string strXPosition = log.floatToString(position.x);
	std::string strYPosition = log.floatToString(position.y);
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Resetting position of the top panel: " + "X = " + strXPosition + "; Y = " + strYPosition ));
	panelT->setPosition(position);
}

void Level0::resetPanelR(Window &window) {
	// Dependent on the top panel
	sf::Vector2f position(
		window.getScreenResolution().x - panelR->getOrigin().x,
		window.getScreenResolution().y / 2 + panelT->getOrigin().y
	);
	std::string strXPosition = log.floatToString(position.x);
	std::string strYPosition = log.floatToString(position.y);
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Resetting position of the right panel: " + "X = " + strXPosition + "; Y = " + strYPosition ));
	panelR->setPosition(position);
}

void Level0::resetPlayer(Window &window) {
	// Dependent on the position of the bottom panel
	sf::Vector2f position(window.getScreenResolution().x / 2, window.getScreenResolution().y - player->getOrigin().y - 16);
	std::string strXPosition = log.floatToString(position.x);
	std::string strYPosition = log.floatToString(position.y);
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Resetting position of the paddle: " + "X = " + strXPosition + "; Y = " + strYPosition ));
	player->setActivePowerUp( PowerUp::TypeID::None );
	player->setPosition(position);
	player->setXVelocity( 0.f );
	player->setYVelocity( 0.f );
	this->defaultPaddleSpeed = 500;
}

void Level0::unloadObjects() {

}