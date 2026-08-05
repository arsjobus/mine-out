#pragma once

#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <SFML/Graphics.hpp>
#include "../GameState.h"
#include "../Window.h"

class GameOver: public GameState
{
public:
    GameOver(Window &window);
    ~GameOver(void);

    void loadBackground(Window &window);
    void loadGameOverText(Window &window);

public:
    void processEvents(Window &window);
    void update(Window &window, sf::Time dt);
    void render(Window &window, sf::Time dt);

private:
    void loadDefaultFonts();
    void loadDefaultSettings();

private:
    std::unique_ptr<sf::Text> txtGameOver;
    std::unique_ptr<sf::Text> txtReturn;
    float timeSinceStart = 0.0f;
};

#endif
