#! /bin/bash

# Double check for cmakelist
if [ ! -e "CMakeLists.txt" ]; then
	echo "TTRTS: No source cmakelist found"
	exit
fi

# Run cmake
echo "TTRTS: Running cmake"
test ! -e build && mkdir build
cd build/
cmake ..
if [[ $? != 0 ]]; then
    echo "TTRTS: CMake failed, exiting Bootstrap"
    exit
fi

# Run make
echo "TTRTS: Running make"
make
if [[ $? != 0 ]]; then
    echo "TTRTS: make failed, exiting Bootstrap"
    exit
fi

# Run tests
echo "TTRTS: Running tests"
./source/test/ttrts-test
if [[ $? != 0 ]]; then
    echo "TTRTS: Tests failed, build must be broken"
    exit
fi

# Generate maps
echo "TTRTS: Generating maps"
cd ..
./scripts/gen_maps.sh "$PWD/build/source/gen/ttrts-gen"
if [[ $? != 0 ]]; then
    echo "TTRTS: Failed to generate maps, exiting Bootstrap"
    exit
fi

# Move binaries
echo "TTRTS: Moving binaries"
if [ ! -e build/source/client/ttrts ]; then
    echo "TTRTS: No TTRTS Binary found, something has gone wrong"
    exit
fi

cp build/source/client/ttrts .
chmod a+x ttrts

echo "TTRTS: Bootstrap complete"
