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
	sf::Event userInput;

	while (window.pollEvent(userInput))
	{
		switch (userInput.type)
		{
			case sf::Event::Closed:
				setNextState( STATE_EXIT );
				break;
			case sf::Event::KeyPressed:
				if (userInput.key.code == sf::Keyboard::Space)
					setNextState( STATE_LEVEL1 );
				break;
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

	window.draw(txtMainTitle);

	window.display();
}

void Title::loadBackground(Window &window)
{
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Loading background.."));

	sf::Vector2f screenResolution(window.getScreenResolution().x, window.getScreenResolution().y);

	getRefToBackground().setSize(sf::Vector2f(screenResolution.x, screenResolution.y));
    getRefToBackground().setFillColor(sf::Color::Black);
    getRefToBackground().setOrigin(getRefToBackground().getGlobalBounds().width / 2, getRefToBackground().getGlobalBounds().height / 2);
	getRefToBackground().setPosition(screenResolution.x / 2, screenResolution.y / 2);
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

	txtMainTitle.setFont(getPrimaryFont());
    txtMainTitle.setString(window.getDefaultWindowTitle());
    txtMainTitle.setCharacterSize(48);
    txtMainTitle.setColor(getDefaultTextColor());
    txtMainTitle.setStyle(sf::Text::Bold);
    txtMainTitle.setOrigin(txtMainTitle.getGlobalBounds().width / 2, txtMainTitle.getGlobalBounds().height / 2);
    txtMainTitle.setPosition(window.getSize().x / 2, window.getSize().y / 3);
}


