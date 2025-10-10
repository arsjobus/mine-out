#include "PanelR.h"

PanelR::PanelR(void) { this->loadDefaultSettings(); }
PanelR::PanelR(int newWidth, int newHeight) {
	this->loadDefaultSettings();
	iWidth = newWidth;
	iHeight = newHeight;
	// Override default settings here:
	this->setSize(sf::Vector2f(iWidth, iHeight));
	this->setOrigin(sf::Vector2f(getGlobalBounds().size.x / 2, getGlobalBounds().size.y / 2));
}
PanelR::~PanelR(void) { }

void PanelR::loadDefaultSettings() {
	this->setLabel("panel-r");
	this->setSize(sf::Vector2f(100, 668));
    this->setFillColor(sf::Color::White);
	this->setTexture(&resource.getLevelTexture( 2 ));
	this->setOrigin(sf::Vector2f(getGlobalBounds().size.x / 2, getGlobalBounds().size.y / 2));
}