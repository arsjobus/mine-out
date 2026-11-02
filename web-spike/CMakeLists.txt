cmake_minimum_required(VERSION 3.16)
project(MineOutWebGLSpike LANGUAGES CXX)

add_executable(webgl-spike main.cpp)
set_target_properties(webgl-spike PROPERTIES SUFFIX ".html")
target_link_options(webgl-spike PRIVATE
    "-sENVIRONMENT=web"
    "-sMIN_WEBGL_VERSION=2"
    "-sMAX_WEBGL_VERSION=2"
    "-sASSERTIONS=1"
)

add_executable(sdl2-spike sdl2-main.cpp)
set_target_properties(sdl2-spike PROPERTIES SUFFIX ".html")
target_compile_options(sdl2-spike PRIVATE "-sUSE_SDL=2")
target_link_options(sdl2-spike PRIVATE
    "-sENVIRONMENT=web"
    "-sUSE_SDL=2"
    "-sMIN_WEBGL_VERSION=2"
    "-sMAX_WEBGL_VERSION=2"
    "-sASSERTIONS=1"
)

add_executable(sdl2-capabilities sdl2-capabilities.cpp)
set_target_properties(sdl2-capabilities PROPERTIES SUFFIX ".html")
target_compile_options(sdl2-capabilities PRIVATE
    "-sUSE_SDL=2"
    "-sUSE_SDL_IMAGE=2"
    "-sUSE_SDL_TTF=2"
    "-sUSE_SDL_MIXER=2"
)
target_link_options(sdl2-capabilities PRIVATE
    "-sENVIRONMENT=web"
    "-sUSE_SDL=2"
    "-sUSE_SDL_IMAGE=2"
    "-sUSE_SDL_TTF=2"
    "-sUSE_SDL_MIXER=2"
    "-sSDL2_IMAGE_FORMATS=[\"png\"]"
    "-sUSE_VORBIS=1"
    "-sUSE_OGG=1"
    "-sMIN_WEBGL_VERSION=2"
    "-sMAX_WEBGL_VERSION=2"
    "-sASSERTIONS=1"
    "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/../assets@/assets"
)

add_executable(title-screen title-screen.cpp)
set_target_properties(title-screen PROPERTIES SUFFIX ".html")
target_compile_options(title-screen PRIVATE
    "-sUSE_SDL=2"
    "-sUSE_SDL_IMAGE=2"
    "-sUSE_SDL_TTF=2"
    "-sUSE_SDL_MIXER=2"
)
target_link_options(title-screen PRIVATE
    "-sENVIRONMENT=web"
    "-sUSE_SDL=2"
    "-sUSE_SDL_IMAGE=2"
    "-sUSE_SDL_TTF=2"
    "-sUSE_SDL_MIXER=2"
    "-sSDL2_IMAGE_FORMATS=[\"png\"]"
    "-sUSE_VORBIS=1"
    "-sUSE_OGG=1"
    "-sMIN_WEBGL_VERSION=2"
    "-sMAX_WEBGL_VERSION=2"
    "-sASSERTIONS=1"
    "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/../assets@/assets"
)
