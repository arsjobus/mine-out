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
still needs to verify which existing logic can be retained.

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

### 2. Adapt the application lifecycle and window

- [ ] Replace or wrap the `while` loop in `src/main.cpp` with the selected
  platform's browser frame loop (for Emscripten, a main-loop callback).
- [ ] Configure a browser canvas instead of creating a desktop window; handle
  resize/fullscreen events and preserve the existing 4:3 letterboxed game view.
- [ ] Keep frame timing stable when a tab loses focus or is throttled, and
  verify the game resumes cleanly when focus returns.
- [ ] Map browser keyboard and pointer events to existing controls, including
  preventing page scrolling for gameplay keys when the canvas is active.

#### Implementation plan

The application cannot move to a browser loop independently of its renderer:
game objects, scenes, resources, and events currently use SFML types directly.
Make the migration in these slices, keeping the existing native SFML build
working throughout:

1. **Validate the SDL2 backend before changing game code.**
   - Extend the spike to test the rendering operations the game needs
     (textures, filled shapes, text, alpha blending, and a 1024 × 768 logical
     canvas), plus keyboard/focus/resize events.
   - Check the pinned Emscripten SDK's available image, font, and audio
     backends; build a small proof for each required format, including OGG
     music and WAV effects. Do not assume SDL2's core port provides these.
   - Record the chosen SDL2 APIs and any required ports/libraries. If a
     required capability is unavailable, resolve the backend choice here
     rather than after migrating gameplay.
   - **Checkpoint:** the pinned toolchain builds and serves the feature spike;
     the native SFML build remains unchanged.
2. **Create a backend boundary.**
   - Introduce game-facing window, event, time, drawing, resource, and audio
     interfaces without SFML types in gameplay headers.
   - Implement the current native behavior behind an SFML adapter first and
     keep native window creation, controls, and rendering working.
   - Replace SFML shape inheritance and direct `window.draw(...)` calls with
     backend-neutral game objects/draw operations. Migrate resources and
     audio through the same boundary instead of adding SDL calls throughout
     gameplay.
   - **Checkpoint:** the native game builds and its title, levels, transitions,
     audio, and controls behave as before; gameplay code no longer depends
     directly on SFML.
3. **Implement and exercise the SDL2 backend.**
   - Add an SDL2 implementation for the interfaces and enable it only for the
     web target; leave the native SFML target and its dependencies intact.
   - Render into a fixed 1024 × 768 logical view, scaling and letterboxing it
     in the canvas on resize. Keep input coordinates consistent with that view.
   - Run a browser title-screen slice first, then migrate and verify gameplay,
     scene transitions, restart/game-over, and pause/resume.
   - **Checkpoint:** a browser build starts and plays through the game using
     the SDL2 backend, with no SFML dependency in the web target.
4. **Move lifecycle and input onto the browser loop.**
   - Replace the blocking web loop with Emscripten's main-loop callback while
     retaining the native loop. Ensure state transitions and cleanup remain
     valid when the callback is cancelled or the game exits.
   - Use elapsed frame time with a bounded delta; do not advance gameplay
     while the page is hidden or focus is lost, and reset timing on resume to
     avoid a large catch-up step.
   - Map keyboard events to the existing controls, prevent browser scrolling
     for gameplay keys while the canvas is active, and handle focus changes.
     Add pointer input only if an existing control requires it.
   - **Checkpoint:** verify title, gameplay, state changes, and pause/resume in
     a browser; resizing preserves the 4:3 view and browser UI remains
     responsive.

**Exit criteria:** title screen, gameplay, state changes, and pause/resume work
in a browser without blocking the browser UI.

**Current status:** phase blocked at implementation-plan step 1. The existing
SDL2 spike proves only window/context creation and frame callbacks; it does
not yet prove the game-required rendering, font, image, or audio capabilities.
The application still requires SFML 3 and cannot configure with the pinned
Emscripten toolchain. Do not mark this phase complete until all four
checkpoints and the exit criteria pass. The native loop's long-frame clamp and
focus-loss pause are useful safeguards, but do not satisfy the browser
criteria.

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
