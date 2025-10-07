#include "Level1.h"

Level1::Level1(Window &window) {
	loadDefaultSettings();
	log.quickWrite( LOG_INFO, std::string( getCurrentModeName() + log.getSeparator() + "Initializing mode." ) );
	loadObjects( window );
	loadBackground( window );
	loadLevelDataFromFile( "level1.dat" );
	resetAllObjects( window );
}

Level1::~Level1(void) {
	log.quickWrite( LOG_INFO, std::string( getCurrentModeName() + log.getSeparator() + "Exiting mode." ) );
	// Only unload objects when exitting level / game.
	unloadObjects();
}

void Level1::render( Window &window ) {
	Level0::render( window );
	// Add more objects to render here:
	window.display();
}

void Level1::update( Window &window ) {
	Level0::update( window );
	//If all blocks have been destoryed go to next level.
	if (this->getActiveBlocksCount() <= 0)
		setNextState( STATE_LEVEL2 );
	else
		this->setActiveBlocksCount( 0 );
}

void Level1::loadBackground( Window &window ) {
	Level0::loadBackground( window );
	// Override default background here:
}

void Level1::loadObjects( Window &window ) {
	Level0::loadObjects( window );
	// Overide default objects and add more objects here:
}

void Level1::loadDefaultSettings() {
	Level0::loadDefaultSettings();
	// Override level 0 default settings here:
	setCurrentModeName( "Level 1" );
}