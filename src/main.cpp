#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Config.h"
#include "GameState.h"
#include "PreloadResources.h"
#include "Scenes/LoadScreen.h"
#include "Scenes/Title.h"
#include "Scenes/LevelX.h"
#include "main.h"
#include "Log.h"
#include "Window.h"

using namespace std;

// Game state object
std::unique_ptr<GameState> currentState;

// State variables
int stateID = GameState::State::STATE_NULL;
int nextState = GameState::State::STATE_NULL;

/**
 * Set the next state of the game.
 */
void setNextState(int newState) {
    if (nextState != GameState::State::STATE_EXIT) nextState = newState;
}

/**
 * Change the current game state.
 */
void changeState(Window& window) {
    if (nextState != GameState::State::STATE_NULL) {
        // Change the state
        switch (nextState) {
            case GameState::State::STATE_LOAD:
                currentState = std::make_unique<LoadScreen>(window);
                break;
            case GameState::State::STATE_TITLE:
                currentState = std::make_unique<Title>(window);
                break;
            case GameState::State::STATE_LEVEL1:
                currentState = std::make_unique<LevelX>(window, "level1.dat", GameState::State::STATE_LEVEL2);
                break;
            case GameState::State::STATE_LEVEL2:
                currentState = std::make_unique<LevelX>(window, "level2.dat", GameState::State::STATE_LEVEL3);
                break;
            case GameState::State::STATE_LEVEL3:
                currentState = std::make_unique<LevelX>(window, "level3.dat", GameState::State::STATE_TITLE);
                break;
            case GameState::State::STATE_EXIT:
                PreloadResources::unloadResources();
                break;
        }

        // Change the current state ID
        stateID = nextState;
        // NULL the next state ID
        nextState = GameState::State::STATE_NULL;
    }
}

int main() {
    sf::Clock randomTime;
    sf::Clock deltaClock;
    std::srand(randomTime.restart().asMicroseconds());

    Log log;
    Window window;

    window.create(sf::VideoMode(
        sf::Vector2u(
            window.getScreenResolution().x,
            window.getScreenResolution().y
        )
    ), window.getDefaultWindowTitle());

    // Initial state
    stateID = GameState::State::STATE_LOAD;
    currentState = std::make_unique<LoadScreen>(window);

    log.quickWrite(LOG_INFO, "Entering game loop...");

    while (stateID != GameState::State::STATE_EXIT) {
        sf::Time dt = deltaClock.restart();
        // Game logic
        currentState->processEvents(window);
        currentState->update(window, dt);
        changeState(window);
        currentState->render(window, dt);
    }

    log.quickWrite(LOG_INFO, "Exiting game loop...");

    return 0;
}
