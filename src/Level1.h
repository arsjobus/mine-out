#pragma once

#ifndef LEVEL1_H
#define LEVEL1_H

#include <SFML/Graphics.hpp>
#include "Level0.h"
#include "Window.h"

class Level1: public Level0
{
public:
	Level1(Window &window);
	~Level1(void);

	void loadBackground(Window &window);
	void loadDefaultSettings();
	void loadObjects(Window &window);

public:
    void render(Window &window);
	void update(Window &window);
};

#endif
