#include "RightPanel.h"

RightPanel::RightPanel(void)
{
	this->loadDefaultSettings();
}

RightPanel::RightPanel(int newWidth, int newHeight)
{
	this->loadDefaultSettings();

	iWidth = newWidth;
	iHeight = newHeight;

	// Override default settings here:
	this->setSize(sf::Vector2f(iWidth, iHeight));

	this->setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
}

RightPanel::~RightPanel(void)
{
}

void RightPanel::loadDefaultSettings()
{
	this->setSize(sf::Vector2f(100, 668));

    this->setFillColor(sf::Color::White);

	this->setTexture(&resource.getLevelTexture( 2 ));

    this->setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
}