#pragma once

#ifndef LEVELX_H
#define LEVELX_H

#include <SFML/Graphics.hpp>
#include "Level0.h"
#include "../Window.h"

class LevelX: public Level0
{
public:
	LevelX(Window &window, const char *filename, GameState::State nextState);
	~LevelX(void);

	void loadBackground(Window &window);
	void loadDefaultSettings();
	void loadObjects(Window &window);

public:
    void render(Window &window, sf::Time dt);
	void update(Window &window, sf::Time dt);

private:
	std::unique_ptr<sf::Text> fpsText;
};

#endif
