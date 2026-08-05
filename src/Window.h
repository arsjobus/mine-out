#pragma once

#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
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
	bool handleScreenshotHotkey(const sf::Event& event);
	void saveScreenshot(const std::string& prefix = "screenshot");

	std::optional<sf::Event> pollEvent();
	void updateLetterboxView(unsigned int windowWidth, unsigned int windowHeight);

private:
	void loadFromConfig();

	Log log;
	bool bFullScreen;
	bool screenshotKeyHeld;

private:
	sf::Vector2u uMinimumScreenResolution;
	sf::Vector2u uScreenResolution;
	sf::Vector2u uDefaultScreenResolution;
	sf::String defaultWindowTitle;
	Config config;
};

#endif