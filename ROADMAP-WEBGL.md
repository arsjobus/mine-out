# Browser / WebGL Roadmap

## Goal

Make Mine-Out playable in a desktop web browser from an itch.io game page, with
the existing C++ game and assets reused where practical. The game uses a fixed
1024 × 768 (4:3) play area; the browser canvas should scale to fit while
preserving that aspect ratio.

This roadmap tracks the browser port; the native build remains unchanged.

## Recommended approach

Use Emscripten to compile to WebAssembly and render through WebGL. The
compatibility spike found that the pinned SFML 3 release does not support
Emscripten. The selected fallback is Emscripten's SDL2 port with WebGL; add a
small platform/render/audio adapter and retain gameplay logic where feasible.
Do not assume the existing SFML code can be recompiled unchanged, and avoid a
renderer rewrite until the port demonstrates it is necessary.

## Phases

### 1. Prove the toolchain and graphics backend

- [x] Pin an Emscripten SDK version and identify a compatible SFML 3 (or
  fallback backend) version.
- [x] Build a minimal browser target that opens a canvas, clears it using
  WebGL, and runs its frame callback without a blocking native loop.
- [x] Confirm the target runs locally in a browser and that its generated
  `.wasm`, JavaScript, and asset files can be served over HTTP.
- [x] Decide and document the backend based on this test before proceeding.

**Exit criteria:** a reproducible minimal browser build and a backend decision.

**Decision record (2026-11-02):** Pin Emscripten SDK 6.0.13 and SFML 3.0.2
for the compatibility result. Configuring SFML 3.0.2 with Emscripten fails
in its platform detection with “Unsupported operating system or environment”;
its CMake platform list does not include Emscripten. Use the SDL2 2.32.10 port
bundled with the pinned Emscripten SDK as the fallback, with WebGL rendering
and an adapter for platform, rendering, and audio APIs. Gameplay migration
can reuse the existing game source through a narrow compatibility adapter.

The standalone proof targets are in `web-spike/`. Install and activate the
pinned SDK from an emsdk checkout:

```sh
./emsdk install 6.0.13
./emsdk activate 6.0.13
source ./emsdk_env.sh
```

Then, from the project root:

```sh
emcmake cmake -S web-spike -B build/web-spike
cmake --build build/web-spike
python3 -m http.server 8000 --directory build/web-spike
```

Open `http://localhost:8000/webgl-spike.html` or
`http://localhost:8000/sdl2-spike.html` in a browser. The first target creates
a WebGL 2 context directly; the second creates an SDL2 window and WebGL 2
context. Both clear the canvas on Emscripten's non-blocking browser main loop.
Verified over HTTP in Firefox: the HTML and `.wasm` return 200, WebGL 2
initializes, and both frame callbacks run repeatedly. Emscripten 6.0.13
retrieved and built its SDL2 2.32.10 port for the SDL2 proof.

The `sdl2-capabilities` target extends that proof with an SDL renderer using a
1024 × 768 logical size, a PNG texture, OTF text, filled rectangles, alpha
blending, resize/focus/keyboard event handling, and OGG/WAV playback initiated
by a key press. It packages `assets/` into the Emscripten virtual filesystem.
Build and serve it with:

```sh
cmake --build build/web-spike --target sdl2-capabilities
python3 -m http.server 8000 --directory build/web-spike
```

Open `http://localhost:8000/sdl2-capabilities.html`;
`window.sdl2Capabilities` reports resource-load readiness, frame timing, and
keyboard/focus/resize state. Press a key to unlock music and press E to play
the WAV effect. The implementation uses SDL_Renderer logical sizing/drawing,
SDL_image PNG (`SDL2_IMAGE_FORMATS=["png"]` is required), SDL_ttf with the
SDK's FreeType/Harfbuzz ports, and SDL_mixer with its OGG, Vorbis, and WAV
support. The pinned SDK successfully compiled and linked this target,
retrieving SDL_image 2.6.0, SDL_ttf 2.20.2, SDL_mixer 2.8.0, FreeType,
Harfbuzz, Ogg, and Vorbis ports. In Firefox 159 over HTTP, all four real game
assets loaded and rendered; a keyboard action started OGG music and the WAV
effect; browser blur/refocus paused and resumed its frame timer; gameplay
arrow-key defaults were prevented; and resizing the browser resized the
canvas to a 4:3 size and produced SDL resize events. The observed frame delta
remained bounded at 100 ms. The generated HTML, JavaScript, WebAssembly, and
preloaded asset data all returned HTTP 200. The application now uses the same
validated SDL2/WebGL path through its browser compatibility adapter.

