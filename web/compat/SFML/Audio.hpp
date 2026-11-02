#pragma once

#include "Graphics.hpp"
#include <SDL_mixer.h>

#include <memory>

namespace sf {

class SoundBuffer {
	struct Resource {
		Mix_Chunk* chunk = nullptr;
		~Resource() { if (chunk) Mix_FreeChunk(chunk); }
	};
public:
	bool loadFromFile(const std::string& path) {
		Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
		if (!chunk) return false;
		auto resource = std::make_shared<Resource>();
		resource->chunk = chunk;
		data = std::move(resource);
		return true;
	}
	Mix_Chunk* native() const { return data ? data->chunk : nullptr; }
private:
	std::shared_ptr<Resource> data;
};

class Sound {
public:
	Sound() = default;
	explicit Sound(const SoundBuffer& buffer) : buffer(&buffer) {}
	void setLooping(bool value) { loops = value; }
	void play() {
		if (!buffer || !buffer->native()) return;
		if (channel >= 0 && Mix_GetChunk(channel) == buffer->native()) {
			if (Mix_Paused(channel)) {
				Mix_Resume(channel);
				return;
			}
			if (Mix_Playing(channel)) return;
		}
		channel = Mix_PlayChannel(-1, buffer->native(), loops ? -1 : 0);
	}
	void pause() { if (channel >= 0) Mix_Pause(channel); }
	void stop() { if (channel >= 0) Mix_HaltChannel(channel); }
private:
	const SoundBuffer* buffer = nullptr;
	int channel = -1;
	bool loops = false;
};

} // namespace sf
