#include "Title.h"

Title::Title(Window &window) {
	setCurrentModeName("Title");
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Initializing mode."));
	loadDefaultSettings();
	loadBackground(window);
	loadDefaultFonts();
	loadTitle(window);
}
Title::~Title(void) { log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Exiting mode.")); }

void Title::processEvents(Window &window) {
    while (std::optional<sf::Event> eventOpt = window.pollEvent()) {
        sf::Event event = *eventOpt; // unwrap optional
        if (event.is<sf::Event::Closed>()) {
            setNextState(STATE_EXIT);
        } else if (event.is<sf::Event::KeyPressed>()) {
            const auto* keyEvent = event.getIf<sf::Event::KeyPressed>();
            if (keyEvent->code == sf::Keyboard::Key::Space) {
                setNextState(STATE_LEVEL1);
            } 
			else if (keyEvent->code == sf::Keyboard::Key::Escape) setNextState(GameState::State::STATE_EXIT);
        }
    }
}

void Title::update(Window &window, sf::Time dt) { }

void Title::render(Window &window, sf::Time dt) {
	window.clear();
	window.draw(getRefToBackground());
	window.draw(*txtMainTitle);
	window.draw(*txtPlayInstruction);
	window.display();
}

void Title::loadBackground(Window &window) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading background.."));
	sf::Vector2f screenResolution(window.getScreenResolution().x, window.getScreenResolution().y);
	getRefToBackground().setSize(sf::Vector2f(screenResolution.x, screenResolution.y));
    getRefToBackground().setFillColor(sf::Color::Black);
    getRefToBackground().setOrigin(sf::Vector2f(getRefToBackground().getGlobalBounds().size.x / 2, getRefToBackground().getGlobalBounds().size.y / 2));
	getRefToBackground().setPosition(sf::Vector2f(screenResolution.x / 2, screenResolution.y / 2));
}

void Title::loadDefaultFonts() {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading fonts.."));
}

void Title::loadDefaultSettings() {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading default settings.."));
	setDefaultTextColor(sf::Color::Red);
}

void Title::loadTitle(Window &window) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading title.."));
	txtMainTitle = std::make_unique<sf::Text>(resources.getFont(0), window.getDefaultWindowTitle(), 48);
    txtMainTitle->setFillColor(getDefaultTextColor());
    txtMainTitle->setStyle(sf::Text::Bold);
    txtMainTitle->setOrigin(sf::Vector2f(txtMainTitle->getGlobalBounds().size.x / 2, txtMainTitle->getGlobalBounds().size.y / 2));
    txtMainTitle->setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 3));
	txtPlayInstruction = std::make_unique<sf::Text>(resources.getFont(1), "PRESS <SPACE> TO PLAY", 24);
    txtPlayInstruction->setFillColor(getDefaultTextColor());
    txtPlayInstruction->setStyle(sf::Text::Bold);
    txtPlayInstruction->setOrigin(sf::Vector2f(txtPlayInstruction->getGlobalBounds().size.x / 2, txtPlayInstruction->getGlobalBounds().size.y / 2));
    txtPlayInstruction->setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y - 100));
}


