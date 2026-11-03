#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <SDL_mixer.h>

#include <cstdio>
#include <stdexcept>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

namespace {

bool audioUnlockedByInput = false;

sf::Keyboard::Key mapKey(SDL_Keycode key) {
	switch (key) {
		case SDLK_SPACE: return sf::Keyboard::Key::Space;
		case SDLK_ESCAPE: return sf::Keyboard::Key::Escape;
		case SDLK_EQUALS:
		case SDLK_PLUS: return sf::Keyboard::Key::Equal;
		case SDLK_F2: return sf::Keyboard::Key::F2;
		default: return sf::Keyboard::Key::Unknown;
	}
}

sf::Keyboard::Scancode mapScancode(SDL_Scancode key) {
	switch (key) {
		case SDL_SCANCODE_SPACE: return sf::Keyboard::Scancode::Space;
		case SDL_SCANCODE_A: return sf::Keyboard::Scancode::A;
		case SDL_SCANCODE_D: return sf::Keyboard::Scancode::D;
		case SDL_SCANCODE_LEFT: return sf::Keyboard::Scancode::Left;
		case SDL_SCANCODE_RIGHT: return sf::Keyboard::Scancode::Right;
		case SDL_SCANCODE_R: return sf::Keyboard::Scancode::R;
		case SDL_SCANCODE_P: return sf::Keyboard::Scancode::P;
		case SDL_SCANCODE_ESCAPE: return sf::Keyboard::Scancode::Escape;
		case SDL_SCANCODE_EQUALS: return sf::Keyboard::Scancode::Equal;
		case SDL_SCANCODE_KP_EQUALS: return sf::Keyboard::Scancode::NumpadEqual;
		case SDL_SCANCODE_F2: return sf::Keyboard::Scancode::F2;
		default: return sf::Keyboard::Scancode::Unknown;
	}
}

#ifdef __EMSCRIPTEN__
EM_BOOL preventGameplayKeyScroll(int, const EmscriptenKeyboardEvent* event, void*) {
	switch (event->keyCode) {
		case 32:
		case 37:
		case 38:
		case 39:
		case 40:
			return EM_TRUE;
		default:
			return EM_FALSE;
	}
}
#endif

} // namespace

namespace sf {

void RenderWindow::create(VideoMode mode, const String& title) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		throw std::runtime_error(std::string("SDL initialization failed: ") + SDL_GetError());
	initialized = true;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
#ifdef __EMSCRIPTEN__
	SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
#endif
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		static_cast<int>(mode.size.x), static_cast<int>(mode.size.y),
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	if (!window)
		throw std::runtime_error(std::string("SDL window creation failed: ") + SDL_GetError());

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (!renderer)
		throw std::runtime_error(std::string("SDL renderer creation failed: ") + SDL_GetError());

	size = mode.size;
	if (SDL_RenderSetLogicalSize(renderer, static_cast<int>(size.x), static_cast<int>(size.y)) != 0)
		throw std::runtime_error(std::string("SDL logical canvas setup failed: ") + SDL_GetError());

	const int imageFlags = IMG_INIT_PNG;
	if ((IMG_Init(imageFlags) & imageFlags) != imageFlags)
		throw std::runtime_error(std::string("SDL_image initialization failed: ") + IMG_GetError());
	if (TTF_Init() != 0)
		throw std::runtime_error(std::string("SDL_ttf initialization failed: ") + TTF_GetError());
	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048) != 0)
		throw std::runtime_error(std::string("SDL_mixer initialization failed: ") + Mix_GetError());
	if (Mix_AllocateChannels(64) < 64)
		throw std::runtime_error(std::string("SDL_mixer channel allocation failed: ") + Mix_GetError());

	Texture::setRenderer(renderer);
#ifdef __EMSCRIPTEN__
	const auto keyHandler = emscripten_set_keydown_callback("#canvas", nullptr, EM_TRUE, preventGameplayKeyScroll);
	if (keyHandler != EMSCRIPTEN_RESULT_SUCCESS)
		throw std::runtime_error("Could not register browser gameplay-key handler.");
#endif
}

void RenderWindow::close() {
	if (!initialized) return;
	Text::clearCache();
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;
	Texture::setRenderer(nullptr);
	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	initialized = false;
}

void RenderWindow::clear(Color color) {
	if (!renderer) return;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(renderer);
}

void RenderWindow::display() {
	if (renderer) SDL_RenderPresent(renderer);
}

std::optional<Event> RenderWindow::pollEvent() {
	SDL_Event source;
	while (SDL_PollEvent(&source)) {
		Event event;
		switch (source.type) {
			case SDL_QUIT:
				event.type = Event::Type::Closed;
				event.closed = {};
				return event;
			case SDL_WINDOWEVENT:
				if (source.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
					event.type = Event::Type::FocusLost;
					event.focusLost = {};
					return event;
				}
				if (source.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
					event.type = Event::Type::FocusGained;
					event.focusGained = {};
					return event;
				}
				if (source.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					event.type = Event::Type::Resized;
					event.resized.size = {static_cast<unsigned int>(source.window.data1), static_cast<unsigned int>(source.window.data2)};
					return event;
				}
				break;
			case SDL_KEYDOWN:
				if (!audioUnlockedByInput) {
					Mix_ResumeMusic();
					Mix_Resume(-1);
					audioUnlockedByInput = true;
				}
				event.type = Event::Type::KeyPressed;
				event.keyPressed = {mapKey(source.key.keysym.sym), mapScancode(source.key.keysym.scancode)};
				return event;
			case SDL_KEYUP:
				event.type = Event::Type::KeyReleased;
				event.keyReleased = {mapKey(source.key.keysym.sym), mapScancode(source.key.keysym.scancode)};
				return event;
			default:
				break;
		}
	}
	return std::nullopt;
}

} // namespace sf
