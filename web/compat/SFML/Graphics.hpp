#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace sf {

template <typename T>
struct Vector2 {
	T x{};
	T y{};
	Vector2() = default;
	Vector2(T x, T y) : x(x), y(y) {}
	template <typename U>
	explicit Vector2(const Vector2<U>& value) : x(static_cast<T>(value.x)), y(static_cast<T>(value.y)) {}
};
using Vector2f = Vector2<float>;
using Vector2u = Vector2<unsigned int>;
using Vector2i = Vector2<int>;

struct Color {
	std::uint8_t r = 0, g = 0, b = 0, a = 255;
	constexpr Color() = default;
	constexpr Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255)
		: r(r), g(g), b(b), a(a) {}
	static const Color Black;
	static const Color White;
	static const Color Yellow;
};
inline const Color Color::Black{0, 0, 0};
inline const Color Color::White{255, 255, 255};
inline const Color Color::Yellow{255, 255, 0};

template <typename T>
struct Rect {
	Vector2<T> position{};
	Vector2<T> size{};
	Rect() = default;
	Rect(Vector2<T> position, Vector2<T> size) : position(position), size(size) {}
	std::optional<Rect<T>> findIntersection(const Rect<T>& other) const {
		const T left = std::max(position.x, other.position.x);
		const T top = std::max(position.y, other.position.y);
		const T right = std::min(position.x + size.x, other.position.x + other.size.x);
		const T bottom = std::min(position.y + size.y, other.position.y + other.size.y);
		if (right <= left || bottom <= top) return std::nullopt;
		return Rect<T>({left, top}, {right - left, bottom - top});
	}
};
using FloatRect = Rect<float>;
using IntRect = Rect<int>;

class Time {
public:
	constexpr Time() = default;
	explicit constexpr Time(float seconds) : value(seconds) {}
	float asSeconds() const { return value; }
	std::int64_t asMicroseconds() const { return static_cast<std::int64_t>(value * 1000000.0f); }
	static const Time Zero;
	friend bool operator>=(Time a, Time b) { return a.value >= b.value; }
	friend bool operator<(Time a, Time b) { return a.value < b.value; }
	friend Time operator+(Time a, Time b) { return Time(a.value + b.value); }
	Time& operator+=(Time other) { value += other.value; return *this; }
	std::int32_t asMilliseconds() const { return static_cast<std::int32_t>(value * 1000.f); }
private:
	float value = 0.f;
};
inline const Time Time::Zero{0.f};
inline Time seconds(float value) { return Time(value); }
inline Time milliseconds(int value) { return Time(value / 1000.f); }

class Clock {
public:
	Clock() : start(SDL_GetTicks64()) {}
	Time restart() {
		const auto now = SDL_GetTicks64();
		const auto elapsed = now - start;
		start = now;
		return Time(static_cast<float>(elapsed) / 1000.f);
	}
	Time getElapsedTime() const {
		return Time(static_cast<float>(SDL_GetTicks64() - start) / 1000.f);
	}
private:
	Uint64 start;
};

class Texture {
	struct Resource {
		SDL_Texture* texture = nullptr;
		Vector2u size{};
		~Resource() { if (texture) SDL_DestroyTexture(texture); }
	};
public:
	bool loadFromFile(const std::string& path) {
		SDL_Surface* surface = IMG_Load(path.c_str());
		if (!surface) return false;
		auto resource = std::make_shared<Resource>();
		resource->size = {static_cast<unsigned int>(surface->w), static_cast<unsigned int>(surface->h)};
		resource->texture = SDL_CreateTextureFromSurface(renderer(), surface);
		SDL_FreeSurface(surface);
		if (!resource->texture) return false;
		data = std::move(resource);
		return true;
	}
	Vector2u getSize() const { return data ? data->size : Vector2u{}; }
	SDL_Texture* native() const { return data ? data->texture : nullptr; }
	static void setRenderer(SDL_Renderer* value) { renderer() = value; }
	static SDL_Renderer* getRenderer() { return renderer(); }
private:
	static SDL_Renderer*& renderer() { static SDL_Renderer* value = nullptr; return value; }
	std::shared_ptr<Resource> data;
};

