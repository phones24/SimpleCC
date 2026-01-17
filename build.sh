#!/bin/bash

set -e

BUILD_DIR="build"
BUILD_TYPE="${1:-Release}"

echo "=== SimpleCC VST3 Plugin Build ==="
echo "Build type: $BUILD_TYPE"

# Check dependencies
echo "Checking dependencies..."
for cmd in cmake g++ git pkg-config; do
    if ! command -v $cmd &> /dev/null; then
        echo "Error: $cmd is required but not installed."
        exit 1
    fi
done

# Check for required dev packages (Linux)
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    MISSING_PKGS=""
    
    # Check pkg-config for required libraries
    for pkg in alsa freetype2 x11 xinerama xrandr xcursor; do
        if ! pkg-config --exists $pkg 2>/dev/null; then
            MISSING_PKGS="$MISSING_PKGS $pkg"
        fi
    done
    
    if [ -n "$MISSING_PKGS" ]; then
        echo ""
        echo "Missing development packages. Install with:"
        echo "  sudo apt install libasound2-dev libfreetype6-dev libx11-dev libxinerama-dev libxrandr-dev libxcursor-dev libxcomposite-dev libgl1-mesa-dev"
        echo ""
        exit 1
    fi
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure
echo "Configuring..."
cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

# Build
echo "Building..."
cmake --build . --config "$BUILD_TYPE" -j$(nproc)

echo ""
echo "=== Build Complete ==="
echo "VST3 plugin location:"
find . -name "*.vst3" -type d 2>/dev/null | head -5

echo ""
echo "To install, copy the .vst3 folder to ~/.vst3/"
