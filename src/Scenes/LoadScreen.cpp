#include "LoadScreen.h"

LoadScreen::LoadScreen(Window &window) {
	setCurrentModeName("Loading Screen");
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Initializing mode."));
	initialize();
	initilizeBackground(window);
	initilizeObjects(window);
}

LoadScreen::~LoadScreen(void) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Exiting mode."));
}

void LoadScreen::processEvents(Window &window) {
	while (std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
        	setNextState(STATE_EXIT);
	}
}

void LoadScreen::update(Window &window)
{
	if (resources.getLoadedResourceCount() < resources.getResourceCount()) {
		if (loadedAnimationsCount < animationTextureNames.size()) {
			if (loadedAnimationsCount == 0 && !animationsAreLoading) {
				textLoadingStatus->setString("Loading Animations..");
				animationsAreLoading = true;
			}
			else resources.loadAnimationTexture(animationTextureNames[loadedAnimationsCount++].c_str());
		}
		else if (loadedPaddlesCount < paddleTextureNames.size()) {
			if (loadedPaddlesCount == 0 && !paddlesAreLoading) {
				textLoadingStatus->setString("Loading Paddles..");
				paddlesAreLoading = true;
			}
			else resources.loadPaddleTexture(paddleTextureNames[loadedPaddlesCount++].c_str());
		}
		else if (loadedBlocksCount < blockTextureNames.size()) {
			if (loadedBlocksCount == 0 && !blocksAreLoading) {
				textLoadingStatus->setString("Loading Blocks..");
				blocksAreLoading = true;
			}
			else resources.loadBlockTexture(blockTextureNames[loadedBlocksCount++].c_str());
		}
		else if (loadedPowerupCount < powerupTextureNames.size()) {
			if (loadedPowerupCount == 0 && !powerupsAreLoading) {
				textLoadingStatus->setString("Loading Powerups..");
				powerupsAreLoading = true;
			}
			else resources.loadPowerupTexture(powerupTextureNames[loadedPowerupCount++].c_str());
		}
		else if (loadedLevelCount < levelTextureNames.size()) {
			if (loadedLevelCount == 0 && !levelsAreLoading) {
				textLoadingStatus->setString("Loading Level Resources..");
				levelsAreLoading = true;
			}
			else resources.loadLevelTexture(levelTextureNames[loadedLevelCount++].c_str());
		}
		else if (loadedSoundCount < soundNames.size()) {
			if (loadedSoundCount == 0 && !soundsAreLoading) {
				textLoadingStatus->setString("Loading Sound Effects..");
				soundsAreLoading = true;
			}
			else resources.loadSound(soundNames[loadedSoundCount++].c_str());
		}
		// Update the appearance of the loading bar.
		rectLoadBar.setSize(sf::Vector2f(resources.getLoadPercentile() / 100.f, 32));
		rectLoadBar.setOrigin(sf::Vector2f(rectLoadBar.getGlobalBounds().size.x / 2, rectLoadBar.getGlobalBounds().size.y / 2));
		rectLoadBar.setPosition(
			sf::Vector2f(
				(rectLoadBarBackground.getPosition().x - rectLoadBarBackground.getOrigin().x) + rectLoadBar.getOrigin().x + 3, 
				(rectLoadBarBackground.getPosition().y - rectLoadBarBackground.getOrigin().y) + rectLoadBar.getOrigin().y + 3
			)
		);
	}
	else setNextState( STATE_TITLE );
	textLoadingStatus->setOrigin(sf::Vector2f(textLoadingStatus->getGlobalBounds().size.x / 2, textLoadingStatus->getGlobalBounds().size.y / 2));
	textLoadingStatus->setPosition(
		sf::Vector2f(
			rectLoadBarBackground.getPosition().x, 
			rectLoadBarBackground.getPosition().y + rectLoadBarBackground.getOrigin().y + 10
		)
	);
}

void LoadScreen::render(Window &window) {
	window.clear();
	window.draw(getRefToBackground());
	window.draw(rectLoadBarBackground);
	window.draw(rectLoadBar);
	window.draw(*textLoadingStatus);
	window.display();
}

