cmake_minimum_required(VERSION 3.16)
project(MineOutGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for IDEs/linting

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Find SFML via pkg-config or Homebrew
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Add executable
add_executable(game ${SOURCES})

# Include directories if needed (e.g., for headers in include/)
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Link SFML libraries
target_link_libraries(game PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

# macOS frameworks (optional – SFML usually links these itself)
if(APPLE)
    target_link_libraries(game PRIVATE "-framework Foundation" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()
