#include "LeftPanel.h"

LeftPanel::LeftPanel(void)
{
	this->loadDefaultSettings();
}

LeftPanel::LeftPanel(int newWidth, int newHeight)
{
	this->loadDefaultSettings();

	// Override default settings here:
	iWidth = newWidth;
	iHeight = newHeight;

	this->setSize(sf::Vector2f(iWidth, iHeight));

	this->setOrigin(sf::Vector2f(getGlobalBounds().size.x / 2, getGlobalBounds().size.y / 2));
}

LeftPanel::~LeftPanel(void)
{
}

void LeftPanel::loadDefaultSettings()
{
	this->setSize(sf::Vector2f(100, 668));

    this->setFillColor(sf::Color::White);

	this->setTexture(&resource.getLevelTexture( 1 ));

	this->setOrigin(sf::Vector2f(getGlobalBounds().size.x / 2, getGlobalBounds().size.y / 2));
}