#!/bin/bash

# set the build directory
BUILD_DIR="build"

# run the executable with any arguments passed to the script ($@ represents all passed arguments)
echo "Running the program with arguments: $@"
echo ""
"$BUILD_DIR"/splc "$@"
