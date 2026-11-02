#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include "Config.h"
#include "GameState.h"
#include "PreloadResources.h"
#include "Scenes/Credits.h"
#include "Scenes/GameOver.h"
#include "Scenes/LoadScreen.h"
#include "Scenes/Title.h"
#include "Scenes/LevelX.h"
#include "main.h"
#include "Log.h"
#include "Window.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;

// Game state object
std::unique_ptr<GameState> currentState;

// State variables
int stateID = GameState::State::STATE_NULL;
int nextState = GameState::State::STATE_NULL;

#ifdef __EMSCRIPTEN__
namespace {
std::unique_ptr<Window> browserWindow;
sf::Clock browserDeltaClock;

void browserFrame() {
    if (stateID == GameState::State::STATE_EXIT) {
        currentState.reset();
        PreloadResources::unloadResources();
        browserWindow->close();
        emscripten_cancel_main_loop();
        return;
    }

    sf::Time dt = browserDeltaClock.restart();
    if (!browserWindow->hasFocus()) dt = sf::Time::Zero;
    else dt = std::min(dt, sf::seconds(0.1f));

    currentState->processEvents(*browserWindow);
    currentState->update(*browserWindow, dt);
    changeState(*browserWindow);
    currentState->render(*browserWindow, dt);
}
}
#endif

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
                GameSession::instance().reset();
                currentState = std::make_unique<Title>(window);
                break;
            case GameState::State::STATE_LEVEL1:
                GameSession::instance().reset();
                currentState = std::make_unique<LevelX>(window, "level01.dat", GameState::State::STATE_LEVEL2);
                break;
            case GameState::State::STATE_LEVEL2:
                currentState = std::make_unique<LevelX>(window, "level02.dat", GameState::State::STATE_LEVEL3);
                break;
            case GameState::State::STATE_LEVEL3:
                currentState = std::make_unique<LevelX>(window, "level03.dat", GameState::State::STATE_LEVEL4);
                break;
            case GameState::State::STATE_LEVEL4:
                currentState = std::make_unique<LevelX>(window, "level04.dat", GameState::State::STATE_LEVEL5);
                break;
            case GameState::State::STATE_LEVEL5:
                currentState = std::make_unique<LevelX>(window, "level05.dat", GameState::State::STATE_LEVEL6);
                break;
            case GameState::State::STATE_LEVEL6:
                currentState = std::make_unique<LevelX>(window, "level06.dat", GameState::State::STATE_LEVEL7);
                break;
            case GameState::State::STATE_LEVEL7:
                currentState = std::make_unique<LevelX>(window, "level07.dat", GameState::State::STATE_LEVEL8);
                break;
            case GameState::State::STATE_LEVEL8:
                currentState = std::make_unique<LevelX>(window, "level08.dat", GameState::State::STATE_LEVEL9);
                break;
            case GameState::State::STATE_LEVEL9:
                currentState = std::make_unique<LevelX>(window, "level09.dat", GameState::State::STATE_LEVEL10);
                break;
            case GameState::State::STATE_LEVEL10:
                currentState = std::make_unique<LevelX>(window, "level10.dat", GameState::State::STATE_CREDITS);
                break;
            case GameState::State::STATE_GAMEOVER:
                currentState = std::make_unique<GameOver>(window);
                break;
            case GameState::State::STATE_CREDITS:
                currentState = std::make_unique<Credits>(window);
                break;
            case GameState::State::STATE_EXIT:
                //PreloadResources::unloadResources();
                break;
        }

        // Change the current state ID
        stateID = nextState;
#ifdef __EMSCRIPTEN__
        EM_ASM({ window.mineOutGameState = $0; }, stateID);
#endif
        // NULL the next state ID
        nextState = GameState::State::STATE_NULL;
    }
}

int main() {
#ifdef __EMSCRIPTEN__
    sf::Clock randomTime;
    std::srand(randomTime.restart().asMicroseconds());
    Log log;
    browserWindow = std::make_unique<Window>();
    browserWindow->create(sf::VideoMode(sf::Vector2u(1024, 768)), browserWindow->getDefaultWindowTitle());
    browserWindow->updateLetterboxView(1024, 768);
    browserWindow->clear(sf::Color::Black);
    browserWindow->display();
    stateID = GameState::State::STATE_LOAD;
    currentState = std::make_unique<LoadScreen>(*browserWindow);
    log.quickWrite(LOG_INFO, "Entering browser game loop...");
    browserDeltaClock.restart();
    emscripten_set_main_loop(browserFrame, 0, 1);
    return 0;
#else
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
    window.updateLetterboxView(window.getSize().x, window.getSize().y);

    // Ensure the initial window contents are black to avoid a white flash on macOS
    window.clear(sf::Color::Black);
    window.display();

    // Initial state
    stateID = GameState::State::STATE_LOAD;
    currentState = std::make_unique<LoadScreen>(window);

    log.quickWrite(LOG_INFO, "Entering game loop...");
    const char* home = std::getenv("HOME");
    const std::filesystem::path screenshotsDir = home
        ? std::filesystem::path(home) / "Pictures" / "MineOut-Screenshots"
        : std::filesystem::current_path() / "screenshots";
    std::cout << "Screenshots will be saved to: " << screenshotsDir << std::endl;

    while (stateID != GameState::State::STATE_EXIT) {
        sf::Time dt = deltaClock.restart();
		if (!window.hasFocus()) {
			dt = sf::Time::Zero;
		} else {
			dt = std::min(dt, sf::seconds(0.1f));
		}
        // Game logic
        currentState->processEvents(window);
        currentState->update(window, dt);
        changeState(window);
        currentState->render(window, dt);

    }

    log.quickWrite(LOG_INFO, "Exiting game loop...");
    currentState.reset();
    PreloadResources::unloadResources();

    return 0;
#endif
}
