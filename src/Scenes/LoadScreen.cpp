#include "LoadScreen.h"
#include <algorithm>

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
        if (window.handleScreenshotHotkey(*event)) {
            continue;
        }
        if (event->is<sf::Event::Closed>())
        	setNextState(STATE_EXIT);
	}
}

void LoadScreen::update(Window &window, sf::Time dt)
{
	if (resources.getLoadedResourceCount() < resources.getResourceCount()) {
		if (loadedAnimationsCount < animationTextureNames.size()) {
			if (loadedAnimationsCount == 0 && !animationsAreLoading) {
				//textLoadingStatus->setString("Loading Animations..");
				animationsAreLoading = true;
			}
			else resources.loadAnimationTexture(animationTextureNames[loadedAnimationsCount++].c_str());
		}
		else if (loadedBackgroundsCount < backgroundTextureNames.size()) {
			if (loadedBackgroundsCount == 0 && !backgroundsAreLoading) {
				//textLoadingStatus->setString("Loading Backgrounds..");
				backgroundsAreLoading = true;
			}
			else resources.loadBackgroundTexture(backgroundTextureNames[loadedBackgroundsCount++].c_str());
		}
		else if (loadedFontsCount < fontNames.size()) {
			if (loadedFontsCount == 0 && !fontsAreLoading) {
				//textLoadingStatus->setString("Loading Fonts..");
				fontsAreLoading = true;
			}
			else resources.loadFont(fontNames[loadedFontsCount++].c_str());
		}
		else if (loadedPaddlesCount < paddleTextureNames.size()) {
			if (loadedPaddlesCount == 0 && !paddlesAreLoading) {
				//textLoadingStatus->setString("Loading Paddles..");
				paddlesAreLoading = true;
			}
			else resources.loadPaddleTexture(paddleTextureNames[loadedPaddlesCount++].c_str());
		}
		else if (loadedBlocksCount < blockTextureNames.size()) {
			if (loadedBlocksCount == 0 && !blocksAreLoading) {
				//textLoadingStatus->setString("Loading Blocks..");
				blocksAreLoading = true;
			}
			else resources.loadBlockTexture(blockTextureNames[loadedBlocksCount++].c_str());
		}
		else if (loadedPowerupCount < powerupTextureNames.size()) {
			if (loadedPowerupCount == 0 && !powerupsAreLoading) {
				//textLoadingStatus->setString("Loading Powerups..");
				powerupsAreLoading = true;
			}
			else resources.loadPowerupTexture(powerupTextureNames[loadedPowerupCount++].c_str());
		}
		else if (loadedLevelCount < levelTextureNames.size()) {
			if (loadedLevelCount == 0 && !levelsAreLoading) {
				//textLoadingStatus->setString("Loading Level Resources..");
				levelsAreLoading = true;
			}
			else resources.loadLevelTexture(levelTextureNames[loadedLevelCount++].c_str());
		}
		else if (loadedSoundCount < soundNames.size()) {
			if (loadedSoundCount == 0 && !soundsAreLoading) {
				//textLoadingStatus->setString("Loading Sound Effects..");
				soundsAreLoading = true;
			}
			else resources.loadSound(soundNames[loadedSoundCount++].c_str());
		}
		else if (loadedMusicCount < musicNames.size()) {
			if (loadedMusicCount == 0 && !musicsAreLoading) {
				//textLoadingStatus->setString("Loading Music..");
				musicsAreLoading = true;
			}
			else resources.loadMusic(musicNames[loadedMusicCount++].c_str());
		}
		// Update displayed percent while resources are still loading.
		float actualPercent = resources.getLoadPercentile();
		float elapsed = loadClock.getElapsedTime().asSeconds();
		// Allow the bar to grow at most proportionally to elapsed time so it doesn't instantly fill.
		float allowedPercentByTime = std::min(100.f, (elapsed / minDisplaySeconds) * 100.f);
		float targetPercent = std::min(actualPercent, allowedPercentByTime);
		// Smoothly step displayedPercent towards targetPercent (avoid large jumps)
		if (targetPercent > displayedPercent) {
			// Increase with a small smoothing factor
			displayedPercent = std::min(targetPercent, displayedPercent + (targetPercent - displayedPercent) * 0.5f + 0.5f);
		} else {
			displayedPercent = std::max(displayedPercent, targetPercent);
		}

		// Compute pixel width based on background width minus padding (3px each side).
		float bgWidth = rectLoadBarBackground.getSize().x;
		float padding = 6.f; // total horizontal padding inside the background
		float usableWidth = std::max(0.f, bgWidth - padding);
		float pct = displayedPercent / 100.f;
		float barWidth = usableWidth * pct;
		rectLoadBar.setSize(sf::Vector2f(barWidth, 32.f));
		rectLoadBar.setOrigin(sf::Vector2f(rectLoadBar.getSize().x / 2.f, rectLoadBar.getSize().y / 2.f));
		rectLoadBar.setPosition(
			sf::Vector2f(
				(rectLoadBarBackground.getPosition().x - rectLoadBarBackground.getOrigin().x) + rectLoadBar.getOrigin().x + 3.f,
				(rectLoadBarBackground.getPosition().y - rectLoadBarBackground.getOrigin().y) + rectLoadBar.getOrigin().y + 3.f
			)
		);
	}
	else {
		// Resources finished loading; ensure the loading screen remains
		// visible for at least `minDisplaySeconds` before switching states.
		if (!resourcesFinished) {
			resourcesFinished = true;
			// Prepare animation from current displayedPercent to 100 over remaining time
			animatingToFull = false;
		}
		// Start or progress animation to full bar if needed
		float elapsedTotal = loadClock.getElapsedTime().asSeconds();
		if (!animatingToFull) {
			animationStartPercent = displayedPercent;
			animationStartTimeSec = elapsedTotal;
			animationDurationSec = std::max(0.001f, minDisplaySeconds - elapsedTotal);
			animatingToFull = true;
		}
		float t = (elapsedTotal - animationStartTimeSec) / animationDurationSec;
		t = std::clamp(t, 0.f, 1.f);
		displayedPercent = animationStartPercent + (100.f - animationStartPercent) * t;

		// Update bar size from displayedPercent
		float bgWidth = rectLoadBarBackground.getSize().x;
		float padding = 6.f;
		float usableWidth = std::max(0.f, bgWidth - padding);
		float barWidth = usableWidth * (displayedPercent / 100.f);
		rectLoadBar.setSize(sf::Vector2f(barWidth, 32.f));
		rectLoadBar.setOrigin(sf::Vector2f(rectLoadBar.getSize().x / 2.f, rectLoadBar.getSize().y / 2.f));
		rectLoadBar.setPosition(
			sf::Vector2f(
				(rectLoadBarBackground.getPosition().x - rectLoadBarBackground.getOrigin().x) + rectLoadBar.getOrigin().x + 3.f,
				(rectLoadBarBackground.getPosition().y - rectLoadBarBackground.getOrigin().y) + rectLoadBar.getOrigin().y + 3.f
			)
		);

		// If the animated bar has visually reached full, transition immediately.
		if (displayedPercent >= 99.9f) {
			setNextState( STATE_TITLE );
		}
	}
	// textLoadingStatus->setOrigin(sf::Vector2f(textLoadingStatus->getGlobalBounds().size.x / 2, textLoadingStatus->getGlobalBounds().size.y / 2));
	// textLoadingStatus->setPosition(
	// 	sf::Vector2f(
	// 		rectLoadBarBackground.getPosition().x, 
	// 		rectLoadBarBackground.getPosition().y + rectLoadBarBackground.getOrigin().y + 10
	// 	)
	// );
}

