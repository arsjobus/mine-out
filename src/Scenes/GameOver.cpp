#include "GameOver.h"

GameOver::GameOver(Window &window) {
    setCurrentModeName("GameOver");
    log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Initializing mode."));
    loadDefaultSettings();
    loadBackground(window);
    loadDefaultFonts();
    loadGameOverText(window);
    timeSinceStart = 0.0f;
}

GameOver::~GameOver(void) {
    log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Exiting mode."));
}

void GameOver::processEvents(Window &window) {
    while (std::optional<sf::Event> eventOpt = window.pollEvent()) {
        sf::Event event = *eventOpt;
        if (window.handleScreenshotHotkey(event)) {
            continue;
        }
        if (event.is<sf::Event::Closed>()) {
            setNextState(STATE_EXIT);
        } else if (event.is<sf::Event::KeyPressed>()) {
            const auto* keyEvent = event.getIf<sf::Event::KeyPressed>();
            if (keyEvent->code == sf::Keyboard::Key::Space) {
                setNextState(STATE_TITLE);
            } else if (keyEvent->code == sf::Keyboard::Key::Escape) {
                setNextState(STATE_EXIT);
            }
        }
    }
}

void GameOver::update(Window &window, sf::Time dt) {
    timeSinceStart += dt.asSeconds();
    if (timeSinceStart >= 5.0f) {
        setNextState(STATE_TITLE);
    }
}

void GameOver::render(Window &window, sf::Time dt) {
    window.clear();
    window.draw(getRefToBackground());
    window.draw(*txtGameOver);
    window.draw(*txtReturn);
    window.display();
}

void GameOver::loadBackground(Window &window) {
    log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading background.."));
    sf::Vector2f screenResolution(window.getScreenResolution().x, window.getScreenResolution().y);
    getRefToBackground().setSize(sf::Vector2f(screenResolution.x, screenResolution.y));
    getRefToBackground().setFillColor(sf::Color::Black);
    getRefToBackground().setOrigin(sf::Vector2f(getRefToBackground().getGlobalBounds().size.x / 2, getRefToBackground().getGlobalBounds().size.y / 2));
    getRefToBackground().setPosition(sf::Vector2f(screenResolution.x / 2, screenResolution.y / 2));
}

void GameOver::loadDefaultFonts() {
    log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading fonts.."));
}

void GameOver::loadDefaultSettings() {
    log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading default settings.."));
    setDefaultTextColor(sf::Color::Red);
}

void GameOver::loadGameOverText(Window &window) {
    log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading game over text.."));
    txtGameOver = std::make_unique<sf::Text>(resources.getFont(0), "GAME OVER", 64);
    txtGameOver->setFillColor(getDefaultTextColor());
    txtGameOver->setStyle(sf::Text::Bold);
    txtGameOver->setOrigin(sf::Vector2f(txtGameOver->getGlobalBounds().size.x / 2, txtGameOver->getGlobalBounds().size.y / 2));
    txtGameOver->setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 3));

    txtReturn = std::make_unique<sf::Text>(resources.getFont(1), "PRESS <SPACE> TO RETURN", 24);
    txtReturn->setFillColor(getDefaultTextColor());
    txtReturn->setStyle(sf::Text::Bold);
    txtReturn->setOrigin(sf::Vector2f(txtReturn->getGlobalBounds().size.x / 2, txtReturn->getGlobalBounds().size.y / 2));
    txtReturn->setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
}
