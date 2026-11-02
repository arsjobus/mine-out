#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <emscripten.h>
#include <emscripten/html5.h>

#include <algorithm>
#include <cstdio>

namespace
{
struct Spike
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* image = nullptr;
    SDL_Texture* text = nullptr;
    Mix_Music* music = nullptr;
    Mix_Chunk* effect = nullptr;
    bool focused = true;
    bool visible = true;
    bool audioStarted = false;
    bool effectPlayed = false;
    double previousFrameTime = 0.0;
    double frameDeltaMs = 0.0;
    double maximumFrameDeltaMs = 0.0;
    int canvasWidth = 0;
    int canvasHeight = 0;
    int canvasResizeResult = 0;
    unsigned int resizeCallbacks = 0;
    unsigned int resizeEvents = 0;
    unsigned int keyEvents = 0;
    unsigned int frames = 0;
};

EM_BOOL preventGameplayKeyScroll(int, const EmscriptenKeyboardEvent* event, void*)
{
    switch (event->keyCode)
    {
        case 32: // Space
        case 33: // Page Up
        case 34: // Page Down
        case 35: // End
        case 36: // Home
        case 37: // Left
        case 38: // Up
        case 39: // Right
        case 40: // Down
            return EM_TRUE;
        default:
            return EM_FALSE;
    }
}

EM_BOOL handleVisibilityChange(int, const EmscriptenVisibilityChangeEvent* event, void* userData)
{
    auto& spike = *static_cast<Spike*>(userData);
    spike.visible = !event->hidden;
    spike.previousFrameTime = emscripten_get_now();
    return EM_TRUE;
}

EM_BOOL resizeCanvas(int, const EmscriptenUiEvent* event, void* userData)
{
    auto& spike = *static_cast<Spike*>(userData);
    ++spike.resizeCallbacks;
    int width = event->windowInnerWidth;
    int height = event->windowInnerHeight;
    if (width <= 0 || height <= 0)
        return EM_TRUE;

    if (width * 3 > height * 4)
        width = height * 4 / 3;
    else
        height = width * 3 / 4;

    spike.canvasWidth = width;
    spike.canvasHeight = height;
    spike.canvasResizeResult = emscripten_set_canvas_element_size("#canvas", width, height);
    SDL_SetWindowSize(spike.window, width, height);
    return EM_TRUE;
}

void reportCapabilities(const Spike& spike)
{
    EM_ASM({
        window.sdl2Capabilities = window.sdl2Capabilities || {};
        window.sdl2Capabilities.ready = $0;
        window.sdl2Capabilities.image = $1;
        window.sdl2Capabilities.text = $2;
        window.sdl2Capabilities.music = $3;
        window.sdl2Capabilities.effect = $4;
        window.sdl2Capabilities.focused = $5;
        window.sdl2Capabilities.visible = $6;
        window.sdl2Capabilities.audioStarted = $7;
        window.sdl2Capabilities.effectPlayed = $8;
    }, spike.image && spike.text && spike.music && spike.effect,
       spike.image != nullptr, spike.text != nullptr, spike.music != nullptr,
       spike.effect != nullptr, spike.focused, spike.visible, spike.audioStarted,
       spike.effectPlayed);
    EM_ASM({
        window.sdl2Capabilities.deltaMs = $0;
        window.sdl2Capabilities.maxDeltaMs = $1;
        window.sdl2Capabilities.resizeCallbacks = $2;
        window.sdl2Capabilities.canvasWidth = $3;
        window.sdl2Capabilities.canvasHeight = $4;
        window.sdl2Capabilities.canvasResizeResult = $5;
        window.sdl2Capabilities.resizeEvents = $6;
        window.sdl2Capabilities.keyEvents = $7;
        window.sdl2Capabilities.frames = $8;
    }, spike.frameDeltaMs, spike.maximumFrameDeltaMs,
       spike.resizeCallbacks, spike.canvasWidth, spike.canvasHeight,
       spike.canvasResizeResult, spike.resizeEvents, spike.keyEvents, spike.frames);
}

