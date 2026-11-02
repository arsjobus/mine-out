# Browser / WebGL Roadmap

## Goal

Make Mine-Out playable in a desktop web browser from an itch.io game page, with
the existing C++ game and assets reused where practical. The game uses a fixed
1024 × 768 (4:3) play area; the browser canvas should scale to fit while
preserving that aspect ratio.

This is a plan, not a claim that the current native build already supports the
web. The repository currently builds against native SFML 3 and has no
Emscripten target or browser deployment workflow.

## Recommended approach

Use Emscripten to compile to WebAssembly and render through WebGL. First verify
whether the selected, pinned SFML 3 release can build and run with Emscripten,
including its window, graphics, and audio backends. Do not assume native SFML
3 or the existing CMake configuration is browser-compatible.

If that compatibility spike fails, choose an explicit fallback before porting
gameplay: either a maintained Emscripten-compatible SFML port, or a small
platform/render/audio layer backed by SDL2/WebGL while retaining the existing
gameplay code where feasible. Record the decision and any trade-offs here.
Avoid a renderer rewrite until the spike demonstrates it is necessary.

## Phases

### 1. Prove the toolchain and graphics backend

- [ ] Pin an Emscripten SDK version and identify a compatible SFML 3 (or
  fallback backend) version.
- [ ] Build a minimal browser target that opens a canvas, clears it using
  WebGL, and runs its frame callback without a blocking native loop.
- [ ] Confirm the target runs locally in a browser and that its generated
  `.wasm`, JavaScript, and asset files can be served over HTTP.
- [ ] Decide and document the backend based on this test before proceeding.

**Exit criteria:** a reproducible minimal browser build and a backend decision.

### 2. Adapt the application lifecycle and window

- [ ] Replace or wrap the `while` loop in `src/main.cpp` with the selected
  platform's browser frame loop (for Emscripten, a main-loop callback).
- [ ] Configure a browser canvas instead of creating a desktop window; handle
  resize/fullscreen events and preserve the existing 4:3 letterboxed game view.
- [ ] Keep frame timing stable when a tab loses focus or is throttled, and
  verify the game resumes cleanly when focus returns.
- [ ] Map browser keyboard and pointer events to existing controls, including
  preventing page scrolling for gameplay keys when the canvas is active.

**Exit criteria:** title screen, gameplay, state changes, and pause/resume work
in a browser without blocking the browser UI.

### 3. Make assets, audio, and browser-only behavior work

- [ ] Package `assets/` into the browser virtual filesystem (or use the
  backend's equivalent asset loader); make resource and level paths resolve
  consistently instead of relying on native executable paths or the host's
  current working directory.
- [ ] Verify all level data, textures, fonts, sound effects, and music load in
  the web build. Fail visibly for missing required assets.
- [ ] Start or resume audio only after a user gesture, as required by browser
  autoplay policies; test music and effects in supported browsers.
- [ ] Replace writes to `log.txt` and `$HOME/Pictures` with browser-safe
  console logging and a user-triggered screenshot download, or disable those
  native-only features in the web target.
- [ ] Check memory use and initial download size; avoid loading unused resources
  if they materially delay startup.

**Exit criteria:** a complete playthrough works with packaged assets, usable
audio, and no dependence on native filesystem paths.

### 4. Add a repeatable web build

- [ ] Add an opt-in CMake web configuration or a separate web build script so
  native macOS and Windows builds remain unchanged.
- [ ] Add a CI job using the pinned Emscripten SDK to configure, compile, and
  package the browser build.
- [ ] Produce a zip with `index.html` at its root and all required JavaScript,
  WebAssembly, data, and support files alongside it.
- [ ] Keep the output reproducible and surface build or packaging failures in
  CI; do not silently publish an incomplete zip.

**Exit criteria:** CI creates an itch.io-ready HTML5 zip from a clean checkout
without breaking existing native release jobs.

### 5. Test and publish on itch.io

- [ ] Test the packaged build over HTTP, not `file://`, in current Chrome and
  Firefox; include Safari if it is in the intended support matrix.
- [ ] Verify startup, controls, resizing/letterboxing, audio unlock, all levels,
  restart/game-over flow, focus loss and recovery, and browser console errors.
- [ ] Test the actual upload in an itch.io draft page and confirm the game
  embeds and runs in the intended viewport.
- [ ] Publish manually from the validated zip first. Optionally add Butler
  deployment from CI using a repository secret, keeping deployment separate
  from ordinary pull-request builds.

**Release gate:** do not announce the browser version until a clean
itch.io-hosted build passes the browser smoke test and a complete playthrough.

## Known areas to investigate

- `CMakeLists.txt` currently requires native SFML 3 Graphics, Window, System,
  and Audio components.
- `src/main.cpp` owns a blocking loop and writes a startup path based on `$HOME`.
- `src/Window.cpp` creates native-window behavior and saves screenshots to a
  local filesystem path.
- `src/PreloadResources.cpp` probes host filesystem paths and uses SFML file
  loaders; `src/Scenes/Level0.cpp` reads level files directly.
- `src/Log.cpp` writes `log.txt`. Browser builds need a deliberate policy for
  this and any other writes.
- Browser audio generally requires an explicit user interaction before
  playback, even when audio assets are loaded successfully.

## Out of scope for the first release

- Mobile-specific touch controls, offline/PWA support, cloud saves, and online
  multiplayer. Revisit these after desktop browser play is stable.
