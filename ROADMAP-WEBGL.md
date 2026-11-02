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
preloaded asset data all returned HTTP 200. These lifecycle checks are still
only the capability spike; the application loop and input have not yet been
migrated.

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
game objects, scenes, resources, and events currently use SFML types directly
across most of `src/`. Avoid a repository-wide rewrite or a generic abstraction
designed before it is exercised. Keep the native SFML build working, and prove
the boundary with a small end-to-end slice before expanding it:

1. **[x] Validate the SDL2 backend before changing game code.**
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
2. **Define the smallest useful backend seam and prove it on native.**
   - Inventory the SFML use by responsibility (window/events/time, drawing,
     resources, audio); use this to guide migration, not to create a parallel
     SFML-shaped API.
   - Treat startup as part of the vertical slice: the current title is reached
     through an SFML `LoadScreen`, inherits an SFML-based `GameState`, and
     loads fonts through SFML-backed `PreloadResources`. Porting only
     `Title.cpp` would not produce a runnable or backend-independent title.
     Prefer a shared title scene and a small bootstrap that loads only its
     required resources: the native `Title` state delegates to that scene
     after the existing loader, while the web entry point starts the same
     scene directly. This avoids migrating the all-assets loader prematurely
     and avoids duplicating the title UI.
   - Introduce game-owned types and operations only for the first vertical
     slice: window lifecycle, frame time, keyboard/close/focus/resize events,
     4:3 view, text, textures, and filled shapes. Keep SFML conversions inside
     the native adapter.
   - Migrate the shared title scene and both adapters to that seam. Keep the
     existing native executable as the reference implementation and verify
     the title, controls, resize, resource loading, and clean shutdown still
     work.
   - **Checkpoint:** the title slice builds and runs natively through the new
     boundary; no SFML types leak from that slice into its game-facing headers.
3. **Port the title slice to SDL2 before migrating the whole game.**
   - Add an SDL2 adapter used only by the web target; do not disturb native
     target dependencies or behavior.
   - Run the browser loop with a bounded frame delta, focus/visibility pause,
     timing reset on resume, canvas resize/fullscreen handling, and keyboard
     input. Preserve the 1024 × 768 logical view and 4:3 letterboxing.
   - Package only the selected startup path and title slice's required assets
     for this checkpoint. Verify resource loading, title, text, input, resize,
     focus recovery, and shutdown over HTTP in a browser before generalizing
     the interface.
   - **Checkpoint:** a real game title screen runs in the browser without
     linking SFML; the native title screen remains functional.
4. **Migrate gameplay in independently verifiable slices.**
   - Move drawing and shape inheritance out of game objects, then migrate
     gameplay types in dependency order: shared objects and collision
     geometry; paddle and ball; blocks and power-ups; level and other scenes.
   - Route resource loading and audio through the backend at the point each
     slice needs them. Keep SDL-specific calls in the adapter and preserve
     stable resource handles/lifetimes across scene changes.
   - After each slice, build both targets and verify the affected gameplay
     behavior; do not defer all native verification until the end.
   - **Checkpoint:** title, all levels, transitions, restart/game-over, and
     audio work in both targets; no game-facing source depends on SFML.
5. **Finish browser lifecycle and input behavior.**
   - Ensure callback cancellation, state transitions, and resource cleanup
     remain valid on exit. Prevent page scrolling for gameplay keys while the
     canvas is active; add pointer input only if a game control requires it.
   - **Checkpoint:** verify gameplay, state changes, pause/resume, responsive
     browser UI, and 4:3 resizing in a browser.

**Exit criteria:** title screen, gameplay, state changes, and pause/resume work
in a browser without blocking the browser UI.

**Current status (2026-11-02):** step 1 is complete: the SDL2 feature spike
builds, serves, and passes its browser capability checks, and the native SFML
build succeeds. The game itself still requires SFML 3 and cannot configure
with the pinned Emscripten toolchain. Step 2 is in progress: shared title
presentation and start/exit decisions now live in `TitleScene`, with the
existing native `Window` providing an SFML renderer adapter. This is only the
first slice; `Title` still uses SFML for its state/event lifecycle, and startup
still enters through the SFML `LoadScreen` and loads all resources. Next,
extract the title slice's event/frame contract and a focused resource bootstrap
so the same title scene can be started by native and web entry points. Do not
mark the checkpoint or later phases complete until both native and browser
startup paths are verified; gameplay has not yet been migrated.

### 3. Make assets, audio, and browser-only behavior work

- [ ] Package `assets/` into the browser virtual filesystem (or use the
  backend's equivalent asset loader); make resource and level paths resolve
  consistently instead of relying on native executable paths or the host's
  current working directory. Phase 2 may package only the title slice's
  required files for its browser checkpoint; complete and verify the full
  asset set here.
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
- The project layout assumes a native filesystem with an executable working
  directory, so the browser build must replace any path-dependent loading,
  screenshots, and log output with browser-safe equivalents.
- Title, gameplay, and scene transitions all currently depend on the native
  SFML event and frame lifecycle; migrating them requires a backend seam that
  is proven with a real title slice before generalizing to gameplay.
- Browser audio must be gated behind user interaction and work with the Emscripten
  SDL2 audio stack rather than native OS-level sound modules.
- The project currently ships as a desktop executable target. The web package
  must be opt-in and isolated from the existing macOS/Windows build pipeline.

## Immediate next actions

The work after the SDL2 capability spike should remain narrow and end-to-end:

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
  SDK does not support SFML 3 directly. The SDL2 route is correct for the
  browser target, but it means the game's runtime contracts must be separated
  from SFML before any browser compatibility can be proved.
- **Asset path assumptions:** game resources may currently rely on executable
  location or local filesystem state. The browser build must package assets and
  resolve paths consistently from a virtual filesystem or generated loader.
- **Audio policy mismatch:** browsers block media playback until a user gesture,
  so music and effects must be scheduled only after input unlock.
- **Resize and input fidelity:** the game's 1024 × 768 fixed world must remain
  stable while the browser window changes size; letterboxing and input
  prevention need to be validated under real browser resize and focus changes.
- **Release readiness:** a browser build is not complete if it merely compiles;
  it must be packaged reproducibly, tested over HTTP, and deployed only after
  passing browser smoke and gameplay checks on itch.io.

## Proposed release checklist for the browser target

- [ ] Browser build configured through an opt-in CMake option or a dedicated
  web build script.
- [ ] Native build remains unchanged and still compiles on macOS/Windows.
- [ ] Browser target runs over HTTP with a 4:3 canvas, valid WebGL 2 context,
  and packaged assets.
- [ ] Title screen and core controls run without native SFML dependencies.
- [ ] Gameplay slices are migrated and verified in native + browser targets.
- [ ] Audio unlock flow works after a user gesture in supported browsers.
- [ ] Browser packaging produces a reproducible zip with all assets and runtime
  files at the root.
- [ ] itch.io upload passes smoke tests and a complete playthrough.

This roadmap remains intentionally conservative: minimize scope, validate the
backend seam early, and only expand the browser port after each slice proves the
native behavior and web runtime remain aligned.
