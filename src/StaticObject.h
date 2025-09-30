#pragma once

#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include <SFML/Graphics.hpp>
#include "PreloadResources.h"

class StaticObject: public sf::RectangleShape
{
public:
	virtual ~StaticObject(void) {};

protected:
	PreloadResources resource;
	int iWidth;
	int iHeight;
};

#endif
