#include <cstdlib>

#include "Window.h"

Window::Window(void) {
	bFullScreen = false;
	screenshotKeyHeld = false;
	// The minimum possible screen resolution
	uMinimumScreenResolution.x = 600;
	uMinimumScreenResolution.y = 400;
	// The default/fallback screen resolution
	uDefaultScreenResolution.x = 800;
	uDefaultScreenResolution.y = 600;
	// The default window title
	defaultWindowTitle = "Mine Out";
	// Load settings from settings.ini file into qualifying object members
	loadFromConfig();
}
Window::~Window(void) { }

std::optional<sf::Event> Window::pollEvent() {
	auto eventOpt = sf::RenderWindow::pollEvent();
	if (eventOpt) {
		if (const auto* resized = eventOpt->getIf<sf::Event::Resized>()) {
			updateLetterboxView(resized->size.x, resized->size.y);
		}
	}
	return eventOpt;
}

void Window::updateLetterboxView(unsigned int windowWidth, unsigned int windowHeight) {
	const float targetRatio = static_cast<float>(uScreenResolution.x) / static_cast<float>(uScreenResolution.y);
	const float windowRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

	sf::View view(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(static_cast<float>(uScreenResolution.x), static_cast<float>(uScreenResolution.y))));

	if (windowRatio > targetRatio) {
		float width = targetRatio / windowRatio;
		view.setViewport(sf::FloatRect(sf::Vector2f((1.f - width) / 2.f, 0.f), sf::Vector2f(width, 1.f)));
	} else {
		float height = windowRatio / targetRatio;
		view.setViewport(sf::FloatRect(sf::Vector2f(0.f, (1.f - height) / 2.f), sf::Vector2f(1.f, height)));
	}

	setView(view);
}

bool Window::getFullScreen() { return bFullScreen; }
sf::Vector2u &Window::getScreenResolution() { return uScreenResolution; }
sf::String &Window::getDefaultWindowTitle() { return defaultWindowTitle; }

bool Window::handleScreenshotHotkey(const sf::Event& event) {
	if (event.is<sf::Event::KeyPressed>()) {
		const auto* keyEvent = event.getIf<sf::Event::KeyPressed>();
		if (!keyEvent) {
			return false;
		}

		const auto key = keyEvent->code;
		const auto scancode = keyEvent->scancode;
		const bool isHotkey =
			key == sf::Keyboard::Key::Add ||
			key == sf::Keyboard::Key::Equal ||
			key == sf::Keyboard::Key::F2 ||
			scancode == sf::Keyboard::Scancode::Equal ||
			scancode == sf::Keyboard::Scancode::NumpadEqual ||
			scancode == sf::Keyboard::Scancode::F2;

		if (isHotkey && !screenshotKeyHeld) {
			screenshotKeyHeld = true;
			std::cout << "Screenshot hotkey detected" << std::endl;
			saveScreenshot();
			return true;
		}
	}

	if (event.is<sf::Event::KeyReleased>()) {
		const auto* keyEvent = event.getIf<sf::Event::KeyReleased>();
		if (!keyEvent) {
			return false;
		}

		const auto key = keyEvent->code;
		const auto scancode = keyEvent->scancode;
		const bool isHotkey =
			key == sf::Keyboard::Key::Add ||
			key == sf::Keyboard::Key::Equal ||
			key == sf::Keyboard::Key::F2 ||
			scancode == sf::Keyboard::Scancode::Equal ||
			scancode == sf::Keyboard::Scancode::NumpadEqual ||
			scancode == sf::Keyboard::Scancode::F2;

		if (isHotkey) {
			screenshotKeyHeld = false;
		}
	}

	if (event.is<sf::Event::TextEntered>()) {
		const auto* textEvent = event.getIf<sf::Event::TextEntered>();
		if (textEvent && (textEvent->unicode == L'+' || textEvent->unicode == L'=')) {
			if (!screenshotKeyHeld) {
				screenshotKeyHeld = true;
				std::cout << "Screenshot hotkey detected" << std::endl;
				saveScreenshot();
				return true;
			}
		}
	}

	return false;
}

void Window::saveScreenshot(const std::string& prefix) {
	const auto now = std::chrono::system_clock::now();
	const auto time = std::chrono::system_clock::to_time_t(now);
	std::tm localTime{};
#if defined(_WIN32)
	localtime_s(&localTime, &time);
#else
	localtime_r(&time, &localTime);
#endif

	const char* home = std::getenv("HOME");
	std::filesystem::path outputDir = home
		? std::filesystem::path(home) / "Pictures" / "MineOut-Screenshots"
		: std::filesystem::current_path() / "screenshots";
	std::filesystem::create_directories(outputDir);

	std::ostringstream filename;
	filename << prefix << "-" << std::put_time(&localTime, "%Y%m%d-%H%M%S") << ".png";

	std::filesystem::path outputPath = outputDir / filename.str();

	sf::Texture texture;
	if (!texture.resize(sf::Vector2u(getSize().x, getSize().y))) {
		std::cerr << "Failed to allocate screenshot texture" << std::endl;
		return;
	}
	texture.update(*this);

	sf::Image image = texture.copyToImage();
	if (!image.saveToFile(outputPath)) {
		std::cerr << "Failed to save screenshot: " << outputPath << std::endl;
		return;
	}

	std::cout << "Saved screenshot to " << outputPath << std::endl;
}

/**
* Load settings from settings.ini file into qualifying object members
*/
void Window::loadFromConfig() {
	// Retrieve values for each of this objects qualifying members from the ini file
	//bFullScreen = config.getSettingAsBool("bFullScreen", "settings.ini");
	//uScreenResolution.x = config.getSettingAsUInt("uWindowWidth", "settings.ini");
	//uScreenResolution.y = config.getSettingAsUInt("uWindowHeight", "settings.ini");
	bFullScreen = false;
	uScreenResolution.x = 1024;
	uScreenResolution.y = 768;
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