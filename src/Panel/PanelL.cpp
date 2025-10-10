#include "PanelL.h"

PanelL::PanelL(void) { this->loadDefaultSettings(); }
PanelL::PanelL(int newWidth, int newHeight) {
	this->loadDefaultSettings();
	// Override default settings here:
	iWidth = newWidth;
	iHeight = newHeight;
	this->setSize(sf::Vector2f(iWidth, iHeight));
	this->setOrigin(sf::Vector2f(getGlobalBounds().size.x / 2, getGlobalBounds().size.y / 2));
}
PanelL::~PanelL(void) { }

void PanelL::loadDefaultSettings() {
	this->setLabel("panel-l");
	this->setSize(sf::Vector2f(100, 668));
    this->setFillColor(sf::Color::White);
	this->setTexture(&resource.getLevelTexture( 1 ));
	this->setOrigin(sf::Vector2f(getGlobalBounds().size.x / 2, getGlobalBounds().size.y / 2));
}