### 2. Adapt the application lifecycle and window

- [x] Replace or wrap the `while` loop in `src/main.cpp` with the selected
  platform's browser frame loop (for Emscripten, a main-loop callback).
- [x] Configure a browser canvas instead of creating a desktop window; preserve
  the 4:3 view during viewport resize and browser/itch fullscreen.
- [x] Clamp frame deltas and handle focus events so throttled/background frames
  do not advance gameplay by a large amount.
- [x] Map browser keyboard and pointer events to existing controls, including
  preventing page scrolling for gameplay keys when the canvas is active.

#### Implemented backend boundary

The existing game code uses SFML types throughout its gameplay and scene
sources. Rather than rewrite all game objects, `web/compat/SFML/` supplies the
small SFML-shaped API subset those sources use, implemented with Emscripten's
SDL2, SDL_image, SDL_ttf, and SDL_mixer ports. The native target still links
the real SFML 3 libraries; the web target does not link SFML. This is a
deliberate compatibility adapter, not a game-owned rendering abstraction.
SDL calls remain confined to the compatibility layer.

- [x] Keep the native SFML target separate and buildable.
- [x] Drive the browser application with Emscripten's non-blocking main loop.
- [x] Render the existing title, gameplay objects, levels, transitions, and
  other scenes through the SDL2 adapter.
- [x] Preserve a 1024 × 768 logical world in a centered, responsive 4:3 canvas.
- [x] Clamp frame deltas and map focus, keyboard, and close events; prevent
  gameplay keys from scrolling the page.
- [x] Release cached rendering and audio resources during browser shutdown.

**Exit criteria:** title screen, gameplay, state changes, and pause/resume work
in a browser without blocking the browser UI.

**Status (2026-11-02):** the complete shared game builds for WebAssembly, and
the packaged browser build has been served and exercised in headless Firefox.
The title transitions to Level 1, gameplay assets render, WebGL 2 initializes,
the canvas remains 4:3 at multiple viewport sizes, and the keyboard controls
do not scroll the page. Native SFML and web builds both compile.

### 3. Make assets, audio, and browser-only behavior work

