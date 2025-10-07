#include "Level2.h"

Level2::Level2(Window &window) {
	loadDefaultSettings();
	log.quickWrite( LOG_INFO, std::string( getCurrentModeName() + log.getSeparator() + "Initializing mode." ) );
	loadObjects( window );
	loadBackground( window );
	loadLevelDataFromFile( "level2.dat" );
	resetAllObjects( window );
}

Level2::~Level2(void) {
	log.quickWrite( LOG_INFO, std::string( getCurrentModeName() + log.getSeparator() + "Exiting mode." ) );
	// Only unload objects when exitting level / game.
	unloadObjects();
}

void Level2::render( Window &window ) {
	Level0::render( window );
	// Add more objects to render here:
	window.display();
}

void Level2::update( Window &window ) {
	Level0::update( window );
	// If all blocks have been destoryed go to next level.
}

void Level2::loadBackground( Window &window ) {
	Level0::loadBackground( window );
	// Override default background here:
}

void Level2::loadObjects( Window &window ) {
	Level0::loadObjects( window );
	// Overide default objects and add more objects here:
}

void Level2::loadDefaultSettings() {
	Level0::loadDefaultSettings();
	// Override level 0 default settings here:
	setCurrentModeName( "Level 2" );
}
