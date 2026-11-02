#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <emscripten.h>
#include <emscripten/html5.h>

#include <algorithm>
#include <cstdio>
#include <string>

namespace
{
struct TitleScreenState
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* background = nullptr;
    SDL_Texture* titleText = nullptr;
    SDL_Texture* promptText = nullptr;
    Mix_Music* music = nullptr;
    bool focused = true;
    bool visible = true;
    bool audioUnlocked = false;
    bool ready = false;
    double previousFrameTime = 0.0;
    double frameDeltaMs = 0.0;
    int keyEvents = 0;
    int resizeCallbacks = 0;
    int canvasWidth = 1024;
    int canvasHeight = 768;
};

SDL_Texture* makeTextTexture(SDL_Renderer* renderer, const char* text, int fontSize, SDL_Color color)
{
    TTF_Font* font = TTF_OpenFont("/assets/fonts/fnt_liberation_sans.otf", fontSize);
    if (font == nullptr)
    {
        std::fprintf(stderr, "Failed to open title font: %s\n", TTF_GetError());
        return nullptr;
    }

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    TTF_CloseFont(font);
    if (surface == nullptr)
    {
        std::fprintf(stderr, "Failed to render title text: %s\n", TTF_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr)
    {
        std::fprintf(stderr, "Failed to create text texture: %s\n", SDL_GetError());
        return nullptr;
    }

    return texture;
}

EM_BOOL preventGameplayKeyScroll(int, const EmscriptenKeyboardEvent* event, void*)
{
    switch (event->keyCode)
    {
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
            return EM_TRUE;
        default:
            return EM_FALSE;
    }
}

EM_BOOL handleVisibilityChange(int, const EmscriptenVisibilityChangeEvent* event, void* userData)
{
    auto* state = static_cast<TitleScreenState*>(userData);
    state->visible = !event->hidden;
    state->previousFrameTime = emscripten_get_now();
    return EM_TRUE;
}

EM_BOOL resizeCanvas(int, const EmscriptenUiEvent* event, void* userData)
{
    auto* state = static_cast<TitleScreenState*>(userData);
    ++state->resizeCallbacks;

    int width = event->windowInnerWidth;
    int height = event->windowInnerHeight;
    if (width <= 0 || height <= 0)
        return EM_TRUE;

    if (width * 3 > height * 4)
        width = height * 4 / 3;
    else
        height = width * 3 / 4;

    state->canvasWidth = width;
    state->canvasHeight = height;
    emscripten_set_canvas_element_size("#canvas", width, height);
    SDL_SetWindowSize(state->window, width, height);
    return EM_TRUE;
}

void reportState(const TitleScreenState& state)
{
    EM_ASM({
        window.titleScreen = window.titleScreen || {};
        window.titleScreen.ready = $0;
        window.titleScreen.audioUnlocked = $1;
        window.titleScreen.focused = $2;
        window.titleScreen.visible = $3;
        window.titleScreen.frameDeltaMs = $4;
        window.titleScreen.canvasWidth = $5;
        window.titleScreen.canvasHeight = $6;
        window.titleScreen.keyEvents = $7;
        window.titleScreen.resizeCallbacks = $8;
    }, state.ready, state.audioUnlocked, state.focused, state.visible,
       state.frameDeltaMs, state.canvasWidth, state.canvasHeight,
       state.keyEvents, state.resizeCallbacks);
}

void renderFrame(void* userData)
{
    auto* state = static_cast<TitleScreenState*>(userData);

    const double now = emscripten_get_now();
    state->frameDeltaMs = 0.0;
    if (state->visible && state->focused && state->previousFrameTime > 0.0)
        state->frameDeltaMs = std::min(now - state->previousFrameTime, 100.0);
    state->previousFrameTime = now;

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
                state->focused = false;
            else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                state->focused = true;
        }

        if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
        {
            ++state->keyEvents;
            if (!state->audioUnlocked && state->music)
            {
                if (Mix_PlayMusic(state->music, -1) == 0)
                    state->audioUnlocked = true;
                else
                    std::fprintf(stderr, "Failed to start music: %s\n", Mix_GetError());
            }
        }
    }

    SDL_SetRenderDrawColor(state->renderer, 10, 16, 28, 255);
    SDL_RenderClear(state->renderer);

    if (state->background)
    {
        int w = 1024;
        int h = 768;
        SDL_Rect dst{0, 0, w, h};
        SDL_RenderCopy(state->renderer, state->background, nullptr, &dst);
    }

    SDL_Rect footer{0, 720, 1024, 48};
    SDL_SetRenderDrawBlendMode(state->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(state->renderer, 8, 8, 12, 180);
    SDL_RenderFillRect(state->renderer, &footer);

    if (state->titleText)
    {
        int w = 0;
        int h = 0;
        SDL_QueryTexture(state->titleText, nullptr, nullptr, &w, &h);
        SDL_Rect dst{(1024 - w) / 2, 120, w, h};
        SDL_RenderCopy(state->renderer, state->titleText, nullptr, &dst);
    }

    if (state->promptText)
    {
        int w = 0;
        int h = 0;
        SDL_QueryTexture(state->promptText, nullptr, nullptr, &w, &h);
        SDL_Rect dst{(1024 - w) / 2, 660, w, h};
        SDL_RenderCopy(state->renderer, state->promptText, nullptr, &dst);
    }

    SDL_RenderPresent(state->renderer);
    reportState(*state);
}

