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

void LevelX::render( Window &window ) {
	Level0::render( window );
	// Add more objects to render here:
	window.display();
}

void LevelX::update( Window &window ) {
	Level0::update( window );
	//If all blocks have been destoryed go to next level.
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
	setCurrentModeName( "Level X" );
}