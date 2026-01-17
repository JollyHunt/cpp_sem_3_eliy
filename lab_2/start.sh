#!/bin/bash

mkdir -p build
cd build
cmake -DGTest_DIR=/usr/local/lib/cmake/GTest ..
make

valgrind ./main