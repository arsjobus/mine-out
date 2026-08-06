#pragma once

// Platform-level abstractions for desktop and future browser backends.
//
// Current repository uses the existing desktop SFML implementations.
// A browser port can later provide alternative definitions for these types
// without changing the shared game logic.

#include "Window.h"
#include "Config.h"
#include "PreloadResources.h"

namespace Platform {
    using Window = ::Window;
    using Config = ::Config;
    using ResourceLoader = ::PreloadResources;
}
