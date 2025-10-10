#pragma once

#ifndef CREDITS_H
#define CREDITS_H

#include <SFML/Graphics.hpp>
#include "../GameState.h"
#include "../Window.h"

class Credits: public GameState
{
public:
	Credits(Window &window);
	~Credits(void);

	void loadBackground(Window &window);
	void loadCredits(Window &window);

public:
	void processEvents(Window &window);
    void update(Window &window, sf::Time dt);
    void render(Window &window, sf::Time dt);

private:
	void loadDefaultFonts();
	void loadDefaultSettings();

private:
	std::unique_ptr<sf::Text> txtThankYou;
	std::unique_ptr<sf::Text> txtAuthorName;
	float timeSinceStart = 0.0f;
};

#endif
