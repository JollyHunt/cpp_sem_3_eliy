#!/bin/bash

rm -rf build
mkdir build
cd build
cmake -DGTest_DIR=/usr/local/lib/cmake/GTest ..
make

if [ -f run_tests ]; then
    ./run_tests
else
    echo "Build failed: run_tests not found"
fi