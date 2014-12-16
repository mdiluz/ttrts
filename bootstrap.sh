#! /bin/bash

echo "TTRTS: Running cmake"
cd build/
cmake ../source
if [[ $? != 0 ]]; then
    echo "TTRTS: CMake failed, exiting Bootstrap"
fi

echo "TTRTS: Running make"
make
if [[ $? != 0 ]]; then
    echo "TTRTS: make failed, exiting Bootstrap"
fi

echo "TTRTS: Running tests"
./test/ttrts-test
if [[ $? != 0 ]]; then
    echo "TTRTS: Tests failed, build must be broken"
fi

echo "TTRTS: Generating maps"
test ! -e ../maps && mkdir ../maps
cd ../maps
./../build/gen/ttrts-gen
if [[ $? != 0 ]]; then
    echo "TTRTS: Generating maps, exiting Bootstrap"
fi

echo "TTRTS: Moving binaries"
cd ..
if [ ! -e build/ttrts/ttrts ]; then
    echo "TTRTS: No TTRTS Binary found, something has gone wrong"
fi

cp build/ttrts/ttrts .
chmod a+x ttrts
