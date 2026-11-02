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
#include "TitleScene.h"

class Window: public sf::RenderWindow, public TitleSceneRenderer
{
public:
	Window(void);
	~Window(void);

	bool getFullScreen();
	bool hasFocus() const;
	sf::String &getDefaultWindowTitle();
	sf::Vector2u &getScreenResolution();
	bool handleScreenshotHotkey(const sf::Event& event);
	void saveScreenshot(const std::string& prefix = "screenshot");

	std::optional<sf::Event> pollEvent();
	void updateLetterboxView(unsigned int windowWidth, unsigned int windowHeight);

	void beginFrame() override;
	void drawCenteredText(
		const std::string& value,
		TitleSceneRenderer::Font font,
		unsigned int characterSize,
		unsigned char red,
		unsigned char green,
		unsigned char blue,
		float x,
		float y,
		bool bold,
		float outlineThickness
	) override;
	void present() override;

private:
	void loadFromConfig();

	Log log;
	bool bFullScreen;
	bool screenshotKeyHeld;
	bool focused;

private:
	sf::Vector2u uMinimumScreenResolution;
	sf::Vector2u uScreenResolution;
	sf::Vector2u uDefaultScreenResolution;
	sf::String defaultWindowTitle;
	Config config;
};

#endif