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
        if (window.handleScreenshotHotkey(event)) {
            continue;
        }
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
    if (txtAuthorBy) window.draw(*txtAuthorBy);
    if (txtAuthorName) window.draw(*txtAuthorName);
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
    setDefaultTextColor(sf::Color::White);
}

void Title::loadTitle(Window &window) {
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading title.."));
    // Responsive title size based on window height, capped for large screens
    unsigned int titleSize = static_cast<unsigned int>(std::min<float>(static_cast<float>(window.getScreenResolution().y) / 5.0f, 120.f));
    txtMainTitle = std::make_unique<sf::Text>(resources.getFont(0), window.getDefaultWindowTitle(), titleSize);
    txtMainTitle->setFillColor(getDefaultTextColor());
    txtMainTitle->setStyle(sf::Text::Bold);
    txtMainTitle->setOutlineColor(sf::Color::Black);
    txtMainTitle->setOutlineThickness(std::max(1.f, titleSize * 0.03f));
    // Center origin using global bounds size (project convention)
    sf::FloatRect mainBounds = txtMainTitle->getGlobalBounds();
    txtMainTitle->setOrigin(sf::Vector2f(mainBounds.size.x / 2.f, mainBounds.size.y / 2.f));
    // Position slightly higher than one third for better visual balance
    float mainY = static_cast<float>(window.getScreenResolution().y) * 0.28f;
    txtMainTitle->setPosition(sf::Vector2f(static_cast<float>(window.getScreenResolution().x) / 2.f, mainY));

    // Author text below the title using two lines: 'by' and the name
    unsigned int authorSize = titleSize / 6;
    if (authorSize < 12) authorSize = 12;
    unsigned int bySize = std::max<unsigned int>(10, static_cast<unsigned int>(authorSize * 0.75f));
    txtAuthorBy = std::make_unique<sf::Text>(resources.getFont(1), "", bySize);
    txtAuthorName = std::make_unique<sf::Text>(resources.getFont(1), "", authorSize);
    txtAuthorBy->setFillColor(sf::Color(200, 200, 200));
    txtAuthorName->setFillColor(sf::Color(200, 200, 200));

    // compute sizes to avoid overlaps
    sf::FloatRect mainGlobal = txtMainTitle->getGlobalBounds();
    sf::FloatRect byGlobal = txtAuthorBy->getGlobalBounds();
    sf::FloatRect nameGlobal = txtAuthorName->getGlobalBounds();
    float padding = 15.f; // fixed padding between title and author block
    float gapBetweenAuthorLines = 4.f;
    float combinedHeight = byGlobal.size.y + gapBetweenAuthorLines + nameGlobal.size.y;
    float authorBlockCenterY = mainY + (mainGlobal.size.y / 2.f) + (combinedHeight / 2.f) + padding;

    // position 'by' and name centered within the author block
    txtAuthorBy->setOrigin(sf::Vector2f(byGlobal.size.x / 2.f, byGlobal.size.y / 2.f));
    txtAuthorName->setOrigin(sf::Vector2f(nameGlobal.size.x / 2.f, nameGlobal.size.y / 2.f));
    float topOfBlock = authorBlockCenterY - (combinedHeight / 2.f);
    float byY = topOfBlock + (byGlobal.size.y / 2.f);
    float nameY = topOfBlock + byGlobal.size.y + gapBetweenAuthorLines + (nameGlobal.size.y / 2.f);
    txtAuthorBy->setPosition(sf::Vector2f(static_cast<float>(window.getScreenResolution().x) / 2.f, byY));
    txtAuthorName->setPosition(sf::Vector2f(static_cast<float>(window.getScreenResolution().x) / 2.f, nameY));

    // Play instruction smaller and positioned towards the bottom
    unsigned int instrSize = static_cast<unsigned int>(std::max<int>(18, titleSize / 5));
    txtPlayInstruction = std::make_unique<sf::Text>(resources.getFont(1), "PRESS <SPACE> TO PLAY", instrSize);
    txtPlayInstruction->setFillColor(getDefaultTextColor());
    txtPlayInstruction->setStyle(sf::Text::Bold);
    sf::FloatRect instrGlobal = txtPlayInstruction->getGlobalBounds();
    txtPlayInstruction->setOrigin(sf::Vector2f(instrGlobal.size.x / 2.f, instrGlobal.size.y / 2.f));
    txtPlayInstruction->setPosition(sf::Vector2f(static_cast<float>(window.getScreenResolution().x) / 2.f, static_cast<float>(window.getScreenResolution().y) - 100.f));
}


