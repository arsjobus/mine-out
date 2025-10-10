#include "PanelT.h"

PanelT::PanelT(void) { this->loadDefaultSettings(); }
PanelT::PanelT(int newWidth, int newHeight) {
	this->loadDefaultSettings();
	// Override default settings here:
	iWidth = newWidth;
	iHeight = newHeight;
	this->setSize(sf::Vector2f(iWidth, iHeight));
	this->setOrigin(sf::Vector2f(getGlobalBounds().size.x / 2, getGlobalBounds().size.y / 2));
}
PanelT::~PanelT(void) { }

void PanelT::loadDefaultSettings() {
	this->setLabel("panel-t");
	this->setSize(sf::Vector2f(1024, 768));
    this->setFillColor(sf::Color::White);
	this->setTexture(&resource.getLevelTexture( 0 ));
	this->setOrigin(sf::Vector2f(getGlobalBounds().size.x / 2, getGlobalBounds().size.y / 2));
}
