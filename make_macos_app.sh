#!/bin/bash
set -e

APPNAME=MineOut
BINARY=game
BUILDDIR=build
APPDIR="$BUILDDIR/$APPNAME.app"

# Build the binary
cmake --build $BUILDDIR

# Create .app bundle structure
mkdir -p "$APPDIR/Contents/MacOS"
mkdir -p "$APPDIR/Contents/Resources"

# Copy binary
cp "$BUILDDIR/$BINARY" "$APPDIR/Contents/MacOS/$APPNAME"

# Copy resources
cp -R data fonts images music sfx settings.ini "$APPDIR/Contents/Resources/"

# Create Info.plist if missing
if [ ! -f "$APPDIR/Contents/Info.plist" ]; then
cat > "$APPDIR/Contents/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>$APPNAME</string>
    <key>CFBundleIdentifier</key>
    <string>com.yourcompany.mineout</string>
    <key>CFBundleName</key>
    <string>$APPNAME</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleVersion</key>
    <string>1.0</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0</string>
</dict>
</plist>
EOF
fi

echo "App bundle created at $APPDIR"
