#pragma once

#ifndef DAMAGEINCREASE_H
#define DAMAGEINCREASE_H

#include "PowerUp.h"
#include "../Window.h"

class DamageIncrease: public PowerUp
{
public:
	DamageIncrease(void);
	~DamageIncrease(void);

	void update(sf::Time dt);
	void loadDefaultSettings();
	void render(Window &window);
};

#endif