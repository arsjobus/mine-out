#include "Title.h"

Title::Title(Window &window)
{
	setCurrentModeName("Title");

	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Initializing mode."));

	loadDefaultSettings();
	
	loadBackground(window);

	loadDefaultFonts();

	loadTitle(window);
}

Title::~Title(void)
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Exiting mode."));
}

void Title::processEvents(Window &window)
{
    while (std::optional<sf::Event> eventOpt = window.pollEvent())
    {
        sf::Event event = *eventOpt; // unwrap optional

        if (event.is<sf::Event::Closed>())
        {
            setNextState(STATE_EXIT);
        }
        else if (event.is<sf::Event::KeyPressed>())
        {
            const auto* keyEvent = event.getIf<sf::Event::KeyPressed>();
            if (keyEvent->code == sf::Keyboard::Key::Space)
            {
                setNextState(STATE_LEVEL1);
            }
        }
    }
}

void Title::update(Window &window)
{

}

void Title::render(Window &window)
{
	window.clear();

	window.draw(getRefToBackground());

	window.draw(*txtMainTitle);

	window.display();
}

void Title::loadBackground(Window &window)
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading background.."));

	sf::Vector2f screenResolution(window.getScreenResolution().x, window.getScreenResolution().y);

	getRefToBackground().setSize(sf::Vector2f(screenResolution.x, screenResolution.y));
    getRefToBackground().setFillColor(sf::Color::Black);
    getRefToBackground().setOrigin(sf::Vector2f(getRefToBackground().getGlobalBounds().size.x / 2, getRefToBackground().getGlobalBounds().size.y / 2));
	getRefToBackground().setPosition(sf::Vector2f(screenResolution.x / 2, screenResolution.y / 2));
}

void Title::loadDefaultFonts()
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading fonts.."));

	if (getPrimaryFontName().isEmpty())
		log.quickWrite(LOG_WARNING, std::string(getCurrentModeName() + log.getSeparator() + "A primary font name was not specified."));
	else if (getFontDirectoryName().isEmpty())
		log.quickWrite(LOG_WARNING, std::string(getCurrentModeName() + log.getSeparator() + "The font directory was not specified."));
	else
		setPrimaryFont(std::string(getFontDirectoryName() + "/" + getPrimaryFontName()));
}

void Title::loadDefaultSettings()
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading default settings.."));

	setPrimaryFontName("abandon.ttf");

	setSecondaryFontName("arial.ttf");

	setFontDirectoryName("fonts");

	setDefaultTextColor(sf::Color::Red);
}

void Title::loadTitle(Window &window)
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading title.."));

	txtMainTitle = new sf::Text(getPrimaryFont(), window.getDefaultWindowTitle(), 48);
    txtMainTitle->setFillColor(getDefaultTextColor());
    txtMainTitle->setStyle(sf::Text::Bold);
    txtMainTitle->setOrigin(sf::Vector2f(txtMainTitle->getGlobalBounds().size.x / 2, txtMainTitle->getGlobalBounds().size.y / 2));
    txtMainTitle->setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 3));
}