void LoadScreen::render(Window &window, sf::Time dt) {
	window.clear();
	window.draw(getRefToBackground());
	window.draw(rectLoadBarBackground);
	window.draw(rectLoadBar);
	// window.draw(*textLoadingStatus);
	window.display();
}

void LoadScreen::initialize() {
	animationsAreLoading = false;
	backgroundsAreLoading = false;
	blocksAreLoading = false;
	levelsAreLoading = false;
	paddlesAreLoading = false;
	powerupsAreLoading = false;
	soundsAreLoading = false;
	musicsAreLoading = false;
	fontsAreLoading = false;
	loadedFontsCount = 0;
	loadedAnimationsCount = 0;
	loadedBackgroundsCount = 0;
	loadedPaddlesCount = 0;
	loadedBlocksCount = 0;
	loadedLevelCount = 0;
	loadedPowerupCount = 0;
	loadedSoundCount = 0;
	loadedMusicCount = 0;

	animationTextureNames.push_back("spr_explosion");   // id: 0
	backgroundTextureNames.push_back("spr_background"); // id: 0
	blockTextureNames.push_back("spr_block_default");   // id: 0
	blockTextureNames.push_back("spr_block_rock01");    // id: 1
	blockTextureNames.push_back("spr_block_rock02");    // id: 2
	blockTextureNames.push_back("spr_block_rock03");    // id: 3
	blockTextureNames.push_back("spr_block_dirt");      // id: 4
	blockTextureNames.push_back("spr_block_gold");      // id: 5
	blockTextureNames.push_back("spr_block_bomb");      // id: 6
	paddleTextureNames.push_back("spr_paddle_sm");      // id: 0
	paddleTextureNames.push_back("spr_paddle_lg");      // id: 1
	levelTextureNames.push_back("spr_level_top");       // id: 0
	levelTextureNames.push_back("spr_level_left");      // id: 1
	levelTextureNames.push_back("spr_level_right");     // id: 2
	powerupTextureNames.push_back("spr_powerup_increased_damage"); // id: 0
	powerupTextureNames.push_back("spr_powerup_grow_paddle"); // id: 1
	powerupTextureNames.push_back("spr_powerup_extra_paddle"); // id: 2
	musicNames.push_back("bgm_crystal_cave"); // id: 0
	soundNames.push_back("sfx_paddle_hit"); // id: 0
	soundNames.push_back("sfx_explosion");  // id: 1
	fontNames.push_back("fnt_quadrangle"); // id: 0
	fontNames.push_back("fnt_liberation_sans"); // id: 1
	resources.setResourceCount(resources.getResourceCount() + 
		blockTextureNames.size() + 
		animationTextureNames.size() +
		backgroundTextureNames.size() +
		paddleTextureNames.size() +
		powerupTextureNames.size() +
		levelTextureNames.size() +
		soundNames.size() +
		musicNames.size() + 
		fontNames.size());

	// Start timing the loading screen display
	loadClock.restart();
	resourcesFinished = false;
}

