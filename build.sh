#!/bin/bash

# Garage Door Opener Build Script
# This script builds and flashes the ESP32 garage door opener firmware

echo "Building Garage Door Opener firmware..."

# Set the RainMaker path if not already set
if [ -z "$RMAKER_PATH" ]; then
    export RMAKER_PATH=/Users/phil/esp/esp-rainmaker
    echo "Set RMAKER_PATH to $RMAKER_PATH"
fi

# Build the project
echo "Building project..."
idf.py build

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo ""
    echo "To flash the firmware, run:"
    echo "  idf.py flash"
    echo ""
    echo "To flash and monitor, run:"
    echo "  idf.py flash monitor"
    echo ""
    echo "To configure the project, run:"
    echo "  idf.py menuconfig"
else
    echo "Build failed!"
    exit 1
fi
