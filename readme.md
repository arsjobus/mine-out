# Mine-Out Game (SFML C++)

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

---

## Installing Dependencies on macOS (Apple Silicon)

1. Install Homebrew (if you haven’t already) by running this command in your terminal:
   /bin/bash -c "$(curl -fsSL [https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh](https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh))"

2. Install SFML and dependencies by running:
   brew install sfml freetype libvorbis

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