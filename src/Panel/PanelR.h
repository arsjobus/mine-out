#pragma once

#ifndef PANELR_H
#define PANELR_H

#include "../StaticObject.h"

class PanelR: public StaticObject
{
public:
	PanelR(void);
	PanelR(int newWidth, int newHeight);
	~PanelR(void);

private:
	void loadDefaultSettings();
};

#endif
