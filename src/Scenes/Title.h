#pragma once

#ifndef TITLE_H
#define TITLE_H

#include <SFML/Graphics.hpp>
#include "../TitleScene.h"
#include "../GameState.h"
#include "../Window.h"

class Title: public GameState
{
public:
	Title(Window &window);
	~Title(void);

public:
	void processEvents(Window &window);
    void update(Window &window, sf::Time dt);
    void render(Window &window, sf::Time dt);

private:
	TitleScene titleScene;
};

#endif
