#include "LevelX.h"

LevelX::LevelX(Window &window, const char *filename, GameState::State nextState) {
	nextGameState = nextState;
	loadDefaultSettings();
	log.quickWrite( LOG_INFO, std::string( getCurrentModeName() + log.getSeparator() + "Initializing mode." ) );
	loadObjects( window );
	loadBackground( window );
	loadLevelDataFromFile( filename );
	resetAllObjects( window );
}

LevelX::~LevelX(void) {
	log.quickWrite( LOG_INFO, std::string( getCurrentModeName() + log.getSeparator() + "Exiting mode." ) );
	// Only unload objects when exitting level / game.
	unloadObjects();
}

void LevelX::render( Window &window, sf::Time dt ) {
	Level0::render( window, dt );
	float fps = 1.f / dt.asSeconds();
	//fpsText->setString("FPS: " + std::to_string(static_cast<int>(fps)));
	// Add more objects to render here:
	///window.draw(*fpsText);
	window.display();
}

void LevelX::update( Window &window, sf::Time dt ) {
	Level0::update( window, dt );
	if (this->getActiveBlocksCount() <= 0)
		setNextState( nextGameState );
	else
		this->setActiveBlocksCount( 0 );
}

void LevelX::loadBackground( Window &window ) {
	Level0::loadBackground( window );
	// Override default background here:
}

void LevelX::loadObjects( Window &window ) {
	Level0::loadObjects( window );
	// Overide default objects and add more objects here:
}

void LevelX::loadDefaultSettings() {
	Level0::loadDefaultSettings();
	// Override level 0 default settings here:
	// fpsText = std::make_unique<sf::Text>(resources.getFont(0), "", 10);
    // fpsText->setFillColor(sf::Color::White);
    // fpsText->setPosition(sf::Vector2f(10.f, 10.f));
	setCurrentModeName( "Level X" );
}