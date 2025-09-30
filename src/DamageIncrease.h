#pragma once

#ifndef DAMAGEINCREASE_H
#define DAMAGEINCREASE_H

#include "powerup.h"
#include "Window.h"

class DamageIncrease: public PowerUp
{
public:
	DamageIncrease(void);
	~DamageIncrease(void);

	void update();
	void loadDefaultSettings();
	void render(Window &window);
};

#endif