void renderFrame(void* userData)
{
    auto& spike = *static_cast<Spike*>(userData);
    const double now = emscripten_get_now();
    spike.frameDeltaMs = 0.0;
    if (spike.visible && spike.focused && spike.previousFrameTime > 0.0)
    {
        spike.frameDeltaMs = std::min(now - spike.previousFrameTime, 100.0);
        spike.maximumFrameDeltaMs = std::max(spike.maximumFrameDeltaMs, spike.frameDeltaMs);
    }
    spike.previousFrameTime = now;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            emscripten_cancel_main_loop();
            return;
        }
        if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                spike.focused = false;
            else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                spike.focused = true;
            else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                ++spike.resizeEvents;
        }
        if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
        {
            ++spike.keyEvents;
            if (!spike.audioStarted && spike.music)
            {
                if (Mix_PlayMusic(spike.music, -1) == 0)
                    spike.audioStarted = true;
                else
                    std::fprintf(stderr, "Failed to start OGG music: %s\n", Mix_GetError());
            }
            if (event.key.keysym.sym == SDLK_e && spike.effect)
            {
                if (Mix_PlayChannel(-1, spike.effect, 0) < 0)
                    std::fprintf(stderr, "Failed to play WAV effect: %s\n", Mix_GetError());
                else
                    spike.effectPlayed = true;
            }
        }
    }

    SDL_SetRenderDrawColor(spike.renderer, 18, 30, 48, 255);
    SDL_RenderClear(spike.renderer);

    SDL_Rect background{0, 0, 1024, 768};
    SDL_SetRenderDrawColor(spike.renderer, 38, 67, 93, 255);
    SDL_RenderFillRect(spike.renderer, &background);

    if (spike.image)
    {
        SDL_Rect imageRect{100, 100, 128, 128};
        SDL_RenderCopy(spike.renderer, spike.image, nullptr, &imageRect);
    }
    if (spike.text)
    {
        SDL_Rect textRect{260, 100, 420, 72};
        SDL_RenderCopy(spike.renderer, spike.text, nullptr, &textRect);
    }

    SDL_Rect translucentOverlay{80, 80, 700, 150};
    SDL_SetRenderDrawBlendMode(spike.renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(spike.renderer, 220, 80, 40, 128);
    SDL_RenderFillRect(spike.renderer, &translucentOverlay);

    SDL_Rect footer{0, 730, 1024, 38};
    SDL_SetRenderDrawBlendMode(spike.renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(spike.renderer, spike.focused ? 36 : 120, 150, 90, 255);
    SDL_RenderFillRect(spike.renderer, &footer);
    SDL_RenderPresent(spike.renderer);

    ++spike.frames;
    reportCapabilities(spike);
}

bool loadResources(Spike& spike)
{
    const int imageFlags = IMG_INIT_PNG;
    if ((IMG_Init(imageFlags) & imageFlags) != imageFlags)
        std::fprintf(stderr, "Failed to initialize SDL_image: %s\n", IMG_GetError());

    spike.image = IMG_LoadTexture(spike.renderer, "/assets/images/blocks/spr_block_default.png");
    if (!spike.image)
        std::fprintf(stderr, "Failed to load PNG texture: %s\n", IMG_GetError());

    if (TTF_Init() != 0)
        std::fprintf(stderr, "Failed to initialize SDL_ttf: %s\n", TTF_GetError());
    TTF_Font* font = TTF_OpenFont("/assets/fonts/fnt_liberation_sans.otf", 42);
    if (!font)
        std::fprintf(stderr, "Failed to load OTF font: %s\n", TTF_GetError());
    else
    {
        SDL_Color color{255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderUTF8_Blended(font, "SDL2 WebGL capability check", color);
        if (!surface)
            std::fprintf(stderr, "Failed to render text: %s\n", TTF_GetError());
        else
        {
            spike.text = SDL_CreateTextureFromSurface(spike.renderer, surface);
            SDL_FreeSurface(surface);
            if (!spike.text)
                std::fprintf(stderr, "Failed to create text texture: %s\n", SDL_GetError());
        }
        TTF_CloseFont(font);
    }

    if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048) != 0)
        std::fprintf(stderr, "Failed to open audio: %s\n", Mix_GetError());
    spike.music = Mix_LoadMUS("/assets/music/bgm_crystal_cave.ogg");
    if (!spike.music)
        std::fprintf(stderr, "Failed to load OGG music: %s\n", Mix_GetError());
    spike.effect = Mix_LoadWAV("/assets/sfx/sfx_paddle_hit.wav");
    if (!spike.effect)
        std::fprintf(stderr, "Failed to load WAV effect: %s\n", Mix_GetError());

    return spike.image && spike.text && spike.music && spike.effect;
}
} // namespace

int main()
{
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        std::fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    Spike spike;
    spike.window = SDL_CreateWindow("SDL2 WebGL capabilities",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   1024,
                                   768,
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!spike.window)
    {
        std::fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
        return 1;
    }
    spike.renderer = SDL_CreateRenderer(spike.window, -1, SDL_RENDERER_ACCELERATED);
    if (!spike.renderer)
    {
        std::fprintf(stderr, "Failed to create SDL renderer: %s\n", SDL_GetError());
        return 1;
    }
    if (SDL_RenderSetLogicalSize(spike.renderer, 1024, 768) != 0)
        std::fprintf(stderr, "Failed to set logical render size: %s\n", SDL_GetError());

    const EMSCRIPTEN_RESULT keyResult =
        emscripten_set_keydown_callback("#canvas", nullptr, EM_TRUE, preventGameplayKeyScroll);
    if (keyResult != EMSCRIPTEN_RESULT_SUCCESS)
        std::fprintf(stderr, "Failed to register keyboard callback: %d\n", keyResult);

    const EMSCRIPTEN_RESULT visibilityResult =
        emscripten_set_visibilitychange_callback(&spike, EM_TRUE, handleVisibilityChange);
    if (visibilityResult != EMSCRIPTEN_RESULT_SUCCESS)
        std::fprintf(stderr, "Failed to register visibility callback: %d\n", visibilityResult);

    const EMSCRIPTEN_RESULT resizeResult =
        emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,
                                   &spike,
                                   EM_TRUE,
                                   resizeCanvas);
    if (resizeResult != EMSCRIPTEN_RESULT_SUCCESS)
        std::fprintf(stderr, "Failed to register browser resize callback: %d\n", resizeResult);

    const bool resourcesLoaded = loadResources(spike);
    if (!resourcesLoaded)
        std::fprintf(stderr, "One or more required capability assets failed to load.\n");

    emscripten_set_main_loop_arg(renderFrame, &spike, 0, EM_TRUE);
}
