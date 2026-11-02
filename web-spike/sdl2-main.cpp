#include <SDL.h>
#include <GLES3/gl3.h>
#include <emscripten.h>

#include <cstdio>

namespace
{
void renderFrame(void* window)
{
    EM_ASM({ window.sdl2SpikeFrames = (window.sdl2SpikeFrames || 0) + 1; });
    glClearColor(0.1F, 0.25F, 0.5F, 1.F);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(static_cast<SDL_Window*>(window));
}
} // namespace

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_Window* window = SDL_CreateWindow("SDL2 WebGL spike",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640,
                                          480,
                                          SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        std::fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr)
    {
        std::fprintf(stderr, "Failed to create WebGL context: %s\n", SDL_GetError());
        return 1;
    }

    emscripten_set_main_loop_arg(renderFrame, window, 0, EM_TRUE);
}
