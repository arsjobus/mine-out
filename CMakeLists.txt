cmake_minimum_required(VERSION 3.16)
project(MineOutGame)

# WebAssembly build option
option(WASM_BUILD "Enable WebAssembly browser build" OFF)

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

# If building for WebAssembly, require Emscripten and allow building against an Emscripten-compatible SFML.
if(WASM_BUILD)
    if(NOT EMSCRIPTEN)
        message(FATAL_ERROR "WASM_BUILD requires Emscripten. Run this with emcmake or set CMAKE_TOOLCHAIN_FILE to Emscripten.cmake.")
    endif()

    message(STATUS "WASM_BUILD enabled. Building with Emscripten-compatible SFML.")
    if(EMSCRIPTEN)
        set(CMAKE_EXECUTABLE_SUFFIX ".html")
        set(SFML_STATIC_LIBRARIES ON CACHE BOOL "Use SFML static libraries" FORCE)
    endif()
endif()

# Find SFML via pkg-config, Homebrew, or a provided Emscripten-compatible SFML installation.
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS src/*.h)

# Add executable
add_executable(game ${SOURCES})

# Make headers in nested source folders visible to the compiler
set(GAME_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src)
foreach(header_file IN LISTS HEADER_FILES)
    get_filename_component(header_dir "${header_file}" DIRECTORY)
    list(APPEND GAME_INCLUDE_DIRS "${header_dir}")
endforeach()
list(REMOVE_DUPLICATES GAME_INCLUDE_DIRS)

target_include_directories(game PRIVATE ${GAME_INCLUDE_DIRS})

# Link SFML libraries
target_link_libraries(game PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

# Enable async support for browser builds and preload the asset directory.
if(EMSCRIPTEN)
    target_link_options(game PRIVATE
        "-sASYNCIFY=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sASSERTIONS=2"
        "-sFORCE_FILESYSTEM=1"
        "--preload-file=${PROJECT_SOURCE_DIR}/assets@/assets"
    )

    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/assets"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets"
        COMMENT "Copy assets into web build output directory"
    )
endif()

# macOS frameworks (optional – SFML usually links these itself)
if(APPLE)
    target_link_libraries(game PRIVATE "-framework Foundation" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()
