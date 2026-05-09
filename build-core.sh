#!/bin/bash
echo "Building C++ Simulator Core"

# Set up environment


# Build using emcmake
cd sim
mkdir -p build
cd build
emcmake cmake ..
make -j4

cd ../../

# Create sim-core directory
mkdir -p sim-core/include
rm -rf sim-core/include/*

# Copy compiled library
cp sim/build/libsimcore.a sim-core/

# Copy ImGui static library
cp sim/build/external/libimgui.a sim-core/

# Copy Mosscap headers
cp -r sim/include/* sim-core/include/

# Copy ImGui headers
cp sim/external/imgui/*.h sim-core/include/
cp sim/external/imgui/backends/imgui_impl_glfw.h sim-core/include/
cp sim/external/imgui/backends/imgui_impl_opengl3.h sim-core/include/

# Copy main.cpp into core
cp sim/src/main.cpp sim-core/
