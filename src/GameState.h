#pragma once

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>
#include "Log.h"
#include "PreloadResources.h"
#include "Window.h"

class GameState {
public:
	static enum State {
		STATE_NULL,
		STATE_LOAD,
		STATE_INTRO,
		STATE_TITLE,
		STATE_LEVEL1,
		STATE_LEVEL2,
		STATE_EXIT,
	};

    virtual void processEvents(Window &window) = 0;
    virtual void update(Window &window) = 0;
    virtual void render(Window &window) = 0;
    virtual ~GameState() {};

public:
	Log log;

	sf::Color &getDefaultTextColor();
	void setDefaultTextColor(sf::Color newColor);

	sf::Font &getPrimaryFont();
	void setPrimaryFont(std::string &pathToFile);

	sf::Font &getSecondaryFont();
	void setSecondaryFont(std::string &pathToFile);

	sf::RectangleShape &getRefToBackground();

	sf::String &getCurrentModeName();
	void setCurrentModeName(const char *newModeName);

	sf::String &getFontDirectoryName();
	void setFontDirectoryName(const char *newDirectoryName);

	sf::String &getPrimaryFontName();
	void setPrimaryFontName(const char *newFontName);

	sf::String &getSecondaryFontName();
	void setSecondaryFontName(const char *newFontName);

private:
	sf::Color defaultTextColor;
	sf::Font fntPrimaryFont;
	sf::Font fntSecondaryFont;
	sf::RectangleShape rectBackground;
	sf::String strCurrentModeName;
	sf::String strFontDirectoryName;
	sf::String strPrimaryFontName;
	sf::String strSecondaryFontName;

protected:
	PreloadResources resources;
};

// State status manager
void setNextState( int newState );

// State changer
void changeState(Window &window);

#endif