- [x] Package `assets/` into the browser virtual filesystem (or use the
  backend's equivalent asset loader); make resource and level paths resolve
  consistently instead of relying on native executable paths or the host's
  current working directory.
- [x] Verify packaged assets resolve from the Emscripten virtual filesystem.
- [x] Start gameplay audio only after the title-screen key gesture.
- [x] Replace writes to `log.txt` and `$HOME/Pictures` with browser-safe
  console logging and a user-triggered screenshot download, or disable those
  native-only features in the web target.
- [x] Package all assets in a roughly 2.9 MB upload ZIP; the source payload is
  about 4.9 MB before ZIP compression.

**Implementation complete.** Audible playback and a complete playthrough
remain visible-browser release checks.

### 4. Add a repeatable web build

- [x] Add an opt-in CMake web configuration or a separate web build script so
  native macOS and Windows builds remain unchanged.
- [x] Add a CI job using the pinned Emscripten SDK to configure, compile, and
  package the browser build.
- [x] Produce a zip with `index.html` at its root and all required JavaScript,
  WebAssembly, data, and support files alongside it.
- [x] Keep the output reproducible and surface build or packaging failures in
  CI; do not silently publish an incomplete zip.

**Exit criteria:** implemented; native and Emscripten builds pass locally. The
new CI job will additionally verify a clean hosted-runner build.

### 5. Test and publish on itch.io

- [x] Test the packaged build over HTTP, not `file://`, in Firefox with WebGL 2.
- [x] Verify title startup, Level 1 gameplay, keyboard interaction, 4:3
  resizing, and pause/restart keys in the browser.
- [ ] Verify audible music/effects, all levels, full restart/game-over flow,
  focus loss/recovery, and browser console errors in a visible browser.
- [ ] Test the actual upload in an itch.io draft page and confirm the game
  embeds and runs in the intended viewport.
- [ ] Publish manually from the validated zip first. Optionally add Butler
  deployment from CI using a repository secret, keeping deployment separate
  from ordinary pull-request builds.

**Release gate:** do not announce the browser version until a clean
itch.io-hosted build passes the browser smoke test and a complete playthrough.

## Resolved architecture notes

- The native build links SFML 3; the web build excludes those libraries and
  uses the SDL2-backed adapter under `web/compat/`.
- The complete `assets/` directory is preloaded into the Emscripten filesystem,
  including level data, textures, fonts, and sound assets.
- Browser logging goes to the console; native screenshot saving is disabled
  for web builds.
- Browser audio is resumed from canvas keyboard input; game music begins after
  the title-screen gesture.
- The browser target is isolated from native platform dependencies and release
  packaging.

## Superseded implementation plan

The initial slice-by-slice plan below was replaced by the compatibility
adapter described above. It is retained as historical context, not as pending
work.

1. **Lock the browser-safe title seam**
   - Extract the event/frame contract from `Title` and `TitleScene` so the same
     scene can be driven by both native and browser adapters.
   - Keep all SFML-specific conversions inside the native adapter and avoid
     duplicating title logic between the web and desktop branches.
   - Verify the title state still loads and exits correctly on native before
     moving the same seam to Emscripten.

2. **Prove the browser startup path**
   - Build a single web target that opens a 4:3 canvas, starts the title scene,
     and loads only the title assets required to render the screen.
   - Confirm keyboard input, focus loss/resume, resize handling, and asset
     loading work over HTTP in a browser before expanding the migrated slice.
   - Keep the browser checkpoint focused on startup and rendering, not a full
     gameplay port.

3. **Split resource ownership from native assumptions**
   - Replace implicit relative asset paths with explicit asset roots and a web
     browser packaging convention.
   - Move screenshot/logging behavior to browser-safe paths or disable them in
     the web target.
   - Verify this with a minimal asset set, then expand to the complete game data.

4. **Prepare the first browser gameplay slice**
   - Migrate one small gameplay dependency chain: shared objects and collision
     geometry, then the paddle/ball or a single level slice.
   - Keep native verification after each slice to avoid a large rewrite without
     test coverage.
   - Treat each slice as a checkpoint with a clear list of build and runtime
     validations.

## Risk register

- **Toolchain mismatch:** the project is pinned to SFML 3, but the Emscripten
  SDK does not support SFML 3 directly. Shared sources instead compile against
  the SDL2-backed adapter under `web/compat/SFML/`.
- **Audio policy mismatch:** browsers block media playback until a user gesture,
  so gameplay audio starts after the title key gesture. Verify audible output
  in a visible browser before release.
- **Release readiness:** the reproducible package and Firefox smoke test exist,
  but the complete playthrough and itch.io-hosted embed remain release gates.

## Proposed release checklist for the browser target

- [x] Browser build configured through an opt-in CMake toolchain or dedicated
  web build script.
- [x] Native build remains unchanged and still compiles on macOS.
- [x] Browser target runs over HTTP with a 4:3 canvas, valid WebGL 2 context,
  and packaged assets.
- [x] Title and Level 1 gameplay run without linking native SFML.
- [x] Shared gameplay source is included in the web target.
- [ ] Audio playback, complete gameplay, and focus recovery pass visible-browser
  testing.
- [x] Browser packaging produces a reproducible zip with all assets and runtime
  files at the root.
- [ ] itch.io upload passes smoke tests and a complete playthrough.

The original slice-by-slice plan is retained for historical context; current
release readiness is summarized above.

## Current release readiness

The browser target is ready to build and upload. The remaining visible-browser
and itch.io checks are release gates, not implementation tasks.
