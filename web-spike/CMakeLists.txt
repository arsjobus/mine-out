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
