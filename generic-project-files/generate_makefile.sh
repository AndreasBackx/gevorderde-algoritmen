
#!/usr/bin/env bash

if [ ! -e build ]
then
    mkdir build
fi

# Use the following commands to generate the Makefile

cd build

if [ -x "$(command -v clang)" ]
then
    cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

    # ln -s compile_commands.json .. # cmake compile_commands.json generation does not generate info for header files
    # use compdb instead until this is fixed (https://github.com/Sarcasm/compdb)
    if [ -x "$(command -v compdb)" ]
    then
        compdb list > ../compile_commands.json
    else
        echo "Warning: compdb is not installed" >&2
    fi
else
    cmake ..
    echo "Warning: clang is not installed, using default compiler" >&2
fi

