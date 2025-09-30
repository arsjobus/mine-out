#pragma once

#ifndef LEVEL2_H
#define LEVEL2_H

#include <SFML/Graphics.hpp>
#include "Level0.h"
#include "Window.h"

class Level2: public Level0
{
public:
	Level2(Window &window);
	~Level2(void);

	void loadBackground(Window &window);
	void loadDefaultSettings();
	void loadObjects(Window &window);

public:
    void render(Window &window);
	void update(Window &window);
};

#endif