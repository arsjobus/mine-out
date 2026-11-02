#include <GLES3/gl3.h>
#include <emscripten.h>
#include <emscripten/html5.h>

#include <cstdio>

namespace
{
EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = 0;

void renderFrame()
{
    EM_ASM({ window.webglSpikeFrames = (window.webglSpikeFrames || 0) + 1; });
    glClearColor(0.1F, 0.25F, 0.5F, 1.F);
    glClear(GL_COLOR_BUFFER_BIT);
}
} // namespace

int main()
{
    EmscriptenWebGLContextAttributes attributes;
    emscripten_webgl_init_context_attributes(&attributes);
    attributes.majorVersion = 2;
    attributes.minorVersion = 0;

    context = emscripten_webgl_create_context("#canvas", &attributes);
    if (context <= 0)
    {
        std::fprintf(stderr,
                     "Failed to create a WebGL 2 context: %lu\n",
                     static_cast<unsigned long>(context));
        return 1;
    }

    const EMSCRIPTEN_RESULT result = emscripten_webgl_make_context_current(context);
    if (result != EMSCRIPTEN_RESULT_SUCCESS)
    {
        std::fprintf(stderr, "Failed to activate the WebGL context: %d\n", result);
        return 1;
    }

    emscripten_set_main_loop(renderFrame, 0, EM_TRUE);
}