class Font {
public:
	bool openFromFile(const std::string& path) {
		if (!TTF_WasInit()) return false;
		TTF_Font* probe = TTF_OpenFont(path.c_str(), 16);
		if (!probe) return false;
		TTF_CloseFont(probe);
		filename = path;
		return true;
	}
	const std::string& path() const { return filename; }
private:
	std::string filename;
};

class Drawable {
public:
	virtual ~Drawable() = default;
	virtual void draw(SDL_Renderer* renderer) const = 0;
};

class RectangleShape : public Drawable {
public:
	void setSize(Vector2f value) { size = value; }
	Vector2f getSize() const { return size; }
	void setOrigin(Vector2f value) { origin = value; }
	Vector2f getOrigin() const { return origin; }
	void setPosition(Vector2f value) { position = value; }
	void setPosition(float x, float y) { position = {x, y}; }
	Vector2f getPosition() const { return position; }
	void setFillColor(Color value) { fill = value; }
	void setTexture(const Texture* value) { texture = value; }
	void setTextureRect(IntRect value) { rect = value; }
	FloatRect getLocalBounds() const { return {{0.f, 0.f}, size}; }
	virtual FloatRect getGlobalBounds() const {
		return {{position.x - origin.x, position.y - origin.y}, size};
	}
	void draw(SDL_Renderer* renderer) const override {
		SDL_Rect dst = destination();
		if (texture && texture->native()) {
			SDL_Rect source{rect.position.x, rect.position.y, rect.size.x, rect.size.y};
			SDL_RenderCopy(renderer, texture->native(),
				(rect.size.x > 0 && rect.size.y > 0) ? &source : nullptr, &dst);
			return;
		}
		SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, fill.a);
		SDL_RenderFillRect(renderer, &dst);
	}
protected:
	SDL_Rect destination() const {
		return {static_cast<int>(std::lround(position.x - origin.x)),
			static_cast<int>(std::lround(position.y - origin.y)),
			static_cast<int>(std::lround(size.x)), static_cast<int>(std::lround(size.y))};
	}
	Vector2f size{};
	Vector2f origin{};
	Vector2f position{};
	Color fill = Color::White;
	const Texture* texture = nullptr;
	IntRect rect{};
};

class CircleShape : public RectangleShape {
public:
	explicit CircleShape(float radius = 0.f) : radius(radius) { size = {radius * 2.f, radius * 2.f}; }
	void setRadius(float value) { radius = value; size = {value * 2.f, value * 2.f}; }
	float getRadius() const { return radius; }
	FloatRect getLocalBounds() const { return {{0.f, 0.f}, size}; }
	void draw(SDL_Renderer* renderer) const override {
		const int cx = static_cast<int>(std::lround(position.x - origin.x + radius));
		const int cy = static_cast<int>(std::lround(position.y - origin.y + radius));
		const int r = static_cast<int>(std::lround(radius));
		SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, fill.a);
		for (int dy = -r; dy <= r; ++dy) {
			const int dx = static_cast<int>(std::sqrt(std::max(0, r * r - dy * dy)));
			SDL_RenderDrawLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
		}
	}
private:
	float radius;
};

