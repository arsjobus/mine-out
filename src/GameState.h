#pragma once

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <sstream>
#include <string>
#include "Log.h"
#include "PreloadResources.h"
#include "Window.h"

class GameState {
public:
	enum State {
		STATE_NULL,
		STATE_LOAD,
		STATE_INTRO,
		STATE_TITLE,
		STATE_LEVEL1,
		STATE_LEVEL2,
		STATE_LEVEL3,
		STATE_LEVEL4,
		STATE_LEVEL5,
		STATE_LEVEL6,
		STATE_LEVEL7,
		STATE_LEVEL8,
		STATE_LEVEL9,
		STATE_LEVEL10,
		STATE_CREDITS,
		STATE_EXIT,
	};

    virtual void processEvents(Window &window) = 0;
    virtual void update(Window &window, sf::Time dt) = 0;
    virtual void render(Window &window, sf::Time dt) = 0;
    virtual ~GameState() {};

public:
	Log log;

	sf::Color &getDefaultTextColor();
	void setDefaultTextColor(sf::Color newColor);

	sf::Font &getPrimaryFont();
	sf::Font &getSecondaryFont();
	sf::RectangleShape &getRefToBackground();

	sf::String &getCurrentModeName();
	void setCurrentModeName(const char *newModeName);

private:
	sf::Color defaultTextColor;
	sf::RectangleShape rectBackground;
	sf::String strCurrentModeName;

protected:
	PreloadResources resources;
};

// State status manager
void setNextState( int newState );

// State changer
void changeState(Window &window);

#endif
