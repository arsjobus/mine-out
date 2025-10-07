#pragma once

#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include "../StaticObject.h"

class RightPanel: public StaticObject
{
public:
	RightPanel(void);
	RightPanel(int newWidth, int newHeight);
	~RightPanel(void);

private:
	void loadDefaultSettings();
};

#endif
