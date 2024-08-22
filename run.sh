#!/bin/bash

# check if the build directory exists, if not, create it
if [ ! -d build ]; then
  mkdir build
fi

# navigate to the build directory
cd build

# run CMake to configure the project
cmake ..

# build the project
make splc

# run the program
./splc

# navigate back to the root directory
cd ..