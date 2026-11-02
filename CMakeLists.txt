cmake_minimum_required(VERSION 3.16)
project(MineOutGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for IDEs/linting

# macOS build defaults
if(APPLE AND NOT DEFINED CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS version" FORCE)
endif()

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# The native game uses SFML. Emscripten builds use the SDL2 compatibility
# adapter, keeping the desktop dependencies and code path unchanged.
if(NOT EMSCRIPTEN)
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})
    find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)
endif()

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS src/*.h)

# Add executable
if(EMSCRIPTEN)
    list(APPEND SOURCES web/compat/Runtime.cpp)
endif()
add_executable(game ${SOURCES})

# Make headers in nested source folders visible to the compiler
set(GAME_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src)
foreach(header_file IN LISTS HEADER_FILES)
    get_filename_component(header_dir "${header_file}" DIRECTORY)
    list(APPEND GAME_INCLUDE_DIRS "${header_dir}")
endforeach()
list(REMOVE_DUPLICATES GAME_INCLUDE_DIRS)

target_include_directories(game PRIVATE ${GAME_INCLUDE_DIRS})

if(EMSCRIPTEN)
    target_include_directories(game BEFORE PRIVATE ${PROJECT_SOURCE_DIR}/web/compat)
    target_compile_options(game PRIVATE
        "-sUSE_SDL=2"
        "-sUSE_SDL_IMAGE=2"
        "-sUSE_SDL_TTF=2"
        "-sUSE_SDL_MIXER=2"
    )
    target_link_options(game PRIVATE
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
        "-sALLOW_MEMORY_GROWTH=1"
        "-sFORCE_FILESYSTEM=1"
        "-sASSERTIONS=1"
        "--shell-file=${PROJECT_SOURCE_DIR}/web/shell.html"
        "--preload-file=${PROJECT_SOURCE_DIR}/assets@/assets"
    )
    set_target_properties(game PROPERTIES SUFFIX ".html")
else()
    target_link_libraries(game PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
    )

    if(APPLE)
        target_link_libraries(game PRIVATE "-framework Foundation" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
    endif()
endif()
