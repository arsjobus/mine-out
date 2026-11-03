#include "Title.h"

Title::Title(Window &window)
	: titleScene(
		window.getScreenResolution().x,
		window.getScreenResolution().y,
		window.getDefaultWindowTitle()
	) {
	setCurrentModeName("Title");
	log.quickWrite(LOG_INFO, std::string(getCurrentModeName() + log.getSeparator() + "Initializing mode."));
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
			TitleScene::Key key = TitleScene::Key::Other;
			if (keyEvent->code == sf::Keyboard::Key::Space) {
				key = TitleScene::Key::Space;
			} else if (keyEvent->code == sf::Keyboard::Key::Escape) {
#ifndef __EMSCRIPTEN__
				key = TitleScene::Key::Escape;
#endif
			}
			switch (titleScene.handleKey(key)) {
				case TitleScene::Action::StartGame:
					setNextState(STATE_LEVEL1);
					break;
				case TitleScene::Action::Exit:
					setNextState(STATE_EXIT);
					break;
				case TitleScene::Action::None:
					break;
			}
        }
    }
}

void Title::update(Window &window, sf::Time dt) { }

void Title::render(Window &window, sf::Time dt) {
	titleScene.render(window);
}
