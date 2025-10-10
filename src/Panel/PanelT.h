#pragma once

#ifndef PANELT_H
#define PANELT_H

#include "../StaticObject.h"

class PanelT: public StaticObject
{
public:
	PanelT(void);
	PanelT(int newWidth, int newHeight);
	~PanelT(void);

private:
	void loadDefaultSettings();
};

#endif