void LoadScreen::initialize() {
	animationsAreLoading = false;
	blocksAreLoading = false;
	levelsAreLoading = false;
	paddlesAreLoading = false;
	powerupsAreLoading = false;
	soundsAreLoading = false;
	loadedAnimationsCount = 0;
	loadedPaddlesCount = 0;
	loadedBlocksCount = 0;
	loadedLevelCount = 0;
	loadedPowerupCount = 0;
	loadedSoundCount = 0;

	animationTextureNames.push_back("explosion1");// id: 0
	blockTextureNames.push_back("block-default"); // id: 0
	blockTextureNames.push_back("block-stone1");  // id: 1
	blockTextureNames.push_back("block-stone2");  // id: 2
	blockTextureNames.push_back("block-stone3");  // id: 3
	blockTextureNames.push_back("block-normal1"); // id: 4
	blockTextureNames.push_back("block-gold1");   // id: 5
	blockTextureNames.push_back("block-tnt1");    // id: 6
	paddleTextureNames.push_back("paddle");       // id: 0
	levelTextureNames.push_back("level-top");     // id: 0
	levelTextureNames.push_back("level-left");    // id: 1
	levelTextureNames.push_back("level-right");   // id: 2
	powerupTextureNames.push_back("powerup-increased-damage"); // id: 0
	powerupTextureNames.push_back("powerup-grow-paddle"); // id: 1
	soundNames.push_back("paddle-hit"); // id: 0
	soundNames.push_back("explosion");  // id: 1
	resources.setResourceCount(resources.getResourceCount() + 
		blockTextureNames.size() + 
		animationTextureNames.size() +
		paddleTextureNames.size() +
		powerupTextureNames.size() +
		levelTextureNames.size() +
		soundNames.size());
}

void LoadScreen::initilizeBackground(Window &window) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading background.."));
	sf::Vector2f screenResolution(window.getScreenResolution().x, window.getScreenResolution().y);
	getRefToBackground().setSize(sf::Vector2f(screenResolution.x, screenResolution.y));
    getRefToBackground().setFillColor(sf::Color(224, 224, 224));
    getRefToBackground().setOrigin(sf::Vector2f(getRefToBackground().getGlobalBounds().size.x / 2, getRefToBackground().getGlobalBounds().size.y / 2));
	getRefToBackground().setPosition(sf::Vector2f(screenResolution.x / 2, screenResolution.y / 2));
}

void LoadScreen::initilizeObjects(Window &window) {
	rectLoadBarBackground.setFillColor(sf::Color::Black);
	rectLoadBarBackground.setSize(sf::Vector2f(106, 38));
	rectLoadBarBackground.setOrigin(sf::Vector2f(rectLoadBarBackground.getGlobalBounds().size.x / 2, rectLoadBarBackground.getGlobalBounds().size.y / 2));
	rectLoadBarBackground.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

	setPrimaryFont(std::string("fonts/arial.ttf"));

	textLoadingStatus = new sf::Text(getPrimaryFont(), "");

	textLoadingStatus->setFont(getPrimaryFont());
	textLoadingStatus->setFillColor(sf::Color::Black);
	textLoadingStatus->setString("Loading..");
	textLoadingStatus->setCharacterSize(24);
	textLoadingStatus->setOrigin(sf::Vector2f(textLoadingStatus->getGlobalBounds().size.x / 2, textLoadingStatus->getGlobalBounds().size.y / 2));
	textLoadingStatus->setPosition(sf::Vector2f(rectLoadBarBackground.getPosition().x, rectLoadBarBackground.getPosition().y + rectLoadBarBackground.getOrigin().y + 10));

	rectLoadBar.setFillColor(sf::Color(153, 0, 76));
	rectLoadBar.setSize(sf::Vector2f(0, 32));
	rectLoadBar.setOrigin(sf::Vector2f(rectLoadBar.getGlobalBounds().size.x / 2, rectLoadBar.getGlobalBounds().size.y / 2));
	rectLoadBar.setPosition(
		sf::Vector2f(
			(rectLoadBarBackground.getPosition().x - rectLoadBarBackground.getOrigin().x) + rectLoadBar.getOrigin().x + 3, 
			(rectLoadBarBackground.getPosition().y - rectLoadBarBackground.getOrigin().y) + rectLoadBar.getOrigin().y + 3
		)
	);
}