void LoadScreen::initilizeBackground(Window &window) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading background.."));
	sf::Vector2f screenResolution(window.getScreenResolution().x, window.getScreenResolution().y);
	getRefToBackground().setSize(sf::Vector2f(screenResolution.x, screenResolution.y));
	// Use black background to avoid a visible light flash on startup
	getRefToBackground().setFillColor(sf::Color::Black);
    getRefToBackground().setOrigin(sf::Vector2f(getRefToBackground().getGlobalBounds().size.x / 2, getRefToBackground().getGlobalBounds().size.y / 2));
	getRefToBackground().setPosition(sf::Vector2f(screenResolution.x / 2, screenResolution.y / 2));
}

void LoadScreen::initilizeObjects(Window &window) {
	rectLoadBarBackground.setFillColor(sf::Color::Black);
	rectLoadBarBackground.setSize(sf::Vector2f(106, 38));
	rectLoadBarBackground.setOrigin(sf::Vector2f(rectLoadBarBackground.getGlobalBounds().size.x / 2, rectLoadBarBackground.getGlobalBounds().size.y / 2));
	rectLoadBarBackground.setPosition(sf::Vector2f(static_cast<float>(window.getScreenResolution().x) / 2.f, static_cast<float>(window.getScreenResolution().y) / 2.f));

	// textLoadingStatus = std::make_unique<sf::Text>(getPrimaryFont(), "");
	// textLoadingStatus->setFont(getPrimaryFont());
	// textLoadingStatus->setFillColor(sf::Color::Black);
	// textLoadingStatus->setString("Loading..");
	// textLoadingStatus->setCharacterSize(24);
	// textLoadingStatus->setOrigin(sf::Vector2f(textLoadingStatus->getGlobalBounds().size.x / 2, textLoadingStatus->getGlobalBounds().size.y / 2));
	// textLoadingStatus->setPosition(sf::Vector2f(rectLoadBarBackground.getPosition().x, rectLoadBarBackground.getPosition().y + rectLoadBarBackground.getOrigin().y + 10));

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