bool loadResources(TitleScreenState& state)
{
    const int imageFlags = IMG_INIT_PNG;
    if ((IMG_Init(imageFlags) & imageFlags) != imageFlags)
        std::fprintf(stderr, "Failed to initialize SDL_image: %s\n", IMG_GetError());

    state.background = IMG_LoadTexture(state.renderer, "/assets/images/backgrounds/spr_background.png");
    if (state.background == nullptr)
        std::fprintf(stderr, "Failed to load background texture: %s\n", IMG_GetError());

    if (TTF_Init() != 0)
        std::fprintf(stderr, "Failed to initialize SDL_ttf: %s\n", TTF_GetError());

    state.titleText = makeTextTexture(state.renderer, "MINE OUT", 80, {255, 255, 255, 255});
    state.promptText = makeTextTexture(state.renderer, "PRESS SPACE", 36, {220, 220, 255, 255});

    if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048) != 0)
        std::fprintf(stderr, "Failed to open audio: %s\n", Mix_GetError());

    state.music = Mix_LoadMUS("/assets/music/bgm_crystal_cave.ogg");
    if (state.music == nullptr)
        std::fprintf(stderr, "Failed to load music: %s\n", Mix_GetError());

    state.ready = state.background != nullptr && state.titleText != nullptr && state.promptText != nullptr;
    return state.ready;
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

    TitleScreenState state;
    state.window = SDL_CreateWindow("Mine Out WebGL title",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    1024,
                                    768,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (state.window == nullptr)
    {
        std::fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
        return 1;
    }

    state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED);
    if (state.renderer == nullptr)
    {
        std::fprintf(stderr, "Failed to create SDL renderer: %s\n", SDL_GetError());
        return 1;
    }

    if (SDL_RenderSetLogicalSize(state.renderer, 1024, 768) != 0)
        std::fprintf(stderr, "Failed to set logical render size: %s\n", SDL_GetError());

    if (!loadResources(state))
        std::fprintf(stderr, "Title-screen resources did not load successfully.\n");

    const EMSCRIPTEN_RESULT keyResult =
        emscripten_set_keydown_callback("#canvas", nullptr, EM_TRUE, preventGameplayKeyScroll);
    if (keyResult != EMSCRIPTEN_RESULT_SUCCESS)
        std::fprintf(stderr, "Failed to register keyboard callback: %d\n", keyResult);

    const EMSCRIPTEN_RESULT visibilityResult =
        emscripten_set_visibilitychange_callback(&state, EM_TRUE, handleVisibilityChange);
    if (visibilityResult != EMSCRIPTEN_RESULT_SUCCESS)
        std::fprintf(stderr, "Failed to register visibility callback: %d\n", visibilityResult);

    const EMSCRIPTEN_RESULT resizeResult =
        emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, &state, EM_TRUE, resizeCanvas);
    if (resizeResult != EMSCRIPTEN_RESULT_SUCCESS)
        std::fprintf(stderr, "Failed to register resize callback: %d\n", resizeResult);

    state.previousFrameTime = emscripten_get_now();
    reportState(state);
    emscripten_set_main_loop_arg(renderFrame, &state, 0, EM_TRUE);
    return 0;
}
