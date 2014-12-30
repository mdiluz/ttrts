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

echo "Performing install"
sudo make install
if [[ $? != 0 ]]; then
    echo "TTRTS: Install failed, check output"
    exit
fi

# Run final test to make sure
echo "TTRTS: Running tests"
./source/test/ttrts-test
if [[ $? != 0 ]]; then
    echo "TTRTS: Tests failed, build must be broken"
    exit
fi

echo "TTRTS: Bootstrap complete"