class Sprite : public RectangleShape {
public:
	using RectangleShape::setTexture;
	explicit Sprite(const Texture& texture) { setTexture(&texture); }
	void setTexture(const Texture& value) { texture = &value; size = {static_cast<float>(value.getSize().x), static_cast<float>(value.getSize().y)}; }
	void setScale(Vector2f value) { scale = value; size = {texture ? texture->getSize().x * value.x : 0.f, texture ? texture->getSize().y * value.y : 0.f}; }
	FloatRect getGlobalBounds() const override {
		return {{position.x - origin.x, position.y - origin.y}, size};
	}
	void draw(SDL_Renderer* renderer) const override {
		if (!texture || !texture->native()) return;
		SDL_Rect src{rect.position.x, rect.position.y, rect.size.x, rect.size.y};
		SDL_Rect* source = (rect.size.x > 0 && rect.size.y > 0) ? &src : nullptr;
		SDL_Rect dst{static_cast<int>(std::lround(position.x - origin.x)), static_cast<int>(std::lround(position.y - origin.y)),
			static_cast<int>(std::lround(size.x)), static_cast<int>(std::lround(size.y))};
		SDL_RenderCopy(renderer, texture->native(), source, &dst);
	}
private:
	Vector2f scale{1.f, 1.f};
};

class Text : public Drawable {
public:
	enum Style { Regular = 0, Bold = 1 };
	Text(const Font& font, const std::string& value = "", unsigned int characterSize = 30)
		: font(&font), string(value), characterSize(characterSize) {}
	void setFillColor(Color value) { color = value; }
	void setStyle(unsigned int value) { style = value; }
	void setStyle(Style value) { style = static_cast<unsigned int>(value); }
	void setCharacterSize(unsigned int value) { characterSize = value; }
	void setString(const std::string& value) { string = value; }
	void setPosition(Vector2f value) { position = value; }
	void setPosition(float x, float y) { position = {x, y}; }
	Vector2f getPosition() const { return position; }
	void setOrigin(Vector2f value) { origin = value; }
	Vector2f getOrigin() const { return origin; }
	void setOutlineColor(Color value) { outline = value; }
	void setOutlineThickness(float value) { outlineThickness = value; }
	FloatRect getGlobalBounds() const {
		const CachedText& cached = cachedText();
		return {{position.x - origin.x, position.y - origin.y},
			{static_cast<float>(cached.width), static_cast<float>(cached.height)}};
	}
	void draw(SDL_Renderer* renderer) const override {
		const CachedText& cached = cachedText();
		if (!cached.texture) return;
		SDL_Rect dst{static_cast<int>(std::lround(position.x - origin.x)),
			static_cast<int>(std::lround(position.y - origin.y)), cached.width, cached.height};
		SDL_RenderCopy(renderer, cached.texture, nullptr, &dst);
	}
	static void clearCache() {
		for (auto& entry : cache()) SDL_DestroyTexture(entry.second.texture);
		cache().clear();
	}
private:
	struct CachedText { SDL_Texture* texture = nullptr; int width = 0; int height = 0; };
	static std::unordered_map<std::string, CachedText>& cache() {
		static std::unordered_map<std::string, CachedText> values;
		return values;
	}
	const CachedText& cachedText() const {
		static const CachedText empty{};
		if (!font || string.empty()) return empty;
		const std::string key = font->path() + '\n' + string + '\n' +
			std::to_string(characterSize) + ':' + std::to_string(style) + ':' +
			std::to_string(color.r) + ':' + std::to_string(color.g) + ':' +
			std::to_string(color.b) + ':' + std::to_string(color.a);
		auto& values = cache();
		auto found = values.find(key);
		if (found != values.end()) return found->second;
		TTF_Font* face = TTF_OpenFont(font->path().c_str(), static_cast<int>(characterSize));
		if (!face) return empty;
		TTF_SetFontStyle(face, (style & Bold) ? TTF_STYLE_BOLD : TTF_STYLE_NORMAL);
		SDL_Color foreground{color.r, color.g, color.b, color.a};
		SDL_Surface* surface = TTF_RenderUTF8_Blended(face, string.c_str(), foreground);
		TTF_CloseFont(face);
		if (!surface) return empty;
		CachedText result;
		result.width = surface->w;
		result.height = surface->h;
		result.texture = SDL_CreateTextureFromSurface(Texture::getRenderer(), surface);
		SDL_FreeSurface(surface);
		if (result.texture) {
			SDL_SetTextureBlendMode(result.texture, SDL_BLENDMODE_BLEND);
			return values.emplace(key, result).first->second;
		}
		return empty;
	}
	const Font* font = nullptr;
	std::string string;
	unsigned int characterSize = 30;
	unsigned int style = Regular;
	Color color = Color::White;
	Color outline = Color::Black;
	float outlineThickness = 0.f;
	Vector2f position{};
	Vector2f origin{};
};

