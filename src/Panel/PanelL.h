#pragma once

#ifndef PANELL_H
#define PANELL_H

#include "../StaticObject.h"

class PanelL: public StaticObject
{
public:
	PanelL(void);
	PanelL(int newWidth, int newHeight);
	~PanelL(void);

private:
	void loadDefaultSettings();
};

#endif
