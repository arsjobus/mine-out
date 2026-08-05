#pragma once

#ifndef EXTRAPADDLE_H
#define EXTRAPADDLE_H

#include "PowerUp.h"
#include "../Window.h"

class ExtraPaddle: public PowerUp
{
public:
    ExtraPaddle(void);
    ~ExtraPaddle(void);

    void update(sf::Time dt);
    void render(Window &window);

private:
    void loadDefaultSettings();
};

#endif
