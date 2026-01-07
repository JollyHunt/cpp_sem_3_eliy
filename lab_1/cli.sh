#!/bin/bash

mkdir build
cd build
cmake ..
make

# ./test_LazySequence
# ./test_ReadOnlyStream
# ./test_WriteOnlyStream

./cli

# ./han