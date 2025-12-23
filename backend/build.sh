#!/bin/bash
# Compile C++ files on the Render server
echo "Compiling Compressor..."
g++ -o compressor compressor.cpp
echo "Compiling Decompressor..."
g++ -o decompressor decompressor.cpp
echo "Compilation Finished."