
#!/usr/bin/env bash

if [ ! -e build ]
then
    mkdir build
fi

# Use the following commands to generate the Makefile

cd build

if [ -x "$(command -v clang)" ]
then
    cmake .. \
        -DCMAKE_C_COMPILER=clang \
        -DCMAKE_CXX_COMPILER=clang++
else
    cmake ..
    echo "Warning: clang is not installed, using default compiler." >&2
fi

# Use the following commands to compile and debug (no need to run cmake everytime)
#
# scan-build make
# make test
