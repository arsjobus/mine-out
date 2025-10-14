#include "Credits.h"

Credits::Credits(Window &window) {
	setCurrentModeName("Title");
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Initializing mode."));
	loadDefaultSettings();
	loadBackground(window);
	loadDefaultFonts();
	loadCredits(window);
	timeSinceStart = 0.0f; // Reset timer when screen starts
}

Credits::~Credits(void) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Exiting mode."));
}

void Credits::processEvents(Window &window) {
	while (std::optional<sf::Event> eventOpt = window.pollEvent()) {
		sf::Event event = *eventOpt;
		if (event.is<sf::Event::Closed>()) {
			setNextState(STATE_EXIT);
		} else if (event.is<sf::Event::KeyPressed>()) {
			const auto* keyEvent = event.getIf<sf::Event::KeyPressed>();
			if (keyEvent->code == sf::Keyboard::Key::Space) {
				setNextState(STATE_TITLE);
			}
			else if (keyEvent->code == sf::Keyboard::Key::Escape) {
				setNextState(STATE_EXIT);
			}
		}
	}
}

void Credits::update(Window &window, sf::Time dt) {
	timeSinceStart += dt.asSeconds();

	if (timeSinceStart >= 5.0f) {
		setNextState(STATE_TITLE);
	}
}

void Credits::render(Window &window, sf::Time dt) {
	window.clear();
	window.draw(getRefToBackground());
	window.draw(*txtThankYou);
	window.draw(*txtAuthorName);
	window.display();
}

void Credits::loadBackground(Window &window) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading background.."));
	sf::Vector2f screenResolution(window.getScreenResolution().x, window.getScreenResolution().y);
	getRefToBackground().setSize(sf::Vector2f(screenResolution.x, screenResolution.y));
    getRefToBackground().setFillColor(sf::Color::Black);
    getRefToBackground().setOrigin(sf::Vector2f(getRefToBackground().getGlobalBounds().size.x / 2, getRefToBackground().getGlobalBounds().size.y / 2));
	getRefToBackground().setPosition(sf::Vector2f(screenResolution.x / 2, screenResolution.y / 2));
}

void Credits::loadDefaultFonts() {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading fonts.."));
}

void Credits::loadDefaultSettings() {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading default settings.."));
	setDefaultTextColor(sf::Color::Red);
}

void Credits::loadCredits(Window &window) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading Credits Texts.."));
	txtThankYou = std::make_unique<sf::Text>(resources.getFont(0), "Thanks for Playing", 48);
    txtThankYou->setFillColor(getDefaultTextColor());
    txtThankYou->setStyle(sf::Text::Bold);
    txtThankYou->setOrigin(sf::Vector2f(txtThankYou->getGlobalBounds().size.x / 2, txtThankYou->getGlobalBounds().size.y / 2));
    txtThankYou->setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 3));
	txtAuthorName = std::make_unique<sf::Text>(resources.getFont(1), "By Alexander Shepherd", 24);
    txtAuthorName->setFillColor(getDefaultTextColor());
    txtAuthorName->setStyle(sf::Text::Bold);
    txtAuthorName->setOrigin(sf::Vector2f(txtAuthorName->getGlobalBounds().size.x / 2, txtAuthorName->getGlobalBounds().size.y / 2));
    txtAuthorName->setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
}


