#include "GameState.h"

sf::String &GameState::getFontDirectoryName() { return strFontDirectoryName; }
void GameState::setFontDirectoryName(const char *newDirectoryName) { strFontDirectoryName = newDirectoryName; }
sf::Font &GameState::getPrimaryFont() { return fntPrimaryFont; }
sf::Color &GameState::getDefaultTextColor() { return defaultTextColor; }
void GameState::setDefaultTextColor(sf::Color newColor) { defaultTextColor = newColor; }

void GameState::setPrimaryFont(const std::string &relativePathToFile) {
	if (!fntPrimaryFont.openFromFile(relativePathToFile))
		log.quickWrite(LOG_WARNING, std::string(getCurrentModeName() + log.getSeparator() + "Could not load primary font."));
	else {
		log.quickWrite(LOG_SUCCESS, std::string(getCurrentModeName() + log.getSeparator() + "Primary font was loaded!"));
	}
}

sf::String &GameState::getCurrentModeName() { return strCurrentModeName; }
void GameState::setCurrentModeName(const char *newModeName) { strCurrentModeName = newModeName; }
sf::String &GameState::getPrimaryFontName() { return strPrimaryFontName; }
void GameState::setPrimaryFontName(const char *newFontName) { strPrimaryFontName = newFontName; }
sf::Font &GameState::getSecondaryFont() { return fntSecondaryFont; }

void GameState::setSecondaryFont(const std::string &relativePathToFile) {
	if (!fntSecondaryFont.openFromFile(relativePathToFile))
		log.quickWrite(LOG_WARNING, "Could not load secondary font.");
	else
		log.quickWrite(LOG_SUCCESS, "Secondary font was loaded!");
}

sf::String &GameState::getSecondaryFontName() { return strSecondaryFontName; }
void GameState::setSecondaryFontName(const char *newFontName) { strSecondaryFontName = newFontName; }
sf::RectangleShape &GameState::getRefToBackground() { return rectBackground; }