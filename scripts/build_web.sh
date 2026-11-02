#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$ROOT_DIR/build/web}"
PACKAGE_DIR="$BUILD_DIR/itchio"
ZIP_PATH="${ZIP_PATH:-$ROOT_DIR/mine-out-html5.zip}"

if ! command -v emcmake >/dev/null 2>&1 || ! command -v emcc >/dev/null 2>&1; then
    echo "Emscripten 6.0.13 is required. Install/activate it with emsdk, then source emsdk_env.sh." >&2
    exit 1
fi

EMSCRIPTEN_VERSION="$(emcc --version | sed -n '1p')"
if [[ "$EMSCRIPTEN_VERSION" != *"6.0.13"* ]]; then
    echo "Expected Emscripten 6.0.13; found: $EMSCRIPTEN_VERSION" >&2
    exit 1
fi

emcmake cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR" --target game --parallel

for artifact in game.html game.js game.wasm game.data; do
    if [[ ! -s "$BUILD_DIR/$artifact" ]]; then
        echo "Browser build did not produce required artifact: $artifact" >&2
        exit 1
    fi
done

cmake -E rm -rf "$PACKAGE_DIR"
cmake -E make_directory "$PACKAGE_DIR"
cp "$BUILD_DIR/game.html" "$PACKAGE_DIR/index.html"
cp "$BUILD_DIR/game.js" "$BUILD_DIR/game.wasm" "$BUILD_DIR/game.data" "$PACKAGE_DIR/"
cmake -E rm -f "$ZIP_PATH"
python3 - "$ZIP_PATH" "$PACKAGE_DIR" <<'PY'
import pathlib
import sys
import zipfile

archive_path = pathlib.Path(sys.argv[1])
package_dir = pathlib.Path(sys.argv[2])
with zipfile.ZipFile(archive_path, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=9) as archive:
    for name in ("index.html", "game.js", "game.wasm", "game.data"):
        info = zipfile.ZipInfo(name, date_time=(1980, 1, 1, 0, 0, 0))
        info.compress_type = zipfile.ZIP_DEFLATED
        info.external_attr = 0o100644 << 16
        archive.writestr(info, (package_dir / name).read_bytes(), compress_type=zipfile.ZIP_DEFLATED, compresslevel=9)
PY
echo "Created itch.io HTML5 upload: $ZIP_PATH"
