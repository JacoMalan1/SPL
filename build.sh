#!/bin/bash

# set the build directory
BUILD_DIR="build"

# run CMake to configure the project, specifying the source (-S) and build (-B) directories
echo "Running CMake..."
cmake -S . -B "$BUILD_DIR" || { echo "CMake configuration failed"; }

# build the project using the build directory
echo "Building the project..."
cmake --build "$BUILD_DIR" || { echo "Build failed";}