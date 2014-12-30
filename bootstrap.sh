#! /bin/bash

# Double check for cmakelist
if [ ! -e "source/CMakeLists.txt" ]; then
	echo "TTRTS: No source cmakelist found"
	exit
fi

# Run cmake
echo "TTRTS: Running cmake"
test ! -e build && mkdir build
cd build/
cmake ../source
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
./test/ttrts-test
if [[ $? != 0 ]]; then
    echo "TTRTS: Tests failed, build must be broken"
    exit
fi

# Generate maps
echo "TTRTS: Generating maps"
./../scripts/gen_maps.sh "$PWD/gen/ttrts-gen"
if [[ $? != 0 ]]; then
    echo "TTRTS: Failed to generate maps, exiting Bootstrap"
    exit
fi
# Copy these maps into parent directory
cp -rf maps ..

# Move binaries
echo "TTRTS: Moving binaries"
cd ..
if [ ! -e build/client/ttrts ]; then
    echo "TTRTS: No TTRTS Binary found, something has gone wrong"
    exit
fi

cp build/client/ttrts .
chmod a+x ttrts

echo "TTRTS: Bootstrap complete"
