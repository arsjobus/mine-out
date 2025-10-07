#pragma once

#ifndef LEFTPANEL_H
#define LEFTPANEL_H

#include "../StaticObject.h"

class LeftPanel: public StaticObject
{
public:
	LeftPanel(void);
	LeftPanel(int newWidth, int newHeight);
	~LeftPanel(void);

private:
	void loadDefaultSettings();
};

#endif
