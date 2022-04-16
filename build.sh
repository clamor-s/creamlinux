#!/bin/bash
mkdir build
cd build
cmake ..
make
cp ../package/* ./lib/
