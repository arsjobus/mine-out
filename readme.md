# Mine-Out Game (SFML C++)

<p align="center">
  <img src="project_screenshot.png" alt="Project Screenshot" width="600">
</p>

This is a C++ game built using SFML 3.0.2 on macOS ARM architecture (Apple Silicon).
The project uses C++17 standard and links SFML dynamically, relying on system-installed libraries.
Historically, I created this project over 15 years ago to learn to make video games, so the source
code has been upgraded to be compatible with v3 of SFML.

---

## Dependencies

Before building, you need to install the following dependencies via Homebrew ([https://brew.sh/](https://brew.sh/)):

* SFML (Simple and Fast Multimedia Library)
* Freetype (Font rendering library)
* libvorbis (Audio codec library)
* Emscripten (for WebAssembly/browser builds)

---

## Installing Dependencies on macOS (Apple Silicon)

1. Install Homebrew (if you haven’t already) by running this command in your terminal:
   `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`

2. Install SFML and dependencies by running:
   `brew install sfml freetype libvorbis`

3. Install Emscripten if you want to build the web version:
   `brew install emscripten`

---

## Build Instructions (Using CMake)

1. Clone or download the project.

2. Open Terminal and cd into the project directory.

3. Create a new build directory:

   `mkdir build`

4. Run CMake to configure the project:

   `cmake -S . -B build`

5. Build the game:

   `cmake --build build`

6. Run the game:

   `./build/game`

### WebAssembly support

This repository can now be built for the web using an Emscripten-capable SFML fork. The current desktop code path is still native SFML, but the build system has been updated to allow a browser target with the proper SFML package.

### Web build setup

1. Clone the Emscripten SFML fork and build it locally:
   ```bash
   cd /tmp
   rm -rf sfml-emcc-fork
   git clone --depth 1 --branch 3.1.0-EMCC https://github.com/Zombieschannel/SFML.git sfml-emcc-fork
   cd sfml-emcc-fork
   rm -rf build-emscripten
   emcmake cmake -S . -B build-emscripten -DCMAKE_BUILD_TYPE=Release
   cmake --build build-emscripten --config Release
   cmake --install build-emscripten --prefix /tmp/sfml-emcc-fork/build-emscripten/install
   ```

2. Build `mine-out` for the web:
   ```bash
   cd /Users/mypc/Documents/GitHub/mine-out
   rm -rf build-web
   emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release \
     -DWASM_BUILD=ON \
     -DSFML_DIR=/tmp/sfml-emcc-fork/build-emscripten/install/lib/cmake/SFML \
     -DSFML_STATIC_LIBRARIES=ON \
     -DCMAKE_FIND_FRAMEWORK=NEVER
   cmake --build build-web --config Release
   ```

3. The generated browser target files will be in `build-web/`, including:
   * `game.html`
   * `game.js`
   * `game.wasm`

4. Serve `build-web/` from a local HTTP server to run it in a browser.
   Example:
   ```bash
   cd build-web
   python3 -m http.server 8000
   open http://localhost:8000/game.html
   ```

### macOS: Intel, Apple Silicon, or Universal Binary

- For Apple Silicon Macs:
  `cmake -S . -B build -DCMAKE_OSX_ARCHITECTURES=arm64`

- For older Intel Macs:
  `cmake -S . -B build -DCMAKE_OSX_ARCHITECTURES=x86_64`

- For a universal macOS binary (Intel + Apple Silicon), make sure SFML and its dependencies are also installed as universal libraries, then build with:
  `cmake -S . -B build-universal -DCMAKE_OSX_ARCHITECTURES='x86_64;arm64' -DCMAKE_OSX_DEPLOYMENT_TARGET=10.15`

  `cmake --build build-universal`

---

## Notes

* The executable dynamically links to SFML and its dependencies installed via Homebrew.
* There is no need to bundle dylib files manually or sign the executable.
* If you encounter library loading errors, ensure your Homebrew environment variables are set correctly. For example:

For Apple Silicon Macs, run:
export PATH="/opt/homebrew/bin:$PATH"

For Intel Macs, run:
export PATH="/usr/local/bin:$PATH"

* You can clean build files by running:
  make clean

---

## Troubleshooting

* Library not loaded errors: Make sure SFML and its dependencies are installed with Homebrew.
* Wrong architecture: Confirm you’re building on an ARM Mac or use the correct Homebrew prefix for your architecture.
* Permissions or sandbox issues: Run your terminal with appropriate permissions or disable system security temporarily for testing.

---

Enjoy playing and modifying the game! Feel free to submit issues or pull requests.