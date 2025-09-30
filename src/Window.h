#pragma once

#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Log.h"

class Window: public sf::RenderWindow
{
public:
	Window(void);
	~Window(void);

	bool getFullScreen();
	sf::String &getDefaultWindowTitle();
	sf::Vector2u &getScreenResolution();

private:
	void loadFromConfig();

	Log log;
	bool bFullScreen;

private:
	sf::Vector2u uMinimumScreenResolution;
	sf::Vector2u uScreenResolution;
	sf::Vector2u uDefaultScreenResolution;
	sf::String defaultWindowTitle;
	Config config;
};

#endif