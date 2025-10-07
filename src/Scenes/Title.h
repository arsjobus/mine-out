#pragma once

#ifndef TITLE_H
#define TITLE_H

#include <SFML/Graphics.hpp>
#include "../GameState.h"
#include "../Window.h"

class Title: public GameState
{
public:
	Title(Window &window);
	~Title(void);

	void loadBackground(Window &window);
	void loadTitle(Window &window);

public:
	void processEvents(Window &window);
    void update(Window &window);
    void render(Window &window);

private:
	void loadDefaultFonts();
	void loadDefaultSettings();

private:
	sf::Text *txtMainTitle;
	sf::Text *txtPlayInstruction;
};

#endif
