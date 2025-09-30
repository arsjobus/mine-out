#pragma once

#ifndef TOPPANEL_H
#define TOPPANEL_H

#include "StaticObject.h"

class TopPanel: public StaticObject
{
public:
	TopPanel(void);
	TopPanel(int newWidth, int newHeight);
	~TopPanel(void);

private:
	void loadDefaultSettings();
};

#endif
