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

//Game state object
GameState *currentState = NULL;

//State variables
int stateID = GameState::State::STATE_NULL;
int nextState = GameState::State::STATE_NULL;

int main() {
	sf::Clock randomTime;
	std::srand(randomTime.restart().asMicroseconds());
	Log log;
	Window window;

	window.create(sf::VideoMode(
        sf::Vector2u(
            window.getScreenResolution().x, 
            window.getScreenResolution().y
        )
    ), window.getDefaultWindowTitle());

    // Set the current state ID
    stateID = GameState::State::STATE_LOAD;
	//stateID = GameState::State::STATE_LEVEL2;

    // Set the current game state object
    currentState = new LoadScreen(window);
	//currentState = new Level2(window);

    log.quickWrite(LOG_INFO, "Entering game loop..");

    while ( stateID != GameState::State::STATE_EXIT ) {
		currentState->processEvents(window);
		currentState->update(window);
		changeState(window);
		currentState->render(window);
    }
    log.quickWrite(LOG_INFO, "Exiting game loop..");
    return 0;
}

/**
* Set the next state of the game.
*/
void setNextState( int newState ) {
    if ( nextState != GameState::State::STATE_EXIT ) nextState = newState;
}

/**
* 
*/
void changeState(Window &window) {
    if ( nextState != GameState::State::STATE_NULL) {
        if ( nextState != GameState::State::STATE_EXIT ) delete currentState;

        // Change the state
        switch ( nextState ) {
			case GameState::State::STATE_LOAD:
				currentState = new LoadScreen(window);
				break;
			case GameState::State::STATE_TITLE:
                currentState = new Title(window);
                break;
			case GameState::State::STATE_LEVEL1:
                currentState = new LevelX(window, "level1.dat", GameState::State::STATE_LEVEL2);
                break;
			case GameState::State::STATE_LEVEL2:
				currentState = new LevelX(window, "level2.dat", GameState::State::STATE_LEVEL3);
                break;
            case GameState::State::STATE_LEVEL3:
                currentState = new LevelX(window, "level3.dat", GameState::State::STATE_TITLE);
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
