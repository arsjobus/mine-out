#pragma once

#ifndef GROWPADDLE_H
#define GROWPADDLE_H

#include "PowerUp.h"
#include "../Window.h"

class GrowPaddle: public PowerUp
{
public:
	GrowPaddle(void);
	~GrowPaddle(void);

	void update(bool isPaused);
	void render(Window &window);

private:
	void loadDefaultSettings();
};

#endif