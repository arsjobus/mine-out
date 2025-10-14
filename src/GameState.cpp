#include "GameState.h"

sf::Font &GameState::getPrimaryFont() { return resources.getFont(0);}
sf::Color &GameState::getDefaultTextColor() { return defaultTextColor; }
void GameState::setDefaultTextColor(sf::Color newColor) { defaultTextColor = newColor; }
sf::String &GameState::getCurrentModeName() { return strCurrentModeName; }
void GameState::setCurrentModeName(const char *newModeName) { strCurrentModeName = newModeName; }
sf::Font &GameState::getSecondaryFont() { return resources.getFont(1); }
sf::RectangleShape &GameState::getRefToBackground() { return rectBackground; }