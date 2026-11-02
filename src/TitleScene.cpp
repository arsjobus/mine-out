#include "TitleScene.h"

#include <algorithm>
#include <utility>

TitleScene::TitleScene(unsigned int width, unsigned int height, std::string title)
	: width(width), height(height), title(std::move(title)) {}

TitleScene::Action TitleScene::handleKey(Key key) const {
	switch (key) {
		case Key::Space:
			return Action::StartGame;
		case Key::Escape:
			return Action::Exit;
		case Key::Other:
			return Action::None;
	}
	return Action::None;
}

void TitleScene::render(TitleSceneRenderer& renderer) const {
	const unsigned int titleSize = static_cast<unsigned int>(
		std::min<float>(static_cast<float>(height) / 5.f, 120.f)
	);
	const unsigned int instructionSize = static_cast<unsigned int>(
		std::max<int>(18, static_cast<int>(titleSize / 5))
	);

	renderer.beginFrame();
	renderer.drawCenteredText(
		title, TitleSceneRenderer::Font::Display, titleSize, 255, 255, 255,
		static_cast<float>(width) / 2.f,
		static_cast<float>(height) * 0.28f,
		true,
		std::max(1.f, titleSize * 0.03f)
	);
	renderer.drawCenteredText(
		"PRESS <SPACE>", TitleSceneRenderer::Font::Body, instructionSize, 255, 255, 255,
		static_cast<float>(width) / 2.f,
		static_cast<float>(height) - 100.f,
		true,
		0.f
	);
	renderer.present();
}
