#pragma once

#include <string>

class TitleSceneRenderer {
public:
	enum class Font {
		Display,
		Body
	};

	virtual ~TitleSceneRenderer() = default;

	virtual void beginFrame() = 0;
	virtual void drawCenteredText(
		const std::string& value,
		Font font,
		unsigned int characterSize,
		unsigned char red,
		unsigned char green,
		unsigned char blue,
		float x,
		float y,
		bool bold,
		float outlineThickness
	) = 0;
	virtual void present() = 0;
};

class TitleScene {
public:
	enum class Key {
		Other,
		Space,
		Escape
	};

	enum class Action {
		None,
		StartGame,
		Exit
	};

	TitleScene(unsigned int width, unsigned int height, std::string title);

	Action handleKey(Key key) const;
	void render(TitleSceneRenderer& renderer) const;

private:
	unsigned int width;
	unsigned int height;
	std::string title;
};
