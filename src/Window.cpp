#include "Window.h"

Window::Window(void) {
	bFullScreen = false;
	// The minimum possible screen resolution
	uMinimumScreenResolution.x = 600;
	uMinimumScreenResolution.y = 400;
	// The default/fallback screen resolution
	uDefaultScreenResolution.x = 800;
	uDefaultScreenResolution.y = 600;
	// The default window title
	defaultWindowTitle = "Breakout";
	// Load settings from settings.ini file into qualifying object members
	loadFromConfig();
}
Window::~Window(void) { }

bool Window::getFullScreen() { return bFullScreen; }
sf::Vector2u &Window::getScreenResolution() { return uScreenResolution; }
sf::String &Window::getDefaultWindowTitle() { return defaultWindowTitle; }

/**
* Load settings from settings.ini file into qualifying object members
*/
void Window::loadFromConfig() {
	// Retrieve values for each of this objects qualifying members from the ini file
	bFullScreen = config.getSettingAsBool("bFullScreen", "settings.ini");
	uScreenResolution.x = config.getSettingAsUInt("uWindowWidth", "settings.ini");
	uScreenResolution.y = config.getSettingAsUInt("uWindowHeight", "settings.ini");
	// Ensure that the window is not set too small in the ini file
	if (uScreenResolution.x < uMinimumScreenResolution.x || uScreenResolution.y < uMinimumScreenResolution.y) {
		uScreenResolution.x = uDefaultScreenResolution.x;
		uScreenResolution.y = uDefaultScreenResolution.y;
	}
	// Log and output information to console
	if (uScreenResolution.x == 800 && uScreenResolution.y == 600)
		log.quickWrite(LOG_INFO, std::string("Setting window resolution to 800 x 600"));
	else if (uScreenResolution.x == 1024 && uScreenResolution.y == 768)
		log.quickWrite(LOG_INFO, std::string("Setting window resolution to 1024 x 768"));
	else
		log.quickWrite(LOG_INFO, std::string("Setting window to custom resolution"));
}