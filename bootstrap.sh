#! /bin/bash

# Double check
if [ ! -e "source/CMakeLists.txt" ]; then
	echo "TTRTS: No source cmakelist found"
	exit
fi

echo "TTRTS: Running cmake"
test ! -e build && mkdir build
cd build/
cmake ../source
if [[ $? != 0 ]]; then
    echo "TTRTS: CMake failed, exiting Bootstrap"
    exit
fi

echo "TTRTS: Running make"
make
if [[ $? != 0 ]]; then
    echo "TTRTS: make failed, exiting Bootstrap"
    exit
fi

echo "TTRTS: Running tests"
./test/ttrts-test
if [[ $? != 0 ]]; then
    echo "TTRTS: Tests failed, build must be broken"
    exit
fi

echo "TTRTS: Generating maps"
./../scripts/gen_maps.sh "$PWD/gen/ttrts-gen"
if [[ $? != 0 ]]; then
    echo "TTRTS: Failed to generate maps, exiting Bootstrap"
    exit
fi
# Copy these maps into parent directory
cp -rf maps ..

echo "TTRTS: Moving binaries"
cd ..
if [ ! -e build/ttrts/ttrts ]; then
    echo "TTRTS: No TTRTS Binary found, something has gone wrong"
    exit
fi

cp build/ttrts/ttrts .
chmod a+x ttrts

echo "TTRTS: Bootstrap complete"