class String : public std::string {
public:
	using std::string::string;
	String() = default;
	String(const std::string& value) : std::string(value) {}
};

class View {
public:
	explicit View(FloatRect) {}
	void setViewport(FloatRect) {}
};

struct VideoMode { explicit VideoMode(Vector2u size) : size(size) {} Vector2u size; };

class RenderWindow {
public:
	virtual ~RenderWindow() { close(); }
	void create(VideoMode mode, const String& title);
	Vector2u getSize() const { return size; }
	void close();
	bool isOpen() const { return window != nullptr; }
	std::optional<class Event> pollEvent();
	void clear(Color color = Color::Black);
	void display();
	void draw(const Drawable& drawable) { if (renderer) drawable.draw(renderer); }
	void setView(const View&) {}
protected:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Vector2u size{};
	bool initialized = false;
	friend class Texture;
};

namespace Keyboard {
	enum class Key { Unknown, Add, Equal, F2, Space, Escape };
	enum class Scancode { Unknown, Equal, NumpadEqual, F2, Space, A, D, Left, Right, R, P, Escape };
}

class Event {
public:
	struct Closed {};
	struct FocusLost {};
	struct FocusGained {};
	struct Resized { Vector2u size{}; };
	struct KeyPressed { Keyboard::Key code = Keyboard::Key::Unknown; Keyboard::Scancode scancode = Keyboard::Scancode::Unknown; };
	struct KeyReleased { Keyboard::Key code = Keyboard::Key::Unknown; Keyboard::Scancode scancode = Keyboard::Scancode::Unknown; };
	struct TextEntered { char32_t unicode = 0; };
	enum class Type { None, Closed, FocusLost, FocusGained, Resized, KeyPressed, KeyReleased, TextEntered };
	Type type = Type::None;
	Closed closed{};
	FocusLost focusLost{};
	FocusGained focusGained{};
	Resized resized{};
	KeyPressed keyPressed{};
	KeyReleased keyReleased{};
	TextEntered textEntered{};
	template <typename T> bool is() const { return type == eventType<T>(); }
	template <typename T> const T* getIf() const {
		if (!is<T>()) return nullptr;
		if constexpr (std::is_same_v<T, Closed>) return &closed;
		else if constexpr (std::is_same_v<T, FocusLost>) return &focusLost;
		else if constexpr (std::is_same_v<T, FocusGained>) return &focusGained;
		else if constexpr (std::is_same_v<T, Resized>) return &resized;
		else if constexpr (std::is_same_v<T, KeyPressed>) return &keyPressed;
		else if constexpr (std::is_same_v<T, KeyReleased>) return &keyReleased;
		else if constexpr (std::is_same_v<T, TextEntered>) return &textEntered;
		else return nullptr;
	}
private:
	template <typename T> static constexpr Type eventType() {
		if constexpr (std::is_same_v<T, Closed>) return Type::Closed;
		else if constexpr (std::is_same_v<T, FocusLost>) return Type::FocusLost;
		else if constexpr (std::is_same_v<T, FocusGained>) return Type::FocusGained;
		else if constexpr (std::is_same_v<T, Resized>) return Type::Resized;
		else if constexpr (std::is_same_v<T, KeyPressed>) return Type::KeyPressed;
		else if constexpr (std::is_same_v<T, KeyReleased>) return Type::KeyReleased;
		else if constexpr (std::is_same_v<T, TextEntered>) return Type::TextEntered;
		else return Type::None;
	}
	friend class RenderWindow;
};

} // namespace sf
